/*
Copyright (c) <2017> <Udo Lugauer>
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), 
to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, 
and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, 
DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE
*/

/// @file vulkanRenderDevice.cpp
///       Vulkan render device

#include "vulkanInstance.h"
#include "vulkanRenderDevice.h"
#include "vulkanPhysicalDevice.h"
#include "vulkanConversion.h"
#include "vulkanMemoryManager.h"
#include "vulkanSwapChain.h"
#include "vulkanCommandPool.h"
#include "vulkanShader.h"
#include "vulkanVertexInput.h"
#include "vulkanInputAssembly.h"
#include "vulkanViewportAndScissor.h"
#include "vulkanRasterizerState.h"
#include "vulkanMultisample.h"
#include "vulkanDepthStencil.h"
#include "vulkanColorBlend.h"
#include "vulkanDynamicState.h"
#include "vulkanPipelineLayout.h"
#include "vulkanRenderPass.h"
#include "vulkanGraphicsPipeline.h"
#include "vulkanSemaphore.h"
#include "vulkanFence.h"
#include "vulkanCommandPool.h"
#include "vulkanCommandBuffer.h"
#include "vulkanDescriptorPool.h"
#include "vulkanDescriptorSet.h"
#include "vulkanBuffer.h"
#include "vulkanImage.h"
#include "vulkanImageView.h"
#include "vulkanSampler.h"
#include "vulkanFrameBuffer.h"
#include "vulkanConversion.h"
#include "vulkanApi.h"

#include<limits>
#include<set>

