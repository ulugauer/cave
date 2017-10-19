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
#include "vulkanApi.h"

#include <iostream>
#include <vector>

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


VulkanInstance::VulkanInstance(BackendInstanceTypes type)
	: HalInstance(type)
{
	// this loads the library 
	VulkanApi* pApi = VulkanApi::GetApi();
	if (!pApi)
	{
		std::cerr << "ERROR: Failed to load vulkan library\n";
		return;
	}

	// load global accessible functions
	if (!pApi->LoadGlobalFunctions())
	{
		std::cerr << "ERROR: Failed to load vulkan global functions\n";
		return;
	}

	// We need to setup all possible used extension before we can create an instance
	uint32_t extensions_count = 0;
	if ((pApi->vkEnumerateInstanceExtensionProperties(nullptr, &extensions_count, nullptr) != VK_SUCCESS) ||
		(extensions_count == 0))
	{
		std::cout << "Error occurred during instance extensions enumeration!" << std::endl;
		return;
	}

	std::vector<VkExtensionProperties> available_extensions(extensions_count);
	if (pApi->vkEnumerateInstanceExtensionProperties(nullptr, &extensions_count, &available_extensions[0]) != VK_SUCCESS)
	{
		std::cerr << "Error occurred during instance extensions enumeration!" << std::endl;
		return;
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
			std::cerr << "Could not find instance extension named \"" << extensions[i] << "\"!" << std::endl;
			return ;
		}
	}

	// create the instance
	VkResult result = VK_SUCCESS;
	VkApplicationInfo appInfo = {};
	VkInstanceCreateInfo instanceCreateInfo = {};

	// setup application info
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "Application";
	appInfo.applicationVersion = 1;
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
		std::cerr << "ERROR: Failed to create vulkan instance\n";
		return;
	}

	// load instance accessible functions
	if (!pApi->LoadInstanceFunctions(&_vkInstance))
	{
		std::cerr << "ERROR: Failed to load vulkan global functions\n";
		return;
	}
}

VulkanInstance::~VulkanInstance()
{
	if (_vkInstance)
	{
		VulkanApi::GetApi()->vkDestroyInstance(_vkInstance, nullptr);
	}
}

}
