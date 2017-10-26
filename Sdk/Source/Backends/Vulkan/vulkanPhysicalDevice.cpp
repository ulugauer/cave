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

/// @file vulkanPhysicalDevice.cpp
///       Vulkan render device

#include "vulkanPhysicalDevice.h"
#include "vulkanApi.h"

#include <limits>

namespace cave
{

VulkanPhysicalDevice::VulkanPhysicalDevice()
	: _vkPhysicalDevice(nullptr)
{
	_physicalDeviceQueueFamilyCount = 0;
	_physicalDeviceQueueFamilyArray = nullptr;
}

VulkanPhysicalDevice::~VulkanPhysicalDevice()
{
	
}

void VulkanPhysicalDevice::ReleasePhysicalDevice(std::shared_ptr<AllocatorBase> allocator)
{
	if (_physicalDeviceQueueFamilyArray)
		DeallocateArray<VkQueueFamilyProperties>(*allocator, _physicalDeviceQueueFamilyArray);
}

void VulkanPhysicalDevice::SetPhysicalDeviceHandle(std::shared_ptr<AllocatorBase> allocator, VkPhysicalDevice physicalDevice)
{
	_vkPhysicalDevice = physicalDevice;
	VulkanApi::GetApi()->vkGetPhysicalDeviceProperties(_vkPhysicalDevice, &_physicalDeviceProperties);
	VulkanApi::GetApi()->vkGetPhysicalDeviceFeatures(_vkPhysicalDevice, &_physicalDeviceFeatures);
	VulkanApi::GetApi()->vkGetPhysicalDeviceMemoryProperties(_vkPhysicalDevice, &_physicalDeviceMemoryProperties);

	// Query queue information
	VulkanApi::GetApi()->vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &_physicalDeviceQueueFamilyCount, nullptr);
	if (_physicalDeviceQueueFamilyCount)
	{
		_physicalDeviceQueueFamilyArray = AllocateArray<VkQueueFamilyProperties>(*allocator, _physicalDeviceQueueFamilyCount);
		VulkanApi::GetApi()->vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &_physicalDeviceQueueFamilyCount, _physicalDeviceQueueFamilyArray);
	}
}

bool VulkanPhysicalDevice::HasQueueCapabilites(VkQueueFlags flags)
{
	for (uint32_t i = 0; i < _physicalDeviceQueueFamilyCount; ++ i)
	{
		if (_physicalDeviceQueueFamilyArray[i].queueFlags & flags)
			return true;
	}

	return false;
}

uint32_t VulkanPhysicalDevice::GetQueueFamilyIndex(VkQueueFlagBits queueBit)
{
	uint32_t index = (std::numeric_limits<uint32_t>::max)();

	for (uint32_t i = 0; i < _physicalDeviceQueueFamilyCount; ++i)
	{
		if (_physicalDeviceQueueFamilyArray[i].queueCount > 0 &&
			_physicalDeviceQueueFamilyArray[i].queueFlags & queueBit)
		{
			// we take the first queue which fullfils the request
			index = i;
			break;
		}
	}

	return index;
}

bool VulkanPhysicalDevice::PresentationQueueSupported(SwapChainInfo& swapChainInfo, uint32_t &familiyQueueIndex)
{
	bool supported = false;

	for (uint32_t i = 0; i < _physicalDeviceQueueFamilyCount; ++i)
	{
		VkBool32 retVal = VK_FALSE;
#ifdef VK_USE_PLATFORM_WIN32_KHR
		retVal = VulkanApi::GetApi()->vkGetPhysicalDeviceWin32PresentationSupportKHR(_vkPhysicalDevice, i);
#else
		retVal = vkGetPhysicalDeviceXcbPresentationSupportKHR(_vkPhysicalDevice, i, swapChainInfo.connection, swapChainInfo.visualId);
#endif
		if (retVal)
		{
			// we take the first queue which fullfills the request
			familiyQueueIndex = i;
			supported = true;
			break;
		}
	}

	return supported;
}

}
