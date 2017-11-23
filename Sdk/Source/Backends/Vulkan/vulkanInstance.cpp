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

/// @file vulkanInstance.cpp
///       Vulkan instance

#include "vulkanInstance.h"
#include "vulkanPhysicalDevice.h"
#include "vulkanRenderDevice.h"
#include "vulkanApi.h"

#include <iostream>
#include <vector>
#include <cstring>

namespace cave
{

/**
* @brief Helper class to find extension names
*
* @pram[in] extension_name	Extension name to search for
* @pram[in] available_extensions	Arrary of available extensions
*
* @return true if found
*/
static bool CheckExtensionAvailability(const char *extension_name, const std::vector<VkExtensionProperties> &available_extensions)
{
	for (size_t i = 0; i < available_extensions.size(); ++i)
	{
		if (strcmp(available_extensions[i].extensionName, extension_name) == 0)
		{
			return true;
		}
	}
	return false;
}


VulkanInstance::VulkanInstance(std::shared_ptr<AllocatorBase> allocator, BackendInstanceTypes type, const char* applicationName)
	: HalInstance(allocator, type)
	, _physicalDeviceCount(0)
	, _physicalDeviceArray(nullptr)
{
	// this loads the library 
	VulkanApi* pApi = VulkanApi::GetApi();
	if (!pApi)
	{
		throw BackendException("Failed to load vulkan library");
	}

	// load global accessible functions
	if (!pApi->LoadGlobalFunctions())
	{
		throw BackendException("Failed to load vulkan global functions");
	}

	// We need to setup all possible used extension before we can create an instance
	uint32_t extensions_count = 0;
	if ((pApi->vkEnumerateInstanceExtensionProperties(nullptr, &extensions_count, nullptr) != VK_SUCCESS) ||
		(extensions_count == 0))
	{
		throw BackendException("Error no instance extensions found");
	}

	std::vector<VkExtensionProperties> available_extensions(extensions_count);
	if (pApi->vkEnumerateInstanceExtensionProperties(nullptr, &extensions_count, &available_extensions[0]) != VK_SUCCESS)
	{
		throw BackendException("Error instance extensions query failed");
	}

	// we probably need the surface extension
	std::vector<const char*> extensions;
	extensions.push_back(VK_KHR_SURFACE_EXTENSION_NAME);
#if defined(VK_USE_PLATFORM_WIN32_KHR)
	extensions.push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
#elif defined(VK_USE_PLATFORM_XCB_KHR)
	extensions.push_back(VK_KHR_XCB_SURFACE_EXTENSION_NAME);
#endif

	for (size_t i = 0; i < extensions.size(); ++i)
	{
		if (!CheckExtensionAvailability(extensions[i], available_extensions))
		{
			std::string msg("Error failed to load extension: ");
			msg += extensions[i];
			throw BackendException(msg);
		}
	}

	// create the instance
	VkResult result = VK_SUCCESS;
	VkApplicationInfo appInfo = {};
	VkInstanceCreateInfo instanceCreateInfo = {};

	// setup application info
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = applicationName;
	appInfo.applicationVersion = 0;
	appInfo.pEngineName = "cave";
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.apiVersion = VK_MAKE_VERSION(1, 0, 3);
	// setup instance
	instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instanceCreateInfo.pNext = nullptr;
	instanceCreateInfo.flags = 0;
	instanceCreateInfo.pApplicationInfo = &appInfo;
	instanceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
	instanceCreateInfo.ppEnabledExtensionNames = &extensions[0];

	result = pApi->vkCreateInstance(&instanceCreateInfo, nullptr, &_vkInstance);

	if (result != VK_SUCCESS)
	{
		throw BackendException("Failed to create vulkan instance");
	}

	// load instance accessible functions
	if (!pApi->LoadInstanceFunctions(&_vkInstance))
	{
		throw BackendException("Failed to load vulkan instance functions");
	}
}

VulkanInstance::~VulkanInstance()
{
	if (_physicalDeviceArray)
	{
		for (uint32_t i = 0; i < _physicalDeviceCount; ++i)
		{
			_physicalDeviceArray[i].ReleasePhysicalDevice(_allocator);
		}
		DeallocateArray<VulkanPhysicalDevice>(*_allocator, _physicalDeviceArray);
	}
	if (_vkInstance)
	{
		VulkanApi::GetApi()->vkDestroyInstance(_vkInstance, nullptr);
	}
}


bool VulkanInstance::QueryPhysicalDevices()
{
	bool success = false;
	if (_vkInstance)
	{
		VulkanApi::GetApi()->vkEnumeratePhysicalDevices(_vkInstance, &_physicalDeviceCount, nullptr);
		if (_physicalDeviceCount)
		{
			std::vector<VkPhysicalDevice> availableDevices(_physicalDeviceCount);
			VulkanApi::GetApi()->vkEnumeratePhysicalDevices(_vkInstance, &_physicalDeviceCount, &availableDevices[0]);
			// Create VulkanPhysicalDevice array
			_physicalDeviceArray = AllocateArray<VulkanPhysicalDevice>(*_allocator, _physicalDeviceCount);
			if (_physicalDeviceArray)
			{
				success = true;
				for (uint32_t i = 0; i < _physicalDeviceCount; ++i)
				{
					_physicalDeviceArray[i].SetPhysicalDeviceHandle(_allocator, availableDevices[i]);
				}
			}
		}
	}

	return success;
}

HalRenderDevice* VulkanInstance::CreateRenderDevice(std::shared_ptr<AllocatorBase> allocator, SwapChainInfo& swapChainInfo)
{
	// Create a presentation surface if required
	VkSurfaceKHR presentationSurface = nullptr;
	if (!swapChainInfo.offscreen)
	{
		presentationSurface = CreatePresentaionSurface(swapChainInfo);
		if (!presentationSurface)
			throw BackendException("Error at presentation surface generation");
	}

	// We want to create a render device.
	// Check if we have a physical device which matches our needs
	VulkanPhysicalDevice* physicalDevice = nullptr;
	// We need graphics and transfer capability
	VkQueueFlags flags = VkQueueFlagBits::VK_QUEUE_GRAPHICS_BIT;
	flags |= VkQueueFlagBits::VK_QUEUE_TRANSFER_BIT;

	for (uint32_t i = 0; i < _physicalDeviceCount; ++i)
	{
		if (_physicalDeviceArray[i].HasQueueCapabilites(flags, presentationSurface) &&
			(!swapChainInfo.offscreen || _physicalDeviceArray[i].HasSwapChainSupport()))
		{
			physicalDevice = &_physicalDeviceArray[i];
			break;
		}
	}

	if (!physicalDevice)
		throw BackendException("CreateRenderDevice: no suitable device found");

	// Found something suitable -> create device
	VulkanRenderDevice* renderDevice = AllocateObject<VulkanRenderDevice>(*allocator, this, physicalDevice, presentationSurface);

	return renderDevice;
}

VkSurfaceKHR VulkanInstance::CreatePresentaionSurface(SwapChainInfo& swapChainInfo)
{
	VkSurfaceKHR presentationSurface = nullptr;
	VkResult success = VK_SUCCESS;
#ifdef VK_USE_PLATFORM_WIN32_KHR
	VkWin32SurfaceCreateInfoKHR surfaceCreateInfo = {};
	surfaceCreateInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
	surfaceCreateInfo.pNext = nullptr;
	surfaceCreateInfo.flags = 0;
	surfaceCreateInfo.hinstance = swapChainInfo.hInstance;
	surfaceCreateInfo.hwnd = swapChainInfo.hWindow;
	success = VulkanApi::GetApi()->vkCreateWin32SurfaceKHR(_vkInstance, &surfaceCreateInfo, nullptr, &presentationSurface);
#else
	VkXcbSurfaceCreateInfoKHR surfaceCreateInfo = {};
	surfaceCreateInfo.sType = VK_STRUCTURE_TYPE_XCB_SURFACE_CREATE_INFO_KHR;
	surfaceCreateInfo.pNext = nullptr;
	surfaceCreateInfo.flags = 0;
	surfaceCreateInfo.connection = swapChainInfo.connection;
	surfaceCreateInfo.window = swapChainInfo.window;
	success = VulkanApi::GetApi()->vkCreateXcbSurfaceKHR(_vkInstance, &surfaceCreateInfo, nullptr, &presentationSurface);
#endif

	if (success != VK_SUCCESS)
		return nullptr;

	return presentationSurface;
}

}
