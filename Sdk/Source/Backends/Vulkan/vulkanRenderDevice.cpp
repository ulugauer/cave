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
#include "vulkanSwapChain.h"
#include "vulkanApi.h"

#include<limits>
#include<set>

namespace cave
{

VulkanRenderDevice::VulkanRenderDevice(VulkanInstance* instance, VulkanPhysicalDevice* physicalDevice, VkSurfaceKHR surface)
	: HalRenderDevice()
	, _pInstance(instance)
	, _pPhysicalDevice(physicalDevice)
	, _presentationSurface(surface)
	, _vkDevice(nullptr)
	, _presentQueue(nullptr)
	, _pSwapChain(nullptr)
{
	std::set<int> uniqueQueueFamilies;
	// First query the graphics queue index
	uint32_t graphicsQueueFamilyIndex = physicalDevice->GetQueueFamilyIndex(VkQueueFlagBits::VK_QUEUE_GRAPHICS_BIT);
	if (graphicsQueueFamilyIndex == (std::numeric_limits<uint32_t>::max)())
		throw BackendException("CreateRenderDevice: no suitable device queues found");

	uniqueQueueFamilies.insert(graphicsQueueFamilyIndex);

	uint32_t presentationQueueFamilyIndex = (std::numeric_limits<uint32_t>::max)();
	if (surface)
	{
		presentationQueueFamilyIndex = physicalDevice->GetPresentationQueueFamilyIndex(graphicsQueueFamilyIndex, surface);
		if (presentationQueueFamilyIndex == (std::numeric_limits<uint32_t>::max)())
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

	if (surface)
		VulkanApi::GetApi()->vkGetDeviceQueue(_vkDevice, presentationQueueFamilyIndex, 0, &_presentQueue);
}

VulkanRenderDevice::~VulkanRenderDevice()
{
	if (_presentationSurface)
		VulkanApi::GetApi()->vkDestroySurfaceKHR(_pInstance->GetInstanceHandle(), _presentationSurface, nullptr);

	if (_pSwapChain)
	{
		DeallocateDelete(*_pInstance->GetEngineAllocator(), *_pSwapChain);
	}

	if (_vkDevice)
	{
		// Shutdown savely. Wait that the device is idle
		VulkanApi::GetApi()->vkDeviceWaitIdle(_vkDevice);
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
}

}