namespace cave
{

VulkanRenderDevice::VulkanRenderDevice(VulkanInstance* instance, VulkanPhysicalDevice* physicalDevice, VkSurfaceKHR surface)
	: HalRenderDevice(instance)
	, _pInstance(instance)
	, _pPhysicalDevice(physicalDevice)
	, _pMemoryManager(nullptr)
	, _presentationSurface(surface)
	, _vkDevice(VK_NULL_HANDLE)
	, _presentQueue(VK_NULL_HANDLE)
	, _graphicsQueue(VK_NULL_HANDLE)
	, _pSwapChain(nullptr)
	, _presentQueueCommandPool(VK_NULL_HANDLE)
	, _graphicsQueueCommandPool(VK_NULL_HANDLE)
	, _presentCommandBufferArray(nullptr)
    , _presentRenderPass(nullptr)
	, _presentationFramebuffers(instance->GetEngineAllocator())
{
	std::set<int> uniqueQueueFamilies;
	// First query the graphics queue index
	_graphicsQueueFamilyIndex = physicalDevice->GetQueueFamilyIndex(VkQueueFlagBits::VK_QUEUE_GRAPHICS_BIT);
	if (_graphicsQueueFamilyIndex == (std::numeric_limits<uint32_t>::max)())
		throw BackendException("CreateRenderDevice: no suitable device queues found");

	uniqueQueueFamilies.insert(_graphicsQueueFamilyIndex);

	_presentationQueueFamilyIndex = (std::numeric_limits<uint32_t>::max)();
	if (surface)
	{
		_presentationQueueFamilyIndex = physicalDevice->GetPresentationQueueFamilyIndex(_graphicsQueueFamilyIndex, surface);
		if (_presentationQueueFamilyIndex == (std::numeric_limits<uint32_t>::max)())
			throw BackendException("CreateRenderDevice: no suitable device queues found");

		uniqueQueueFamilies.insert(_presentationQueueFamilyIndex);
	}


	// setup extensions
	physicalDevice->SetupPhysicalDeviceExtension(_deviceExtensions);

	std::vector<const char*> extensions;
	if (surface && _deviceExtensions.caps.bits.bSwapChainSupport)
	{
		extensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
	}

	// enable minimum features
	VkPhysicalDeviceFeatures supportedFeatures = _pPhysicalDevice->GetPhysicalDeviceFeatures();

	// store some device features for later queries
	_deviceFeatures.caps.bits.bTextureCompressionBC = supportedFeatures.textureCompressionBC;
    _deviceFeatures.caps.bits.bSamplerAnisotropy = supportedFeatures.samplerAnisotropy;

	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
	float queuePriority = 1.0f;
	for (int queueFamily : uniqueQueueFamilies)
	{
		VkDeviceQueueCreateInfo queueCreateInfo = {};
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.queueFamilyIndex = queueFamily;
		queueCreateInfo.queueCount = 1;
		queueCreateInfo.pQueuePriorities = &queuePriority;
		queueCreateInfos.push_back(queueCreateInfo);
	}

	VkDeviceCreateInfo deviceCreateInfo =
	{
		VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,           
		nullptr,                                        
		0,                                              
		static_cast<uint32_t>(queueCreateInfos.size()),
		&queueCreateInfos[0],
		0,                                             
		nullptr,                                       
		static_cast<uint32_t>(extensions.size()),
		&extensions[0],
		&supportedFeatures
	};

	VkResult result = VulkanApi::GetApi()->vkCreateDevice(physicalDevice->GetPhysicalDeviceHandle(), &deviceCreateInfo, nullptr, &_vkDevice);

	if (result != VK_SUCCESS)
	{
		throw BackendException("Failed to create vulkan device");
	}

	uint32_t major, minor, patch;
	physicalDevice->GetApiVersion(major, minor, patch);
	// finally load device level functions
	if (!VulkanApi::GetApi()->LoadDeviceFunctions(&_vkDevice, VK_MAKE_VERSION(major, minor, patch)))
	{
		throw BackendException("Failed to create vulkan device");
	}

	// create device memory manager
	_pMemoryManager = AllocateObject<VulkanMemoryManager>(*_pInstance->GetEngineAllocator(), instance, physicalDevice, this);
	if (!_pMemoryManager)
	{
		throw BackendException("Failed to create vulkan device");
	}

	// get presentation queue
	if (surface)
		VulkanApi::GetApi()->vkGetDeviceQueue(_vkDevice, _presentationQueueFamilyIndex, 0, &_presentQueue);

	// get graphics queue
	VulkanApi::GetApi()->vkGetDeviceQueue(_vkDevice, _graphicsQueueFamilyIndex, 0, &_graphicsQueue);

	// Create presentation command pool
	VkCommandPoolCreateInfo CreateCommandPoolInfo = {};
	CreateCommandPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	CreateCommandPoolInfo.pNext = nullptr;
	CreateCommandPoolInfo.flags = 0;
	CreateCommandPoolInfo.queueFamilyIndex = _presentationQueueFamilyIndex;
	result = VulkanApi::GetApi()->vkCreateCommandPool(_vkDevice, &CreateCommandPoolInfo, nullptr, &_presentQueueCommandPool);
	if (result != VK_SUCCESS)
	{
		throw BackendException("Failed to create vulkan present command queue");
	}

	// Create graphics command pool
	VkCommandPoolCreateInfo vkPoolCreateInfo = {};
	vkPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	vkPoolCreateInfo.pNext = nullptr;
	vkPoolCreateInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
	vkPoolCreateInfo.queueFamilyIndex = _graphicsQueueFamilyIndex;

	VulkanApi::GetApi()->vkCreateCommandPool(_vkDevice, &vkPoolCreateInfo, nullptr, &_graphicsQueueCommandPool);
}

VulkanRenderDevice::~VulkanRenderDevice()
{
	if (_vkDevice)
	{
		// Shutdown savely. Wait that the device is idle
		VulkanApi::GetApi()->vkDeviceWaitIdle(_vkDevice);
	}

    if (_presentRenderPass)
	{
        DeallocateDelete(*_pInstance->GetEngineAllocator(), *_presentRenderPass);
	}

	if (!_presentationFramebuffers.Empty())
	{
		for (size_t i = 0; i < _presentationFramebuffers.Size(); i++)
			VulkanApi::GetApi()->vkDestroyFramebuffer(_vkDevice, _presentationFramebuffers[i], nullptr);
	}
	_presentationFramebuffers.Clear();

	if (_presentCommandBufferArray)
	{
		const uint32_t imageCount = _pSwapChain->GetSwapChainImageCount();
		if (_presentCommandBufferArray[0] != nullptr)
			VulkanApi::GetApi()->vkFreeCommandBuffers(_vkDevice, _presentQueueCommandPool, imageCount, &_presentCommandBufferArray[0]);

		DeallocateArray<VkCommandBuffer>(*_pInstance->GetEngineAllocator(), _presentCommandBufferArray);
	}

	if (_presentQueueCommandPool)
		VulkanApi::GetApi()->vkDestroyCommandPool(_vkDevice, _presentQueueCommandPool, nullptr);

	if (_graphicsQueueCommandPool)
		VulkanApi::GetApi()->vkDestroyCommandPool(_vkDevice, _graphicsQueueCommandPool, nullptr);

	if (_pSwapChain)
	{
		DeallocateDelete(*_pInstance->GetEngineAllocator(), *_pSwapChain);
	}

	if (_presentationSurface)
		VulkanApi::GetApi()->vkDestroySurfaceKHR(_pInstance->GetInstanceHandle(), _presentationSurface, nullptr);

	if (_pMemoryManager)
	{
		DeallocateDelete(*_pInstance->GetEngineAllocator(), *_pMemoryManager);
	}

	if (_vkDevice)
	{
		VulkanApi::GetApi()->vkDestroyDevice(_vkDevice, nullptr);
		_vkDevice = nullptr;
	}
}

void VulkanRenderDevice::GetPhysicalDeviceMemoryProperties(VkPhysicalDeviceMemoryProperties& deviceMemProperties)
{
	if (!_pPhysicalDevice)
		throw BackendException("Vulkan physical device not properly setup");

	VulkanApi::GetApi()->vkGetPhysicalDeviceMemoryProperties(_pPhysicalDevice->GetPhysicalDeviceHandle(), &deviceMemProperties);
}

VkFormat VulkanRenderDevice::FindMatchingImageFormat(caveVector<VkFormat>& formats, VkImageTiling tiling, VkFormatFeatureFlags features)
{
	for (size_t i = 0; i < formats.Size(); i++)
	{
		VkFormatProperties formatProperties;
		VulkanApi::GetApi()->vkGetPhysicalDeviceFormatProperties(_pPhysicalDevice->GetPhysicalDeviceHandle(), formats[i], &formatProperties);

		if (tiling == VK_IMAGE_TILING_LINEAR && (formatProperties.linearTilingFeatures & features) == features) {
			return formats[i];
		}
		else if (tiling == VK_IMAGE_TILING_OPTIMAL && (formatProperties.optimalTilingFeatures & features) == features) {
			return formats[i];
		}
	}

	// no matching format found
	return VK_FORMAT_UNDEFINED;
}

void VulkanRenderDevice::GetApiVersion(uint32_t& major, uint32_t& minor, uint32_t& patch)
{
	if (!_pPhysicalDevice)
		throw BackendException("Vulkan physical device not properly setup");

	_pPhysicalDevice->GetApiVersion(major, minor, patch);
}

void VulkanRenderDevice::GetDriverVersion(uint32_t& major, uint32_t& minor, uint32_t& patch)
{
	if (!_pPhysicalDevice)
		throw BackendException("Vulkan physical device not properly setup");

	_pPhysicalDevice->GetDriverVersion(major, minor, patch);
}

const char* VulkanRenderDevice::GetDeviceName()
{
	if (!_pPhysicalDevice)
		throw BackendException("Vulkan physical device not properly setup");

	return _pPhysicalDevice->GetDeviceName();
}

void VulkanRenderDevice::WaitIdle()
{
	if (_vkDevice)
		VulkanApi::GetApi()->vkDeviceWaitIdle(_vkDevice);
}

void VulkanRenderDevice::CreateSwapChain(SwapChainInfo& swapChainInfo)
{
	if (!_pPhysicalDevice || !_vkDevice)
		throw BackendException("Vulkan device not properly setup");

	if (!_presentationSurface)
		throw BackendException("No presentation surface created");

	// we support only one swap chain per render device
	if (_pSwapChain)
		return;

	_pSwapChain = AllocateObject<VulkanSwapChain>(*_pInstance->GetEngineAllocator(), _pInstance, _pPhysicalDevice, this, swapChainInfo);

	// allocate command buffer
	if (_pSwapChain && _presentQueueCommandPool)
	{
		const uint32_t imageCount = _pSwapChain->GetSwapChainImageCount();
		_presentCommandBufferArray = AllocateArray<VkCommandBuffer>(*_pInstance->GetEngineAllocator(), imageCount);
		if (!_presentCommandBufferArray)
			throw BackendException("Failed to allocate presentation command buffers memory");

		VkCommandBufferAllocateInfo commandBufferAllocInfo = {};
		commandBufferAllocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		commandBufferAllocInfo.pNext = nullptr;
		commandBufferAllocInfo.commandPool = _presentQueueCommandPool;
		commandBufferAllocInfo.commandBufferCount = imageCount;
		commandBufferAllocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;

		VkResult result = VulkanApi::GetApi()->vkAllocateCommandBuffers(_vkDevice, &commandBufferAllocInfo, &_presentCommandBufferArray[0]);
		if (result != VK_SUCCESS)
		{
			throw BackendException("Failed to allocate vulkan presentation command buffers");
		}

		// If all went well set framebuffer size
		_presentationFramebuffers.Resize(imageCount);
		// init with null
		for (uint32_t i = 0; i < imageCount; i++)
		{
			_presentationFramebuffers[i] = VK_NULL_HANDLE;
		}
	}
}

void VulkanRenderDevice::CreateSwapChainFramebuffers()
{
	if (_presentationFramebuffers.Size() != _pSwapChain->GetSwapChainImageCount())
	{
		throw BackendException("Error: presentation framebuffers not equal swap chain images");
	}

    const VkImageView depthImageView = _pSwapChain->GetSwapChainDepthImageView();
    // we have always a color attachment
    uint32_t attachmentCount = 1;
    if (depthImageView != VK_NULL_HANDLE)
        attachmentCount++;

    // create render pass first
    HalRenderPassAttachment renderAttachments[2];
    renderAttachments[0]._format = GetSwapChainImageFormat();
    renderAttachments[0]._samples = HalSampleCount::SampleCount1;
    renderAttachments[0]._loadOp = HalAttachmentLoadOperation::Clear;
    renderAttachments[0]._storeOp = HalAttachmentStoreOperation::Store;
    renderAttachments[0]._loadStencilOp = HalAttachmentLoadOperation::DontCare;
    renderAttachments[0]._storeStencilOp = HalAttachmentStoreOperation::DontCare;
    renderAttachments[0]._initialLayout = HalImageLayout::Undefined;
    renderAttachments[0]._finalLayout = HalImageLayout::PresentSrcKHR;

    HalAttachmentReference colorAttachRef;
    colorAttachRef._attachment = 0;
    colorAttachRef._layout = HalImageLayout::ColorAttachment;

    // depth attachment
    renderAttachments[1]._format = GetSwapChainDepthImageFormat();
    renderAttachments[1]._samples = HalSampleCount::SampleCount1;
    renderAttachments[1]._loadOp = HalAttachmentLoadOperation::Clear;
    renderAttachments[1]._storeOp = HalAttachmentStoreOperation::DontCare;
    renderAttachments[1]._loadStencilOp = HalAttachmentLoadOperation::DontCare;
    renderAttachments[1]._storeStencilOp = HalAttachmentStoreOperation::DontCare;
    renderAttachments[1]._initialLayout = HalImageLayout::Undefined;
    renderAttachments[1]._finalLayout = HalImageLayout::DepthStencilAttachment;

    HalAttachmentReference depthAttachRef;
    depthAttachRef._attachment = 1;
    depthAttachRef._layout = HalImageLayout::DepthStencilAttachment;

    HalSubpassDescription subpassDesc;
    subpassDesc._pipelineBindPoint = HalPipelineBindPoints::Graphics;
    subpassDesc._colorAttachmentCount = 1;
    subpassDesc._pColorAttachments = &colorAttachRef;
    if (depthImageView != VK_NULL_HANDLE)
        subpassDesc._pDepthStencilAttachment = &depthAttachRef;
    else
        subpassDesc._pDepthStencilAttachment = nullptr;

    HalSubpassDependency dependency;
    dependency._srcSubpass = HAL_SUBPASS_EXTERNAL;	// special subpass
    dependency._dstSubpass = 0;	// our subpass
    dependency._srcStageMask = static_cast<HalPipelineStageFlags>(HalPipelineStageBits::ColorAttachmentOutput);
    dependency._srcAccessMask = HalAccessBits::AccessNone;
    dependency._dstStageMask = static_cast<HalPipelineStageFlags>(HalPipelineStageBits::ColorAttachmentOutput);
    dependency._dstAccessMask = HalAccessBits::ColorAttachmentRead | HalAccessBits::ColorAttachmentWrite;
    dependency._dependencyFlags = static_cast<HalDependencyFlags>(HalDependencyBits::DependencyNone);

    HalRenderPassInfo renderPassInfo;
    renderPassInfo._attachmentCount = attachmentCount;
    renderPassInfo._pAttachments = renderAttachments;
    renderPassInfo._subpassCount = 1;
    renderPassInfo._pSubpasses = &subpassDesc;
    renderPassInfo._dependencyCount = 1;
    renderPassInfo._pDependencies = &dependency;
    _presentRenderPass = AllocateObject<VulkanRenderPass>(*_pInstance->GetEngineAllocator(), this, renderPassInfo);

	VkExtent2D swapChainExtend = _pSwapChain->GetSwapChainImageExtend();

	// We have a max of 2 attachments
	VkImageView attachments[2];

	for (size_t i = 0; i < _presentationFramebuffers.Size(); i++)
	{
		// do not create twice
		if (_presentationFramebuffers[i] != VK_NULL_HANDLE)
			continue;

		attachments[0] = _pSwapChain->GetSwapChainImageView(i);
		if (attachmentCount > 1)
			attachments[1] = _pSwapChain->GetSwapChainDepthImageView();

		VkFramebufferCreateInfo framebufferInfo;
		framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferInfo.pNext = nullptr;
		framebufferInfo.flags = 0;
		framebufferInfo.renderPass = _presentRenderPass->GetRenderPass();
		framebufferInfo.attachmentCount = attachmentCount;
		framebufferInfo.pAttachments = attachments;
		framebufferInfo.layers = 1;
		framebufferInfo.width = swapChainExtend.width;
		framebufferInfo.height = swapChainExtend.height;

		if (VulkanApi::GetApi()->vkCreateFramebuffer(_vkDevice, &framebufferInfo, nullptr, &_presentationFramebuffers[i]) != VK_SUCCESS)
		{
			throw BackendException("Error: Failed to create presentation framebuffer");
		}
	}
}


const HalImageFormat VulkanRenderDevice::GetSwapChainImageFormat()
{
	HalImageFormat format = HalImageFormat::Undefined;
	if (_pSwapChain)
		format = VulkanTypeConversion::ConvertImageFormatFromVulkan(_pSwapChain->GetSwapChainImageFormat());

	return format;
}

const HalImageFormat VulkanRenderDevice::GetSwapChainDepthImageFormat()
{
	HalImageFormat format = HalImageFormat::Undefined;
	if (_pSwapChain)
		format = VulkanTypeConversion::ConvertImageFormatFromVulkan(_pSwapChain->GetSwapChainDepthImageFormat());

	return format;
}

const uint32_t VulkanRenderDevice::GetSwapChainImageCount()
{
	if (_pSwapChain)
		return _pSwapChain->GetSwapChainImageCount();

	return 0;
}

const HalExtent2D VulkanRenderDevice::GetSwapChainExtend()
{
	HalExtent2D halExtend;
	halExtend._height = 0;
	halExtend._width = 0;
	if (_pSwapChain)
	{
		VkExtent2D extend = _pSwapChain->GetSwapChainImageExtend();
		halExtend._height = extend.height;
		halExtend._width = extend.width;
	}

	return halExtend;
}

const uint32_t VulkanRenderDevice::AcquireNextSwapChainImage(uint64_t timeout)
{
	if (!_pSwapChain)
		return (std::numeric_limits<uint32_t>::max)();

	return _pSwapChain->AcquireNextSwapChainImage(timeout);
}

HalCommandPool* VulkanRenderDevice::CreateCommandPool(HalCommandPoolInfo& commandPoolInfo)
{
	if (!_pPhysicalDevice || !_vkDevice)
		return nullptr;
	
	VulkanCommandPool* commandPool = AllocateObject<VulkanCommandPool>(*_pInstance->GetEngineAllocator(), this, commandPoolInfo);

	return commandPool;
}

HalDescriptorPool* VulkanRenderDevice::CreateDescriptorPool(HalDescriptorPoolInfo& descriptorPoolInfo)
{
	if (!_pPhysicalDevice || !_vkDevice)
		return nullptr;

	VulkanDescriptorPool* descriptorPool = AllocateObject<VulkanDescriptorPool>(*_pInstance->GetEngineAllocator(), this, descriptorPoolInfo);

	return descriptorPool;
}

HalShader* VulkanRenderDevice::CreateShader(ShaderType type, ShaderLanguage language)
{
	if (!_pPhysicalDevice || !_vkDevice)
		return nullptr;

	VulkanShader* shader = AllocateObject<VulkanShader>(*_pInstance->GetEngineAllocator(), this, type, language);

	return shader;
}

HalVertexInput* VulkanRenderDevice::CreateVertexInput(HalVertexInputStateInfo& vertexInputState)
{
	if (!_pPhysicalDevice || !_vkDevice)
		return nullptr;

	VulkanVertexInput* vertexInput = AllocateObject<VulkanVertexInput>(*_pInstance->GetEngineAllocator(), this, vertexInputState);

	return vertexInput;
}

HalInputAssembly* VulkanRenderDevice::CreateInputAssembly(HalInputAssemblyInfo& inputAssemblyState)
{
	if (!_pPhysicalDevice || !_vkDevice)
		return nullptr;

	VulkanInputAssembly* inputAssembly = AllocateObject<VulkanInputAssembly>(*_pInstance->GetEngineAllocator(), this, inputAssemblyState);

	return inputAssembly;
}

HalViewportAndScissor* VulkanRenderDevice::CreateViewportAndScissor(HalViewport& viewport, HalScissor& scissor)
{
	if (!_pPhysicalDevice || !_vkDevice)
		return nullptr;

	VulkanViewportAndScissor* viewportAndScissor = AllocateObject<VulkanViewportAndScissor>(*_pInstance->GetEngineAllocator(), this, viewport, scissor);

	return viewportAndScissor;
}

HalRasterizerState* VulkanRenderDevice::CreateRasterizerState(HalRasterizerSetup& rasterizerStateInfo)
{
	if (!_pPhysicalDevice || !_vkDevice)
		return nullptr;

	VulkanRasterizerState* rasterizerState = AllocateObject<VulkanRasterizerState>(*_pInstance->GetEngineAllocator(), this, rasterizerStateInfo);

	return rasterizerState;
}

HalMultisample* VulkanRenderDevice::CreateMultisampleState(HalMultisampleState& multisampleStateInfo)
{
	if (!_pPhysicalDevice || !_vkDevice)
		return nullptr;

	VulkanMultisample* multisampleState = AllocateObject<VulkanMultisample>(*_pInstance->GetEngineAllocator(), this, multisampleStateInfo);

	return multisampleState;
}

HalDepthStencil* VulkanRenderDevice::CreateDepthStencilState(HalDepthStencilSetup& depthStencilInfo)
{
	if (!_pPhysicalDevice || !_vkDevice)
		return nullptr;

	VulkanDepthStencil* depthStencilState = AllocateObject<VulkanDepthStencil>(*_pInstance->GetEngineAllocator(), this, depthStencilInfo);

	return depthStencilState;
}

HalColorBlend* VulkanRenderDevice::CreateColorBlendState(HalColorBlendState& colorBlendInfo
	, caveVector<HalColorBlendAttachment>& blendAttachments)
{
	if (!_pPhysicalDevice || !_vkDevice)
		return nullptr;

	VulkanColorBlend* colorBlendState = AllocateObject<VulkanColorBlend>(*_pInstance->GetEngineAllocator(), this, colorBlendInfo, blendAttachments);

	return colorBlendState;
}

HalDynamicState* VulkanRenderDevice::CreateDynamicState(caveVector<HalDynamicStates>& dynamicStates)
{
	if (!_pPhysicalDevice || !_vkDevice)
		return nullptr;

	VulkanDynamicState* dynamicState = AllocateObject<VulkanDynamicState>(*_pInstance->GetEngineAllocator(), this, dynamicStates);

	return dynamicState;
}

HalPipelineLayout* VulkanRenderDevice::CreatePipelineLayout(HalDescriptorSet* descriptorSet, caveVector<HalPushConstantRange>& pushConstants)
{
	if (!_pPhysicalDevice || !_vkDevice)
		return nullptr;

	VulkanPipelineLayout* pipelineLayout = AllocateObject<VulkanPipelineLayout>(*_pInstance->GetEngineAllocator(), this, descriptorSet, pushConstants);

	return pipelineLayout;
}

HalDescriptorSet* VulkanRenderDevice::CreateDescriptorSetLayouts(caveVector<HalDescriptorSetLayout>& descriptorSetLayouts)
{
	if (!_pPhysicalDevice || !_vkDevice)
		return nullptr;

	VulkanDescriptorSet* descriptorSet = AllocateObject<VulkanDescriptorSet>(*_pInstance->GetEngineAllocator(), this, descriptorSetLayouts);

	return descriptorSet;
}

static void UpdateDescriptorSetBufferInfo(HalDescriptorBufferInfo** inBufferInfos, VkDescriptorBufferInfo** outBufferInfos, uint32_t count)
{
	for (size_t i = 0; i < count; ++i)
	{
		VulkanBuffer* buffer = static_cast<VulkanBuffer*>(inBufferInfos[i]->_buffer);
		outBufferInfos[i]->buffer = buffer->GetBuffer();
		outBufferInfos[i]->offset = inBufferInfos[i]->_offset;
		outBufferInfos[i]->range = inBufferInfos[i]->_range;
	}
}

static void UpdateDescriptorSetImageInfo(HalDescriptorImageInfo** inImageInfos, VkDescriptorImageInfo** outImageInfos, uint32_t count)
{
    for (size_t i = 0; i < count; ++i)
    {
        VulkanImageView* imageView = static_cast<VulkanImageView*>(inImageInfos[i]->_imageView);
        VulkanSampler* imageSampler = static_cast<VulkanSampler*>(inImageInfos[i]->_imageSampler);
        outImageInfos[i]->imageView = imageView->GetImageView();
        outImageInfos[i]->sampler = imageSampler->GetSampler();
        outImageInfos[i]->imageLayout = VulkanTypeConversion::ConvertImageLayoutToVulkan(inImageInfos[i]->_imageLayout);
        
    }
}

void VulkanRenderDevice::UpdateDescriptorSets(caveVector<HalWriteDescriptorSet>& descriptorWrites)
{
	uint32_t descriptorWritesCount = static_cast<uint32_t>(descriptorWrites.Size());
	size_t descriptorWriteSize = descriptorWrites.Size() * sizeof(VkWriteDescriptorSet);
	VkWriteDescriptorSet *vkWriteDescriptorSets = static_cast<VkWriteDescriptorSet*>(GetEngineAllocator()->Allocate(descriptorWriteSize, 4));

	if (vkWriteDescriptorSets)
	{
		for (size_t i = 0; i < descriptorWrites.Size(); ++i)
		{
			VulkanDescriptorSet* descriptorSet = static_cast<VulkanDescriptorSet*>(descriptorWrites[i]._dstSet);
			// Set general values
			vkWriteDescriptorSets[i].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			vkWriteDescriptorSets[i].pNext = nullptr;
			vkWriteDescriptorSets[i].dstSet = descriptorSet->GetDescriptorSet();
			vkWriteDescriptorSets[i].dstBinding = descriptorWrites[i]._dstBinding;
			vkWriteDescriptorSets[i].descriptorType = VulkanTypeConversion::ConvertDescriptorTypeToVulkan(descriptorWrites[i]._descriptorType);
			vkWriteDescriptorSets[i].dstArrayElement = descriptorWrites[i]._dstArrayElement;
			vkWriteDescriptorSets[i].descriptorCount = descriptorWrites[i]._descriptorCount;
			vkWriteDescriptorSets[i].pBufferInfo = nullptr;
			vkWriteDescriptorSets[i].pImageInfo = nullptr;
			vkWriteDescriptorSets[i].pTexelBufferView = nullptr;

			// update depending on the type
			if (descriptorWrites[i]._pBufferInfo)
			{
				// this descriptor write contains buffer updates
				assert(descriptorWrites[i]._descriptorType == HalDescriptorType::UniformBuffer ||
					descriptorWrites[i]._descriptorType == HalDescriptorType::StorageBuffer ||
					descriptorWrites[i]._descriptorType == HalDescriptorType::UniformBufferDynamic ||
					descriptorWrites[i]._descriptorType == HalDescriptorType::StorageBufferDynamic);

				size_t descriptorInfoSize = descriptorWrites[i]._descriptorCount * sizeof(VkDescriptorBufferInfo);
				VkDescriptorBufferInfo  *descriptorBufferInfos = static_cast<VkDescriptorBufferInfo *>(GetEngineAllocator()->Allocate(descriptorInfoSize, 4));

				if (descriptorBufferInfos)
					UpdateDescriptorSetBufferInfo(&descriptorWrites[i]._pBufferInfo, &descriptorBufferInfos, descriptorWrites[i]._descriptorCount);

				vkWriteDescriptorSets[i].pBufferInfo = descriptorBufferInfos;
			}
            if (descriptorWrites[i]._pImageInfo)
            {
                // this descriptor write contains image updates
                assert(descriptorWrites[i]._descriptorType == HalDescriptorType::SampledImage ||
                    descriptorWrites[i]._descriptorType == HalDescriptorType::Sampler ||
                    descriptorWrites[i]._descriptorType == HalDescriptorType::StorageImage ||
                    descriptorWrites[i]._descriptorType == HalDescriptorType::CombinedImageSampler);

                size_t descriptorInfoSize = descriptorWrites[i]._descriptorCount * sizeof(VkDescriptorImageInfo);
                VkDescriptorImageInfo  *descriptorImageInfos = static_cast<VkDescriptorImageInfo *>(GetEngineAllocator()->Allocate(descriptorInfoSize, 4));
                if (descriptorImageInfos)
                    UpdateDescriptorSetImageInfo(&descriptorWrites[i]._pImageInfo, &descriptorImageInfos, descriptorWrites[i]._descriptorCount);

                vkWriteDescriptorSets[i].pImageInfo = descriptorImageInfos;
            }
		}

		VulkanApi::GetApi()->vkUpdateDescriptorSets(_vkDevice, descriptorWritesCount, vkWriteDescriptorSets, 0, nullptr);
	}

	// Relase memory
	if (vkWriteDescriptorSets)
	{
		for (uint32_t i = 0; i < descriptorWritesCount; ++i)
		{
			if (vkWriteDescriptorSets[i].pBufferInfo)
			{
				GetEngineAllocator()->Deallocate((void *)vkWriteDescriptorSets[i].pBufferInfo);
			}
            if (vkWriteDescriptorSets[i].pImageInfo)
            {
                GetEngineAllocator()->Deallocate((void *)vkWriteDescriptorSets[i].pImageInfo);
            }
		}

		GetEngineAllocator()->Deallocate((void *)vkWriteDescriptorSets);
	}
}

HalRenderPass* VulkanRenderDevice::CreateRenderPass(HalRenderPassInfo& renderPassInfo)
{
	if (!_pPhysicalDevice || !_vkDevice)
		return nullptr;

	VulkanRenderPass* renderPass = AllocateObject<VulkanRenderPass>(*_pInstance->GetEngineAllocator(), this, renderPassInfo);

	return renderPass;
}

HalGraphicsPipeline* VulkanRenderDevice::CreateGraphicsPipeline(HalGraphicsPipelineInfo& graphicsPipelineInfo)
{
	if (!_pPhysicalDevice || !_vkDevice)
		return nullptr;

	VulkanGraphicsPipeline* graphicsPipeline = AllocateObject<VulkanGraphicsPipeline>(*_pInstance->GetEngineAllocator(), this, graphicsPipelineInfo);

	return graphicsPipeline;
}

HalFrameBuffer* VulkanRenderDevice::CreateFrameBuffer(HalRenderPass* renderPass,
    uint32_t width, uint32_t height, uint32_t layers,
    caveVector<HalImageView*>& renderAttachments)
{
    if (!_pPhysicalDevice || !_vkDevice)
        return nullptr;

    VulkanRenderPass* vkRenderPass = static_cast<VulkanRenderPass*>(renderPass);
    // setup vulkan image view array
    caveVector<VkImageView> vkImageViews(_pInstance->GetEngineAllocator());
    vkImageViews.Resize(renderAttachments.Size());
    for (size_t i = 0; i < vkImageViews.Size(); i++)
    {
        VulkanImageView* vkImageView = static_cast<VulkanImageView*>(renderAttachments[i]);
        vkImageViews[i] = vkImageView->GetImageView();
    }

    VulkanFrameBuffer* framebuffer = AllocateObject<VulkanFrameBuffer>(*_pInstance->GetEngineAllocator(), this, 
        vkRenderPass->GetRenderPass(), width, height, layers, vkImageViews);

    return framebuffer;
}

HalSemaphore* VulkanRenderDevice::CreateSemaphore(HalSemaphoreDesc& semaphoreDesc)
{
	if (!_pPhysicalDevice || !_vkDevice)
		return nullptr;

	VulkanSemaphore* semaphore = AllocateObject<VulkanSemaphore>(*_pInstance->GetEngineAllocator(), this, semaphoreDesc);

	return semaphore;
}

HalFence* VulkanRenderDevice::CreateFence(HalFenceDesc& fenceDesc)
{
    if (!_pPhysicalDevice || !_vkDevice)
        return nullptr;

    VulkanFence* fence = AllocateObject<VulkanFence>(*_pInstance->GetEngineAllocator(), this, fenceDesc);

    return fence;
}

HalBuffer* VulkanRenderDevice::CreateBuffer(HalBufferInfo& bufferInfo)
{
	if (!_pPhysicalDevice || !_vkDevice)
		return nullptr;

	VulkanBuffer *buffer = AllocateObject<VulkanBuffer>(*_pInstance->GetEngineAllocator(), this, bufferInfo);

	return buffer;
}

HalImage* VulkanRenderDevice::CreateImage(HalImageInfo& imageInfo)
{
	if (!_pPhysicalDevice || !_vkDevice)
		return nullptr;

	VulkanImage *image = AllocateObject<VulkanImage>(*_pInstance->GetEngineAllocator(), this, imageInfo);

	return image;
}

HalImageView* VulkanRenderDevice::CreateImageView(HalImage* image, HalImageViewInfo& viewInfo)
{
    if (!_pPhysicalDevice || !_vkDevice)
        return nullptr;

    VulkanImageView *imageView = AllocateObject<VulkanImageView>(*_pInstance->GetEngineAllocator(), this, image, viewInfo);

    return imageView;
}

HalSampler* VulkanRenderDevice::CreateSampler(HalSamplerCreateInfo& samplerInfo)
{
    if (!_pPhysicalDevice || !_vkDevice)
        return nullptr;

    VulkanSampler *sampler = AllocateObject<VulkanSampler>(*_pInstance->GetEngineAllocator(), this, samplerInfo);

    return sampler;
}

bool VulkanRenderDevice::AllocateCommandBuffers(HalCommandPool* commandPool
						, HalCommandBufferInfo& commandBufferInfo
						, caveVector<HalCommandBuffer*>& commandBuffers)
{
	if (!_pPhysicalDevice || !_vkDevice || !commandPool)
		return false;

	assert(commandBufferInfo._bufferCount == static_cast<uint32_t>(commandBuffers.Size()));

	// tmp buffer
	caveVector<VkCommandBuffer> vkCommandBuffers(_pInstance->GetEngineAllocator());
	vkCommandBuffers.Resize(commandBufferInfo._bufferCount);

	VulkanCommandPool* vkPool = static_cast<VulkanCommandPool*>(commandPool);
	VkCommandBufferAllocateInfo allocInfo;
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.pNext = nullptr;
	allocInfo.commandPool = vkPool->GetCommandPool();
	allocInfo.commandBufferCount = commandBufferInfo._bufferCount;
	allocInfo.level = VulkanTypeConversion::ConvertCommandBufferLevelToVulkan(commandBufferInfo._level);

	if (VulkanApi::GetApi()->vkAllocateCommandBuffers(_vkDevice, &allocInfo, vkCommandBuffers.Data()) != VK_SUCCESS)
	{
		assert(false);
		return false;
	}

	for (size_t i = 0; i < commandBuffers.Size(); ++i)
	{
		commandBuffers[i] = AllocateObject<VulkanCommandBuffer>(*_pInstance->GetEngineAllocator(), this, vkCommandBuffers[i]);
	}

	return true;
}

void VulkanRenderDevice::BeginCommandBuffer(HalCommandBuffer* commandBuffer, HalCommandBufferBeginInfo& commandBufferBeginInfo)
{
	if (!_pPhysicalDevice || !_vkDevice)
		return;

	VkCommandBufferBeginInfo beginInfo;
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.pNext = nullptr;
	beginInfo.flags = VulkanTypeConversion::ConvertCommandBufferUsageFlagsToVulkan(commandBufferBeginInfo._flags);
	beginInfo.pInheritanceInfo = nullptr;

	VulkanApi::GetApi()->vkBeginCommandBuffer(static_cast<VulkanCommandBuffer*>(commandBuffer)->GetCommandBuffer(), &beginInfo);
}

void VulkanRenderDevice::EndCommandBuffer(HalCommandBuffer* commandBuffer)
{
	if (!_pPhysicalDevice || !_vkDevice)
		return;

	VulkanApi::GetApi()->vkEndCommandBuffer(static_cast<VulkanCommandBuffer*>(commandBuffer)->GetCommandBuffer());
}

void VulkanRenderDevice::CmdBeginRenderPass(HalCommandBuffer* commandBuffer, HalCmdRenderPassInfo& renderPassBeginInfo, HalSubpassContents subpass)
{
	VkRenderPassBeginInfo vkRenderPassInfo;
	vkRenderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	vkRenderPassInfo.pNext = nullptr;
	VulkanRenderPass* vulkanRenderPass = static_cast<VulkanRenderPass*>(renderPassBeginInfo._renderPass);
	vkRenderPassInfo.renderPass = vulkanRenderPass->GetRenderPass();
    
    if (renderPassBeginInfo._framebuffer != nullptr)
    {
        VulkanFrameBuffer* vulkanFrameBuffer = static_cast<VulkanFrameBuffer*>(renderPassBeginInfo._framebuffer);
        vkRenderPassInfo.framebuffer = vulkanFrameBuffer->GetFrameBuffer();
    }
    else
    {
        vkRenderPassInfo.framebuffer = _presentationFramebuffers[renderPassBeginInfo._swapChainIndex];
    }

	vkRenderPassInfo.renderArea.offset.x = renderPassBeginInfo._renderRect._x;
	vkRenderPassInfo.renderArea.offset.y = renderPassBeginInfo._renderRect._y;
	vkRenderPassInfo.renderArea.extent.height = renderPassBeginInfo._renderRect._height;
	vkRenderPassInfo.renderArea.extent.width = renderPassBeginInfo._renderRect._width;
	vkRenderPassInfo.clearValueCount = renderPassBeginInfo._clearValueCount;
	// copy clear values
	caveVector<VkClearValue> vkClearValueArray(_pInstance->GetEngineAllocator());
	vkClearValueArray.Resize(renderPassBeginInfo._clearValueCount);
	for (size_t i = 0; i < renderPassBeginInfo._clearValueCount; ++i)
	{
		vkClearValueArray[i].color.float32[0] = renderPassBeginInfo._clearValues[i]._color.float32[0];
		vkClearValueArray[i].color.float32[1] = renderPassBeginInfo._clearValues[i]._color.float32[1];
		vkClearValueArray[i].color.float32[2] = renderPassBeginInfo._clearValues[i]._color.float32[2];
		vkClearValueArray[i].color.float32[3] = renderPassBeginInfo._clearValues[i]._color.float32[3];

		vkClearValueArray[i].depthStencil.depth = renderPassBeginInfo._clearValues[i]._depthStencil._depth;
		vkClearValueArray[i].depthStencil.stencil = renderPassBeginInfo._clearValues[i]._depthStencil._stencil;
	}
	vkRenderPassInfo.pClearValues = vkClearValueArray.Data();
	VkSubpassContents content = VulkanTypeConversion::ConvertSubpassContentToVulkan(subpass);
	VulkanApi::GetApi()->vkCmdBeginRenderPass(static_cast<VulkanCommandBuffer*>(commandBuffer)->GetCommandBuffer(), &vkRenderPassInfo, content);
}

void VulkanRenderDevice::CmdEndRenderPass(HalCommandBuffer* commandBuffer)
{
	if (!_pPhysicalDevice || !_vkDevice)
		return;

	VulkanApi::GetApi()->vkCmdEndRenderPass(static_cast<VulkanCommandBuffer*>(commandBuffer)->GetCommandBuffer());
}

void VulkanRenderDevice::CmdTransitionResource(HalCommandBuffer* commandBuffer, 
    HalPipelineStageFlags srcStageMask, HalPipelineStageFlags dstStageMask,
    HalTransitionBarrierDesc& transitionBarrierDes)
{
    if (!_pPhysicalDevice || !_vkDevice)
        return;

    VkPipelineStageFlags vkSrcStageMask = VulkanTypeConversion::ConvertPipelineFlagsToVulkan(srcStageMask);
    VkPipelineStageFlags vkDstStageMask = VulkanTypeConversion::ConvertPipelineFlagsToVulkan(dstStageMask);

    // memory barrier
    caveVector<VkMemoryBarrier> vkMemoryBarriers(_pInstance->GetEngineAllocator());
    for (size_t i = 0; i < transitionBarrierDes._memoryBarrierCount; i++)
    {
        VkMemoryBarrier vkMemoryBarrier = { VK_STRUCTURE_TYPE_MEMORY_BARRIER };
        vkMemoryBarrier.srcAccessMask = VulkanTypeConversion::ConvertAccessFlagsToVulkan(transitionBarrierDes._pMemoryBarriers[i]._srcAccessMask);
        vkMemoryBarrier.dstAccessMask = VulkanTypeConversion::ConvertAccessFlagsToVulkan(transitionBarrierDes._pMemoryBarriers[i]._dstAccessMask);

        vkMemoryBarriers.Push(vkMemoryBarrier);
    }

    // buffer barrier
    caveVector<VkBufferMemoryBarrier > vkBufferMemoryBarriers(_pInstance->GetEngineAllocator());
    for (size_t i = 0; i < transitionBarrierDes._bufferMemoryBarrierCount; i++)
    {
        VulkanBuffer* vkBuffer = static_cast<VulkanBuffer*>(transitionBarrierDes._pBufferMemoryBarriers[i]._buffer);
        VkBufferMemoryBarrier vkBufferMemoryBarrier = { VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER };
        vkBufferMemoryBarrier.srcAccessMask = VulkanTypeConversion::ConvertAccessFlagsToVulkan(transitionBarrierDes._pBufferMemoryBarriers[i]._srcAccessMask);
        vkBufferMemoryBarrier.dstAccessMask = VulkanTypeConversion::ConvertAccessFlagsToVulkan(transitionBarrierDes._pBufferMemoryBarriers[i]._dstAccessMask);
        vkBufferMemoryBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        vkBufferMemoryBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        vkBufferMemoryBarrier.buffer = (vkBuffer) ? vkBuffer->GetBuffer() : nullptr;
        vkBufferMemoryBarrier.offset = static_cast<VkDeviceSize>(transitionBarrierDes._pBufferMemoryBarriers[i]._offset);
        vkBufferMemoryBarrier.size = static_cast<VkDeviceSize>(transitionBarrierDes._pBufferMemoryBarriers[i]._size);

        vkBufferMemoryBarriers.Push(vkBufferMemoryBarrier);
    }

    // image barrier
    caveVector<VkImageMemoryBarrier> vkImageMemoryBarriers(_pInstance->GetEngineAllocator());
    for (size_t i = 0; i < transitionBarrierDes._imageMemoryBarrierCount; i++)
    {
        VulkanImage* vkImage = static_cast<VulkanImage*>(transitionBarrierDes._pImageMemoryBarriers[i]._image);
        VkImageMemoryBarrier vkImageMemoryBarrier = { VK_STRUCTURE_TYPE_MEMORY_BARRIER };
        vkImageMemoryBarrier.srcAccessMask = VulkanTypeConversion::ConvertAccessFlagsToVulkan(transitionBarrierDes._pImageMemoryBarriers[i]._srcAccessMask);
        vkImageMemoryBarrier.dstAccessMask = VulkanTypeConversion::ConvertAccessFlagsToVulkan(transitionBarrierDes._pImageMemoryBarriers[i]._dstAccessMask);
        vkImageMemoryBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        vkImageMemoryBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        vkImageMemoryBarrier.image = (vkImage) ? vkImage->GetImage() : nullptr;
        vkImageMemoryBarrier.oldLayout = VulkanTypeConversion::ConvertImageLayoutToVulkan(transitionBarrierDes._pImageMemoryBarriers[i]._oldLayout);
        vkImageMemoryBarrier.newLayout = VulkanTypeConversion::ConvertImageLayoutToVulkan(transitionBarrierDes._pImageMemoryBarriers[i]._newLayout);
        vkImageMemoryBarrier.subresourceRange.aspectMask = 
            VulkanTypeConversion::ConvertImageAspectFlagsToVulkan(transitionBarrierDes._pImageMemoryBarriers[i]._subresourceRange._aspectMask);
        vkImageMemoryBarrier.subresourceRange.baseArrayLayer = transitionBarrierDes._pImageMemoryBarriers[i]._subresourceRange._baseArrayLayer;
        vkImageMemoryBarrier.subresourceRange.baseMipLevel = transitionBarrierDes._pImageMemoryBarriers[i]._subresourceRange._baseMipLevel;
        vkImageMemoryBarrier.subresourceRange.layerCount = transitionBarrierDes._pImageMemoryBarriers[i]._subresourceRange._layerCount;
        vkImageMemoryBarrier.subresourceRange.levelCount = transitionBarrierDes._pImageMemoryBarriers[i]._subresourceRange._levelCount;

        vkImageMemoryBarriers.Push(vkImageMemoryBarrier);
    }

    VulkanApi::GetApi()->vkCmdPipelineBarrier(static_cast<VulkanCommandBuffer*>(commandBuffer)->GetCommandBuffer(),
        vkSrcStageMask, vkDstStageMask, 0,
        transitionBarrierDes._memoryBarrierCount, vkMemoryBarriers.Data(),
        transitionBarrierDes._bufferMemoryBarrierCount, vkBufferMemoryBarriers.Data(),
        transitionBarrierDes._imageMemoryBarrierCount, vkImageMemoryBarriers.Data());
}

void VulkanRenderDevice::CmdBindGraphicsPipeline(HalCommandBuffer* commandBuffer, HalGraphicsPipeline* graphicsPipelineInfo)
{
	if (!_pPhysicalDevice || !_vkDevice)
		return;

	VulkanApi::GetApi()->vkCmdBindPipeline(static_cast<VulkanCommandBuffer*>(commandBuffer)->GetCommandBuffer()
		, VK_PIPELINE_BIND_POINT_GRAPHICS
		, static_cast<VulkanGraphicsPipeline*>(graphicsPipelineInfo)->GetGraphicsPipeline());
}

void VulkanRenderDevice::CmdBindVertexBuffers(HalCommandBuffer* commandBuffer, uint32_t firstBinding, uint32_t bindingCount
	, HalBuffer** vertexBuffers, const uint64_t* offsetArray)
{
	if (!_pPhysicalDevice || !_vkDevice)
		return;

	// tmp buffer
	caveVector<VkBuffer> vkBuffers(_pInstance->GetEngineAllocator());
	vkBuffers.Resize(bindingCount);
	for (uint32_t i = 0; i < bindingCount; i++)
	{
		VulkanBuffer* buffer = static_cast<VulkanBuffer*>(vertexBuffers[i]);
		vkBuffers[i] = buffer->GetBuffer();
	}

	VulkanApi::GetApi()->vkCmdBindVertexBuffers(static_cast<VulkanCommandBuffer*>(commandBuffer)->GetCommandBuffer()
		, firstBinding, bindingCount, vkBuffers.Data(), offsetArray);
}

void VulkanRenderDevice::CmdBindDescriptorSets(HalCommandBuffer* commandBuffer, HalPipelineBindPoints pipelineBindPoint
	, HalPipelineLayout* layout, uint32_t firstSet, uint32_t descriptorSetCount
	, HalDescriptorSet** descriptorSets, uint32_t dynamicOffsetCount, const uint32_t* dynamicOffsets)
{
	if (!_pPhysicalDevice || !_vkDevice)
		return;

	// tmp buffer
	caveVector<VkDescriptorSet> vkDescriptorSets(_pInstance->GetEngineAllocator());
	vkDescriptorSets.Resize(descriptorSetCount);
	for (uint32_t i = 0; i < descriptorSetCount; i++)
	{
		VulkanDescriptorSet* descriptorSet = static_cast<VulkanDescriptorSet*>(descriptorSets[i]);
		vkDescriptorSets[i] = descriptorSet->GetDescriptorSet();
	}

	VulkanApi::GetApi()->vkCmdBindDescriptorSets(static_cast<VulkanCommandBuffer*>(commandBuffer)->GetCommandBuffer()
		, VulkanTypeConversion::ConvertPipelineBindPointToVulkan(pipelineBindPoint)
		, static_cast<VulkanPipelineLayout*>(layout)->GetPipelineLayout()
		, firstSet, descriptorSetCount, vkDescriptorSets.Data(), dynamicOffsetCount, dynamicOffsets);
}

void VulkanRenderDevice::CmdPushConstants(HalCommandBuffer* commandBuffer, HalPipelineLayout* layout
	, HalShaderStagesFlags shaderStagesFlags, uint32_t offset, uint32_t size, const void* pData)
{
	if (!_pPhysicalDevice || !_vkDevice)
		return;

	VulkanApi::GetApi()->vkCmdPushConstants(static_cast<VulkanCommandBuffer*>(commandBuffer)->GetCommandBuffer()
		, static_cast<VulkanPipelineLayout*>(layout)->GetPipelineLayout()
		, VulkanTypeConversion::ConvertShaderStagesToVulkan(shaderStagesFlags)
		, offset, size, pData);
}

void VulkanRenderDevice::CmdBindIndexBuffer(HalCommandBuffer* commandBuffer
	, HalBuffer* indexBuffer, const uint64_t offset, HalIndexType indexType)
{
	if (!_pPhysicalDevice || !_vkDevice)
		return;

	VulkanBuffer* buffer = static_cast<VulkanBuffer*>(indexBuffer);
	VulkanApi::GetApi()->vkCmdBindIndexBuffer(static_cast<VulkanCommandBuffer*>(commandBuffer)->GetCommandBuffer()
	 , buffer->GetBuffer(), offset, VulkanTypeConversion::ConvertIndexTypeToVulkan(indexType));
}

void VulkanRenderDevice::CmdDraw(HalCommandBuffer* commandBuffer, uint32_t vertexCount, uint32_t instanceCount
								, uint32_t firstVertex, uint32_t firstInstance)
{
	if (!_pPhysicalDevice || !_vkDevice)
		return;

	VulkanApi::GetApi()->vkCmdDraw(static_cast<VulkanCommandBuffer*>(commandBuffer)->GetCommandBuffer()
		, vertexCount, instanceCount, firstVertex, firstInstance);
}

void VulkanRenderDevice::CmdDrawIndexed(HalCommandBuffer* commandBuffer, uint32_t indexCount, uint32_t instanceCount
	, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance)
{
	if (!_pPhysicalDevice || !_vkDevice)
		return;

	VulkanApi::GetApi()->vkCmdDrawIndexed(static_cast<VulkanCommandBuffer*>(commandBuffer)->GetCommandBuffer()
		, indexCount, instanceCount, firstIndex, vertexOffset, firstInstance);
}

void VulkanRenderDevice::CmdCopyImage(HalCommandBuffer* commandBuffer, HalImage* srcImage, HalImageLayout srcLayout, 
    HalImage* dstImage, HalImageLayout dstLayout, uint32_t regionCount, HalImageCopy* regions)
{
    if (srcImage && dstImage)
    {
        VulkanImage* vkSrcImage = static_cast<VulkanImage*>(srcImage);
        VulkanImage* vkDstImage = static_cast<VulkanImage*>(dstImage);
        VulkanCommandBuffer* vkCmdBuffer = static_cast<VulkanCommandBuffer*>(commandBuffer);

        uint32_t maxSrcLevel = 0;
        uint32_t maxDstLevel = 0;

        // create VkImageCopy structs
        caveVector<VkImageCopy> vkCopyImageArray(_pInstance->GetEngineAllocator());
        vkCopyImageArray.Resize(regionCount);
        for (uint32_t i = 0; i < regionCount; i++)
        {
            VkImageCopy vkImageCopy = {};
            vkImageCopy.srcOffset = VulkanTypeConversion::ConvertOffset3DToVulkan(regions[i]._srcOffset);
            vkImageCopy.srcSubresource.aspectMask = VulkanTypeConversion::ConvertImageAspectFlagsToVulkan(regions[i]._srcLayers._aspectMask);
            vkImageCopy.srcSubresource.baseArrayLayer = regions[i]._srcLayers._baseArrayLayer;
            vkImageCopy.srcSubresource.layerCount = regions[i]._srcLayers._layerCount;
            vkImageCopy.srcSubresource.mipLevel = regions[i]._srcLayers._mipLevel;

            if (regions[i]._srcLayers._mipLevel >= maxSrcLevel)
                maxSrcLevel = regions[i]._srcLayers._mipLevel + 1;

            vkImageCopy.dstOffset = VulkanTypeConversion::ConvertOffset3DToVulkan(regions[i]._dstOffset);
            vkImageCopy.dstSubresource.aspectMask = VulkanTypeConversion::ConvertImageAspectFlagsToVulkan(regions[i]._dstLayers._aspectMask);
            vkImageCopy.dstSubresource.baseArrayLayer = regions[i]._dstLayers._baseArrayLayer;
            vkImageCopy.dstSubresource.layerCount = regions[i]._dstLayers._layerCount;
            vkImageCopy.dstSubresource.mipLevel = regions[i]._dstLayers._mipLevel;

            if (regions[i]._dstLayers._mipLevel >= maxDstLevel)
                maxDstLevel = regions[i]._dstLayers._mipLevel + 1;

            vkImageCopy.extent = VulkanTypeConversion::ConvertExtent3DToVulkan(regions[i]._extent);

            vkCopyImageArray.Push(vkImageCopy);
        }

        // transition source and dest image to be ready for the copy
        VkImageAspectFlags aspectFlag = (srcImage->IsDepthFormat()) ? VK_IMAGE_ASPECT_DEPTH_BIT : VK_IMAGE_ASPECT_COLOR_BIT;
        aspectFlag |= (srcImage->IsStencilFormat()) ? VK_IMAGE_ASPECT_STENCIL_BIT : 0;

        VkImageMemoryBarrier imageStartBarrier[2];
        imageStartBarrier[0].sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        imageStartBarrier[0].pNext = nullptr;
        imageStartBarrier[0].srcAccessMask = VulkanTypeConversion::GetAccessFlagsFromLayout(srcLayout);
        imageStartBarrier[0].dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
        imageStartBarrier[0].oldLayout = VulkanTypeConversion::ConvertImageLayoutToVulkan(srcLayout);
        imageStartBarrier[0].newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
        imageStartBarrier[0].srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        imageStartBarrier[0].dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        imageStartBarrier[0].image = vkSrcImage->GetImage();
        imageStartBarrier[0].subresourceRange = VkImageSubresourceRange{ aspectFlag, 0, maxSrcLevel, 0, 1 };

        imageStartBarrier[1].sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        imageStartBarrier[1].pNext = nullptr;
        imageStartBarrier[1].srcAccessMask = 0;
        imageStartBarrier[1].dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        imageStartBarrier[1].oldLayout = VulkanTypeConversion::ConvertImageLayoutToVulkan(dstLayout);
        imageStartBarrier[1].newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
        imageStartBarrier[1].srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        imageStartBarrier[1].dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        imageStartBarrier[1].image = vkDstImage->GetImage();
        imageStartBarrier[1].subresourceRange = VkImageSubresourceRange{ aspectFlag, 0, maxDstLevel, 0, 1 };

        VulkanApi::GetApi()->vkCmdPipelineBarrier(vkCmdBuffer->GetCommandBuffer(), VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0
            , 0, nullptr
            , 0, nullptr
            , 2, imageStartBarrier);

        VulkanApi::GetApi()->vkCmdCopyImage(vkCmdBuffer->GetCommandBuffer(), vkSrcImage->GetImage(), VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
            vkDstImage->GetImage(), VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, regionCount, vkCopyImageArray.Data());

        // Transition images back to previous layout
        imageStartBarrier[0].sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        imageStartBarrier[0].pNext = nullptr;
        imageStartBarrier[0].srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
        imageStartBarrier[0].dstAccessMask = VulkanTypeConversion::GetAccessFlagsFromLayout(srcLayout);
        imageStartBarrier[0].oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
        imageStartBarrier[0].newLayout = VulkanTypeConversion::ConvertImageLayoutToVulkan(srcLayout);
        imageStartBarrier[0].srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        imageStartBarrier[0].dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        imageStartBarrier[0].image = vkSrcImage->GetImage();
        imageStartBarrier[0].subresourceRange = VkImageSubresourceRange{ aspectFlag, 0, maxSrcLevel, 0, 1 };

        imageStartBarrier[1].sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        imageStartBarrier[1].pNext = nullptr;
        imageStartBarrier[1].srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        imageStartBarrier[1].dstAccessMask = VulkanTypeConversion::GetAccessFlagsFromLayout(dstLayout);
        imageStartBarrier[1].oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
        imageStartBarrier[1].newLayout = VulkanTypeConversion::ConvertImageLayoutToVulkan(dstLayout);
        imageStartBarrier[1].srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        imageStartBarrier[1].dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        imageStartBarrier[1].image = vkDstImage->GetImage();
        imageStartBarrier[1].subresourceRange = VkImageSubresourceRange{ aspectFlag, 0, maxDstLevel, 0, 1 };

        VulkanApi::GetApi()->vkCmdPipelineBarrier(vkCmdBuffer->GetCommandBuffer(), VK_PIPELINE_STAGE_TRANSFER_BIT, 
            VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, 0
            , 0, nullptr
            , 0, nullptr
            , 2, imageStartBarrier);
    }
}

void VulkanRenderDevice::CmdCopyImage(HalCommandBuffer* commandBuffer, HalImage* srcImage, HalImageLayout srcLayout,
    size_t swapChainIndex, uint32_t regionCount, HalImageCopy* regions)
{
    VkImage dstImage = _pSwapChain->GetSwapChainImage(swapChainIndex);
    if (srcImage && dstImage)
    {
        VulkanImage* vkSrcImage = static_cast<VulkanImage*>(srcImage);
        VulkanCommandBuffer* vkCmdBuffer = static_cast<VulkanCommandBuffer*>(commandBuffer);

        uint32_t maxSrcLevel = 0;

        // create VkImageCopy structs
        caveVector<VkImageCopy> vkCopyImageArray(_pInstance->GetEngineAllocator());
        vkCopyImageArray.Resize(regionCount);
        for (uint32_t i = 0; i < regionCount; i++)
        {
            vkCopyImageArray[i].srcOffset = VulkanTypeConversion::ConvertOffset3DToVulkan(regions[i]._srcOffset);
            vkCopyImageArray[i].srcSubresource.aspectMask = VulkanTypeConversion::ConvertImageAspectFlagsToVulkan(regions[i]._srcLayers._aspectMask);
            vkCopyImageArray[i].srcSubresource.baseArrayLayer = regions[i]._srcLayers._baseArrayLayer;
            vkCopyImageArray[i].srcSubresource.layerCount = regions[i]._srcLayers._layerCount;
            vkCopyImageArray[i].srcSubresource.mipLevel = regions[i]._srcLayers._mipLevel;

            if (regions[i]._srcLayers._mipLevel >= maxSrcLevel)
                maxSrcLevel = regions[i]._srcLayers._mipLevel + 1;

            vkCopyImageArray[i].dstOffset = VulkanTypeConversion::ConvertOffset3DToVulkan(regions[i]._dstOffset);
            vkCopyImageArray[i].dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            vkCopyImageArray[i].dstSubresource.baseArrayLayer = 0;
            vkCopyImageArray[i].dstSubresource.layerCount = 1;
            vkCopyImageArray[i].dstSubresource.mipLevel = 0;

            vkCopyImageArray[i].extent = VulkanTypeConversion::ConvertExtent3DToVulkan(regions[i]._extent);
        }

        // transition source and dest image to be ready for the copy
        assert(!srcImage->IsDepthFormat() || !srcImage->IsStencilFormat());

        VkImageMemoryBarrier imageStartBarrier[2];
        imageStartBarrier[0].sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        imageStartBarrier[0].pNext = nullptr;
        imageStartBarrier[0].srcAccessMask = 0;// VulkanTypeConversion::GetAccessFlagsFromLayout(srcLayout);
        imageStartBarrier[0].dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
        imageStartBarrier[0].oldLayout = VulkanTypeConversion::ConvertImageLayoutToVulkan(srcLayout);
        imageStartBarrier[0].newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
        imageStartBarrier[0].srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        imageStartBarrier[0].dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        imageStartBarrier[0].image = vkSrcImage->GetImage();
        imageStartBarrier[0].subresourceRange = VkImageSubresourceRange{ VK_IMAGE_ASPECT_COLOR_BIT, 0, maxSrcLevel, 0, 1 };

        imageStartBarrier[1].sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        imageStartBarrier[1].pNext = nullptr;
        imageStartBarrier[1].srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
        imageStartBarrier[1].dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        imageStartBarrier[1].oldLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
        imageStartBarrier[1].newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
        imageStartBarrier[1].srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        imageStartBarrier[1].dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        imageStartBarrier[1].image = dstImage;
        imageStartBarrier[1].subresourceRange = VkImageSubresourceRange{ VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 };

        VulkanApi::GetApi()->vkCmdPipelineBarrier(vkCmdBuffer->GetCommandBuffer(), VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0
            , 0, nullptr
            , 0, nullptr
            , 2, imageStartBarrier);

        VulkanApi::GetApi()->vkCmdCopyImage(vkCmdBuffer->GetCommandBuffer(), vkSrcImage->GetImage(), VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
            dstImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, regionCount, vkCopyImageArray.Data());

        // Transition images back to previous layout
        imageStartBarrier[0].sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        imageStartBarrier[0].pNext = nullptr;
        imageStartBarrier[0].srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
        imageStartBarrier[0].dstAccessMask = VulkanTypeConversion::GetAccessFlagsFromLayout(srcLayout);
        imageStartBarrier[0].oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
        imageStartBarrier[0].newLayout = VulkanTypeConversion::ConvertImageLayoutToVulkan(srcLayout);
        imageStartBarrier[0].srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        imageStartBarrier[0].dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        imageStartBarrier[0].image = vkSrcImage->GetImage();
        imageStartBarrier[0].subresourceRange = VkImageSubresourceRange{ VK_IMAGE_ASPECT_COLOR_BIT, 0, maxSrcLevel, 0, 1 };

        imageStartBarrier[1].sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        imageStartBarrier[1].pNext = nullptr;
        imageStartBarrier[1].srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        imageStartBarrier[1].dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
        imageStartBarrier[1].oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
        imageStartBarrier[1].newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
        imageStartBarrier[1].srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        imageStartBarrier[1].dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        imageStartBarrier[1].image = dstImage;
        imageStartBarrier[1].subresourceRange = VkImageSubresourceRange{ VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 };

        VulkanApi::GetApi()->vkCmdPipelineBarrier(vkCmdBuffer->GetCommandBuffer(), VK_PIPELINE_STAGE_TRANSFER_BIT,
            VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, 0
            , 0, nullptr
            , 0, nullptr
            , 2, imageStartBarrier);
    }
}

bool VulkanRenderDevice::CmdSubmitGraphicsQueue(HalSubmitInfo& submitInfo, HalFence* fence)
{
    VkSubmitInfo vkSubmitInfo = { VK_STRUCTURE_TYPE_SUBMIT_INFO };

    // setup command buffers
    caveVector<VkCommandBuffer> vkCommandBuffers(GetEngineAllocator());
    for (size_t i = 0; i < submitInfo._commandBufferCount; i++)
    {
        VulkanCommandBuffer* vkCmdBuffer = static_cast<VulkanCommandBuffer*>(submitInfo._pCommandBuffers[i]);
        vkCommandBuffers.Push(vkCmdBuffer->GetCommandBuffer());
    }

    // setup wait semaphores
    caveVector<VkSemaphore> vkWaitSemaphores(GetEngineAllocator());
    for (size_t i = 0; i < submitInfo._waitSemaphoreCount; i++)
    {
        VulkanSemaphore* vkSemaphore = static_cast<VulkanSemaphore*>(submitInfo._pWaitSemaphores[i]);
        vkWaitSemaphores.Push(vkSemaphore->GetSemaphore());
    }

    // setup wait stages
    caveVector<VkPipelineStageFlags> vkWaitStages(GetEngineAllocator());
    for (size_t i = 0; i < submitInfo._waitSemaphoreCount; i++)
    {
        VkPipelineStageFlags vkWaitStage = VulkanTypeConversion::ConvertPipelineFlagsToVulkan(*submitInfo._waitStageMask[i]);
        vkWaitStages.Push(vkWaitStage);
    }

    // setup signal semaphores
    caveVector<VkSemaphore> vkSignalSemaphores(GetEngineAllocator());
    for (size_t i = 0; i < submitInfo._signalSemaphoreCount; i++)
    {
        VulkanSemaphore* vkSemaphore = static_cast<VulkanSemaphore*>(submitInfo._pSignalSemaphores[i]);
        vkSignalSemaphores.Push(vkSemaphore->GetSemaphore());
    }


    vkSubmitInfo.commandBufferCount = static_cast<uint32_t>(submitInfo._commandBufferCount);
    vkSubmitInfo.pCommandBuffers = vkCommandBuffers.Data();
    vkSubmitInfo.waitSemaphoreCount = static_cast<uint32_t>(submitInfo._waitSemaphoreCount);
    vkSubmitInfo.pWaitSemaphores = vkWaitSemaphores.Data();
    vkSubmitInfo.pWaitDstStageMask = vkWaitStages.Data();
    vkSubmitInfo.signalSemaphoreCount = static_cast<uint32_t>(submitInfo._signalSemaphoreCount);
    vkSubmitInfo.pSignalSemaphores = vkSignalSemaphores.Data();

    VkResult result = VK_INCOMPLETE;
    if (_graphicsQueue)
    {
        VulkanFence* vkFence = static_cast<VulkanFence*>(fence);
        result = VulkanApi::GetApi()->vkQueueSubmit(_graphicsQueue, 1, &vkSubmitInfo, (vkFence) ? vkFence->GetFence() : nullptr);
    }

    return (result == VK_SUCCESS);
}

void VulkanRenderDevice::FlushCopies()
{
	// flush outstanding copies
	_pMemoryManager->SubmitCopies();
}

bool VulkanRenderDevice::PresentQueueSubmit(HalCommandBuffer* commandBuffer)
{
	if (!_pSwapChain)
		return false;

	VkResult result = VK_INCOMPLETE;
	if (_graphicsQueue)
	{
		// wait for possible copies
		_pMemoryManager->WaitForCopies();

		VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
		const VkSemaphore waitSemaphore = _pSwapChain->GetImageAvailableSemaphore();
		const VkSemaphore signalSemaphore = _pSwapChain->GetRenderingDoneSemaphore();
		VkCommandBuffer vkCommandBuffer = static_cast<VulkanCommandBuffer*>(commandBuffer)->GetCommandBuffer();

		VkSubmitInfo submitInfo;
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.pNext = nullptr;
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = &waitSemaphore;
		submitInfo.pWaitDstStageMask = waitStages;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &vkCommandBuffer;
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = &signalSemaphore;

		result = VulkanApi::GetApi()->vkQueueSubmit(_graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
	}

	return (result == VK_SUCCESS);
}

bool VulkanRenderDevice::PresentQueue(uint32_t imageIndex)
{
	if (!_pSwapChain)
		return false;

	VkResult result = VK_INCOMPLETE;
	if (_presentQueue)
	{
		const VkSemaphore waitSemaphore = _pSwapChain->GetRenderingDoneSemaphore();
		const VkSwapchainKHR swapChain = _pSwapChain->GetSwapChainHandle();

		VkPresentInfoKHR presentInfo;
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		presentInfo.pNext = nullptr;
		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = &waitSemaphore;
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = &swapChain;
		presentInfo.pImageIndices = &imageIndex;
		presentInfo.pResults = nullptr;

		result = VulkanApi::GetApi()->vkQueuePresentKHR(_presentQueue, &presentInfo);
	}

	return (result == VK_SUCCESS);
}

void VulkanRenderDevice::ReadPixels(void* data)
{
	if (_pSwapChain)
		_pSwapChain->ReadPixels(_graphicsQueueCommandPool, data);
}

}
