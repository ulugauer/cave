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
#include <vector>
#include <cstring>

namespace cave
{

/**
* @brief Helper class to find extension names
*
* @pram[in] extensionName	Extension name to search for
* @pram[in] deviceExtensions	Arrary of available extensions
*
* @return true if found
*/
static bool CheckExtensionAvailability(const char *extensionName, const std::vector<VkExtensionProperties> &deviceExtensions)
{
	for (size_t i = 0; i < deviceExtensions.size(); ++i)
	{
		if (strcmp(deviceExtensions[i].extensionName, extensionName) == 0)
		{
			return true;
		}
	}
	return false;
}

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

void VulkanPhysicalDevice::SetupPhysicalDeviceExtension(HalDeviceExtensions& deviceExtensionsCaps)
{
	uint32_t deviceExtensionCount = 0;
	if (VulkanApi::GetApi()->vkEnumerateDeviceExtensionProperties(_vkPhysicalDevice, nullptr, &deviceExtensionCount, nullptr) != VK_SUCCESS)
	{
		return;
	}
	std::vector<VkExtensionProperties> deviceExtensions(deviceExtensionCount);
	if (VulkanApi::GetApi()->vkEnumerateDeviceExtensionProperties(_vkPhysicalDevice, nullptr, &deviceExtensionCount, &deviceExtensions[0]) != VK_SUCCESS)
	{
		return;
	}

	// A list of extension we currently looking for
	if (CheckExtensionAvailability(VK_KHR_SWAPCHAIN_EXTENSION_NAME, deviceExtensions))
	{
		deviceExtensionsCaps.caps.bits.bSwapChainSupport = true;
	}
	if (CheckExtensionAvailability(VK_NV_GLSL_SHADER_EXTENSION_NAME, deviceExtensions))
	{
		deviceExtensionsCaps.caps.bits.bGLSLSupport = true;
	}
}

void VulkanPhysicalDevice::GetApiVersion(uint32_t& major, uint32_t& minor, uint32_t& patch)
{
	major = VK_VERSION_MAJOR(_physicalDeviceProperties.apiVersion);
	minor = VK_VERSION_MINOR(_physicalDeviceProperties.apiVersion);
	patch = VK_VERSION_PATCH(_physicalDeviceProperties.apiVersion);
}

void VulkanPhysicalDevice::GetDriverVersion(uint32_t& major, uint32_t& minor, uint32_t& patch)
{
	major = VK_VERSION_MAJOR(_physicalDeviceProperties.driverVersion);
	minor = VK_VERSION_MINOR(_physicalDeviceProperties.driverVersion);
	patch = VK_VERSION_PATCH(_physicalDeviceProperties.driverVersion);
}

const char* VulkanPhysicalDevice::GetDeviceName()
{
	return _physicalDeviceProperties.deviceName;
}

bool VulkanPhysicalDevice::HasQueueCapabilites(VkQueueFlags flags, VkSurfaceKHR presentationSurface)
{
	for (uint32_t i = 0; i < _physicalDeviceQueueFamilyCount; ++ i)
	{
		VkBool32 presentSupport = true;
		if (presentationSurface)
			VulkanApi::GetApi()->vkGetPhysicalDeviceSurfaceSupportKHR(_vkPhysicalDevice, i, presentationSurface, &presentSupport);

		if (_physicalDeviceQueueFamilyArray[i].queueFlags & flags)
			return true;
	}

	return false;
}

bool VulkanPhysicalDevice::HasSwapChainSupport()
{
	if (!_vkPhysicalDevice)
		return false;

	// extensions
	uint32_t deviceExtensionCount = 0;
	if (VulkanApi::GetApi()->vkEnumerateDeviceExtensionProperties(_vkPhysicalDevice, nullptr, &deviceExtensionCount, nullptr) != VK_SUCCESS)
	{
		return false;
	}
	std::vector<VkExtensionProperties> deviceExtensions(deviceExtensionCount);
	if (VulkanApi::GetApi()->vkEnumerateDeviceExtensionProperties(_vkPhysicalDevice, nullptr, &deviceExtensionCount, &deviceExtensions[0]) != VK_SUCCESS)
	{
		return false;
	}

	std::vector<const char*> extensions;
	extensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);

	for (size_t i = 0; i < extensions.size(); ++i)
	{
		if (!CheckExtensionAvailability(extensions[i], deviceExtensions))
		{
			return false;
		}
	}

	return  true;
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

uint32_t VulkanPhysicalDevice::GetPresentationQueueFamilyIndex(uint32_t graphisIndex, VkSurfaceKHR presentationSurface)
{
	bool findMatchingQueueIndex = (std::numeric_limits<uint32_t>::max)() != graphisIndex;
	uint32_t queueIndex = (std::numeric_limits<uint32_t>::max)();
	for (uint32_t i = 0; i < _physicalDeviceQueueFamilyCount; ++i)
	{
		VkBool32 presentSupport = false;
		if (presentationSurface)
			VulkanApi::GetApi()->vkGetPhysicalDeviceSurfaceSupportKHR(_vkPhysicalDevice, i, presentationSurface, &presentSupport);

		if (_physicalDeviceQueueFamilyArray[i].queueCount > 0 && presentSupport)
		{
			// Store the first one which supports presntation
			if ((std::numeric_limits<uint32_t>::max)() == queueIndex)
				queueIndex = i;

			if (findMatchingQueueIndex && graphisIndex == i)
				break;
			else if (!findMatchingQueueIndex)
				break;
		}
	}

	return queueIndex;
}

bool VulkanPhysicalDevice::PresentationQueueSupported(SwapChainInfo& swapChainInfo, uint32_t &familyQueueIndex)
{
	bool supported = false;

	for (uint32_t i = 0; i < _physicalDeviceQueueFamilyCount; ++i)
	{
		VkBool32 retVal = VK_FALSE;
#ifdef VK_USE_PLATFORM_WIN32_KHR
		UNREFERENCED_PARAMETER(swapChainInfo);
		retVal = VulkanApi::GetApi()->vkGetPhysicalDeviceWin32PresentationSupportKHR(_vkPhysicalDevice, i);
#else
		retVal = VulkanApi::GetApi()->vkGetPhysicalDeviceXcbPresentationSupportKHR(_vkPhysicalDevice, i, swapChainInfo.connection, swapChainInfo.visualId);
#endif
		if (retVal)
		{
			// we take the first queue which fullfills the request
			familyQueueIndex = i;
			supported = true;
			break;
		}
	}

	return supported;
}

}
