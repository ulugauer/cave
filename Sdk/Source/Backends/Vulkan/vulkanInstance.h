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

/// @file vulkanInstance.h
///       Vulkan instance

#include "halInstance.h"
#include "osPlatformLib.h"

#include "vulkan.h"

/** \addtogroup backend 
*  @{
*		
*/

namespace cave
{

///< forwards
class VulkanPhysicalDevice;

/**
* Vulkan device instance
*/
class VulkanInstance : public HalInstance
{
public:
	/**
	* @brief Constructor
	*
	* @param allocator	Pointer to global allocator
	* @param type	Backend graphics API 
	* @param applicationName	Name of application (optional)
	*
	*/
	VulkanInstance(std::shared_ptr<AllocatorBase> allocator, BackendInstanceTypes type, const char* applicationName);
	/** @brief Destructor */
	virtual ~VulkanInstance();

	/**
	* @brief Create a list of physical devices supported by this instance
	*
	* @return True if a list was successfuly build
	*/
	bool QueryPhysicalDevices();

	/**
	* @brief Create a hardware render device
	*
	* @param[in] allocator	Engine global allocator
	* @param[in] swapChainInfo	Swap chain creation info
	*
	* @return Pointer to a hardware render device
	*/
	HalRenderDevice* CreateRenderDevice(std::shared_ptr<AllocatorBase> allocator, SwapChainInfo& swapChainInfo) override;

	/**
	* @brief Get vulkan instance handle
	*
	* @return Lowlevel vulkan handle
	*/
	VkInstance GetInstanceHandle() { return _vkInstance; }

	/**
	* @brief GetAllocator
	*
	* @return Pointer Engine allocator
	*/
	std::shared_ptr<AllocatorBase> GetEngineAllocator() { return _allocator; }

private:
	/**
	* @brief Create a presentation surface
	*
	* @param[in] swapChainInfo	Swap chain creation info
	*
	* @return Vulkan handle to surface
	*/
	VkSurfaceKHR CreatePresentaionSurface(SwapChainInfo& swapChainInfo);

private:
	std::shared_ptr<AllocatorBase> _allocator;	///< Pointer to global allocator
	VkInstance _vkInstance;	///< Handle to vulkan instance
	uint32_t _physicalDeviceCount;	///< Physical devices accessible by this instance
	VulkanPhysicalDevice* _physicalDeviceArray;	///< Pointer to array of physical devices
};

}

/** @}*/
