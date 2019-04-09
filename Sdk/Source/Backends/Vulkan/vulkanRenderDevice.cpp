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
#include "vulkanCommandPool.h"
#include "vulkanCommandBuffer.h"
#include "vulkanDescriptorPool.h"
#include "vulkanDescriptorSet.h"
#include "vulkanBuffer.h"
#include "vulkanImage.h"
#include "vulkanImageView.h"
#include "vulkanSampler.h"
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

HalSemaphore* VulkanRenderDevice::CreateSemaphore()
{
	if (!_pPhysicalDevice || !_vkDevice)
		return nullptr;

	VulkanSemaphore* semaphore = AllocateObject<VulkanSemaphore>(*_pInstance->GetEngineAllocator(), this);

	return semaphore;
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
	vkRenderPassInfo.framebuffer = _presentationFramebuffers[renderPassBeginInfo._swapChainIndex];
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
