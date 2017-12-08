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
#include "vulkanSwapChain.h"
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
#include "vulkanApi.h"

#include<limits>
#include<set>

namespace cave
{

VulkanRenderDevice::VulkanRenderDevice(VulkanInstance* instance, VulkanPhysicalDevice* physicalDevice, VkSurfaceKHR surface)
	: HalRenderDevice(instance)
	, _pInstance(instance)
	, _pPhysicalDevice(physicalDevice)
	, _presentationSurface(surface)
	, _vkDevice(VK_NULL_HANDLE)
	, _presentQueue(VK_NULL_HANDLE)
	, _pSwapChain(nullptr)
	, _presentQueueCommandPool(VK_NULL_HANDLE)
	, _presentCommandBufferArray(nullptr)
{
	std::set<int> uniqueQueueFamilies;
	// First query the graphics queue index
	uint32_t graphicsQueueFamilyIndex = physicalDevice->GetQueueFamilyIndex(VkQueueFlagBits::VK_QUEUE_GRAPHICS_BIT);
	if (graphicsQueueFamilyIndex == (std::numeric_limits<uint32_t>::max)())
		throw BackendException("CreateRenderDevice: no suitable device queues found");

	uniqueQueueFamilies.insert(graphicsQueueFamilyIndex);

	_presentationQueueFamilyIndex = (std::numeric_limits<uint32_t>::max)();
	if (surface)
	{
		_presentationQueueFamilyIndex = physicalDevice->GetPresentationQueueFamilyIndex(graphicsQueueFamilyIndex, surface);
		if (_presentationQueueFamilyIndex == (std::numeric_limits<uint32_t>::max)())
			throw BackendException("CreateRenderDevice: no suitable device queues found");

		uniqueQueueFamilies.insert(graphicsQueueFamilyIndex);
	}


	// extensions
	std::vector<const char*> extensions;
	if (surface && physicalDevice->HasSwapChainSupport())
	{
		extensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
	}

	// enable minimum features
	VkPhysicalDeviceFeatures supportedFeatures = _pPhysicalDevice->GetPhysicalDeviceFeatures();
	VkPhysicalDeviceFeatures requiredFeatures = { 0 };

	requiredFeatures.tessellationShader = supportedFeatures.tessellationShader;
	requiredFeatures.geometryShader = supportedFeatures.geometryShader;
	requiredFeatures.multiDrawIndirect = supportedFeatures.multiDrawIndirect;

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
		&requiredFeatures
	};

	VkResult result = VulkanApi::GetApi()->vkCreateDevice(physicalDevice->GetPhysicalDeviceHandle(), &deviceCreateInfo, nullptr, &_vkDevice);

	if (result != VK_SUCCESS)
	{
		throw BackendException("Failed to create vulkan device");
	}

	// finally load device level functions
	if (!VulkanApi::GetApi()->LoadDeviceFunctions(&_vkDevice))
	{
		throw BackendException("Failed to create vulkan device");
	}

	// allocate presentation queue
	if (surface)
		VulkanApi::GetApi()->vkGetDeviceQueue(_vkDevice, _presentationQueueFamilyIndex, 0, &_presentQueue);

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
}

VulkanRenderDevice::~VulkanRenderDevice()
{
	if (_vkDevice)
	{
		// Shutdown savely. Wait that the device is idle
		VulkanApi::GetApi()->vkDeviceWaitIdle(_vkDevice);
	}

	if (_presentCommandBufferArray)
	{
		const uint32_t imageCount = _pSwapChain->GetSwapChainImageCount();
		if (_presentCommandBufferArray[0] != nullptr)
			VulkanApi::GetApi()->vkFreeCommandBuffers(_vkDevice, _presentQueueCommandPool, imageCount, &_presentCommandBufferArray[0]);

		DeallocateArray<VkCommandBuffer>(*_pInstance->GetEngineAllocator(), _presentCommandBufferArray);
	}

	if (_presentationSurface)
		VulkanApi::GetApi()->vkDestroySurfaceKHR(_pInstance->GetInstanceHandle(), _presentationSurface, nullptr);

	if (_presentQueueCommandPool)
		VulkanApi::GetApi()->vkDestroyCommandPool(_vkDevice, _presentQueueCommandPool, nullptr);

	if (_pSwapChain)
	{
		DeallocateDelete(*_pInstance->GetEngineAllocator(), *_pSwapChain);
	}

	if (_vkDevice)
	{
		VulkanApi::GetApi()->vkDestroyDevice(_vkDevice, nullptr);
		_vkDevice = nullptr;
	}
}

void VulkanRenderDevice::CreateSwapChain(SwapChainInfo& )
{
	if (!_pPhysicalDevice || !_vkDevice)
		throw BackendException("Vulkan device not properly setup");

	if (!_presentationSurface)
		throw BackendException("No presentation surface created");

	// we support only one swap chain per render device
	if (_pSwapChain)
		return;

	_pSwapChain = AllocateObject<VulkanSwapChain>(*_pInstance->GetEngineAllocator(), _pInstance, _pPhysicalDevice, this);

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
	}
}

const HalImageFormat VulkanRenderDevice::GetSwapChainImageFormat()
{
	HalImageFormat format = HalImageFormat::Undefined;
	if (_pSwapChain)
		format = VulkanTypeConversion::ConvertImageFormatFromVulkan(_pSwapChain->GetSwapChainImageFormat());

	return format;
}

HalShader* VulkanRenderDevice::CreateShader(ShaderType type, ShaderLanguage language)
{
	if (!_pPhysicalDevice || !_vkDevice)
		return nullptr;

	VulkanShader* shader = AllocateObject<VulkanShader>(*_pInstance->GetEngineAllocator(), this, type, language);

	return shader;
}

HalVertexInput* VulkanRenderDevice::CreateVertexInput()
{
	if (!_pPhysicalDevice || !_vkDevice)
		return nullptr;

	VulkanVertexInput* vertexInput = AllocateObject<VulkanVertexInput>(*_pInstance->GetEngineAllocator(), this);

	return vertexInput;
}

HalInputAssembly* VulkanRenderDevice::CreateInputAssembly()
{
	if (!_pPhysicalDevice || !_vkDevice)
		return nullptr;

	VulkanInputAssembly* inputAssembly = AllocateObject<VulkanInputAssembly>(*_pInstance->GetEngineAllocator(), this);

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

HalPipelineLayout* VulkanRenderDevice::CreatePipelineLayout(caveVector<HalDescriptorSetLayout>& descriptorSetLayouts
	, caveVector<HalPushConstantRange>& pushConstants)
{
	if (!_pPhysicalDevice || !_vkDevice)
		return nullptr;

	VulkanPipelineLayout* pipelineLayout = AllocateObject<VulkanPipelineLayout>(*_pInstance->GetEngineAllocator(), this, descriptorSetLayouts, pushConstants);

	return pipelineLayout;
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

}
