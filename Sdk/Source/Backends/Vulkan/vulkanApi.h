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
#pragma once

/// @file vulkanApi.h
///       Vulkan instance

#include "osPlatformLib.h"
#include "vulkan.h"

#include <iostream>

/** \addtogroup backend 
*  @{
*		
*/

namespace cave
{

/** function pointer definitions of Vulkan API
* @{
*/
typedef PFN_vkVoidFunction	(VKAPI_PTR* vkGetInstanceProcAddrPtr) (VkInstance instance, const char* pName);
typedef VkResult	(VKAPI_PTR* vkEnumerateInstanceExtensionPropertiesPtr) (const char* pLayerName, uint32_t* pPropertyCount, VkExtensionProperties* pProperties);
typedef VkResult	(VKAPI_PTR* vkEnumerateInstanceLayerPropertiesPtr)(uint32_t* pPropertyCount, VkLayerProperties* pProperties);
typedef VkResult	(VKAPI_PTR* vkCreateInstancePtr) (const VkInstanceCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkInstance* pInstance);

/** @}*/


/**
* Vulkan api
*/
class VulkanApi
{
private:
	/**
	* @brief Constructor
	*
	*/
	VulkanApi() 
	{
#if defined(_WIN32)		
		_hVulkan = OsPlatformLib::OsLoadLibrary("vulkan-1");
#else
		_hVulkan = OsPlatformLib::OsLoadLibrary("vulkan");
#endif

		if (_hVulkan)
		{
			bool retval = true;
			retval &= LoadFunction(_hVulkan, "vkGetInstanceProcAddr", vkGetInstanceProcAddr);
			if (vkGetInstanceProcAddr)
			{
				retval &= LoadGlobalFunction("vkEnumerateInstanceExtensionProperties", vkEnumerateInstanceExtensionProperties);
				retval &= LoadGlobalFunction("vkEnumerateInstanceLayerProperties", vkEnumerateInstanceLayerProperties);
				retval &= LoadGlobalFunction("vkEnumerateInstanceExtensionProperties", vkGetInstanceProcAddr);
			}
		}

	}
	/** @brief Destructor */
	~VulkanApi()
	{
		if (_hVulkan)
			OsPlatformLib::OsUnloadLibrary(_hVulkan);

		_hVulkan = nullptr;
	}

	/**
	* @brief Template function to load function from vulkan library
	*
	* @param[in]  module	Library handle
	* @param[in]  fnName	Function name
	* @param[out] fnPtr		Function pointer
	*
	* @return true if loading function succeded
	*/
	template<typename TFuncType>
	bool LoadFunction(osLibraryHandle module, const char* fnName, TFuncType& fnPtr)
	{
		fnPtr = reinterpret_cast<TFuncType>(OsPlatformLib::OsGetProcAddress(module, fnName));

		if (fnPtr == nullptr)
			std::cerr << "Could not load function: " << fnName << "!" << std::endl;

		return fnPtr != nullptr;
	}

	/**
	* @brief Template function to load function from vulkan library
	*
	* @param[in]  fnName	Function name
	* @param[out] fnPtr		Function pointer
	*
	* @return true if loading function succeded
	*/
	template<typename TFuncType>
	bool LoadGlobalFunction(const char* fnName, TFuncType& fnPtr)
	{
		fnPtr = reinterpret_cast<TFuncType>(vkGetInstanceProcAddr(nullptr, fnName));

		if (fnPtr == nullptr)
			std::cout << "Could not global load function: " << fnName << "!" << std::endl;

		return fnPtr != nullptr;
	}

public:

	/** 
	* @brief factory for Vulkan API, this will load the ICD on first use
	*/
	static VulkanApi* GetApi()
	{
		static VulkanApi api;
		return (api._hVulkan != 0) ? &api : nullptr;
	}


private:
    osLibraryHandle _hVulkan; ///< Vulkan library handle

public:
	/** function pointers into Vulkan ICD
	* @{
	*/

	// ************************************************************ //
	// Exported functions                                           //
	//                                                              //
	// These functions are always exposed by vulkan libraries.      //
	// ************************************************************ //
	vkGetInstanceProcAddrPtr	vkGetInstanceProcAddr;

	// ************************************************************ //
	// Global level functions                                       //
	//                                                              //
	// They allow checking what instance extensions are available   //
	// and allow creation of a Vulkan Instance.                     //
	// ************************************************************ // 
	vkEnumerateInstanceExtensionPropertiesPtr	vkEnumerateInstanceExtensionProperties;
	vkEnumerateInstanceLayerPropertiesPtr		vkEnumerateInstanceLayerProperties;
	vkCreateInstancePtr							vkCreateInstance;
};

}

/** @}*/
