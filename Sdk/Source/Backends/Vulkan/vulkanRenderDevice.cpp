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

#include "vulkanRenderDevice.h"
#include "vulkanPhysicalDevice.h"
#include "vulkanApi.h"

#include<limits>

namespace cave
{

VulkanRenderDevice::VulkanRenderDevice(VulkanPhysicalDevice* physicalDevice)
	: HalRenderDevice()
	, _pPhysicalDevice(physicalDevice)
	, _vkDevice(nullptr)
{
	// First query the graphics queue index
	uint32_t graphicsQueueFamilyIndex = physicalDevice->GetQueueFamilyIndex(VkQueueFlagBits::VK_QUEUE_GRAPHICS_BIT);
	if (graphicsQueueFamilyIndex == (std::numeric_limits<uint32_t>::max)())
		throw BackendException("CreateRenderDevice: no suitable device queues found");

	// enable minimum features
	VkPhysicalDeviceFeatures supportedFeatures = _pPhysicalDevice->GetPhysicalDeviceFeatures();
	VkPhysicalDeviceFeatures requiredFeatures = { 0 };

	requiredFeatures.tessellationShader = supportedFeatures.tessellationShader;
	requiredFeatures.geometryShader = supportedFeatures.geometryShader;
	requiredFeatures.multiDrawIndirect = supportedFeatures.multiDrawIndirect;

	VkDeviceQueueCreateInfo deviceQueueCreateInfo =
	{
		VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,  
		nullptr,                                     
		0,                                           
		graphicsQueueFamilyIndex,
		1,							// Single queue currently
		nullptr,					// and no priorities      
	};

	VkDeviceCreateInfo deviceCreateInfo =
	{
		VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,           
		nullptr,                                        
		0,                                              
		1,										// single VkDeviceQueueCreateInfo currently
		&deviceQueueCreateInfo,
		0,                                             
		nullptr,                                       
		0,										// no device extension so far
		nullptr,                                 
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
}

VulkanRenderDevice::~VulkanRenderDevice()
{
	if (_vkDevice)
	{
		// Shutdown savely. Wait that the device is idle
		VulkanApi::GetApi()->vkDeviceWaitIdle(_vkDevice);
		VulkanApi::GetApi()->vkDestroyDevice(_vkDevice, nullptr);
		_vkDevice = nullptr;
	}
}

}
