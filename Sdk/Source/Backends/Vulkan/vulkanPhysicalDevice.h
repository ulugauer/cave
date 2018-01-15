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

/// @file vulkanPhysicalDevice.h
///       Vulkan physical device

#include "halRenderDevice.h"
#include "Memory/allocatorBase.h"

#include "vulkan.h"

#include <memory>

/** \addtogroup backend 
*  @{
*		
*/

namespace cave
{

/**
* Vulkan physical device
*/
class VulkanPhysicalDevice
{
public:
	/**
	* @brief Constructor
	*
	*/
	VulkanPhysicalDevice();

	/** @brief Destructor */
	virtual ~VulkanPhysicalDevice();

	/**
	* @brief Set the vulkan physical device handle.
	*		 In addition this function queries the device properties and features
	*
	* @param allocator	Handle global allocator
	* @param physicalDevice	Handle to vulkan physical devide
	*
	*/
	void SetPhysicalDeviceHandle(std::shared_ptr<AllocatorBase> allocator, VkPhysicalDevice physicalDevice);

	/**
	* @brief Release internal allocated structures
	*
	* @param allocator	Handle global allocator
	*
	*/
	void ReleasePhysicalDevice(std::shared_ptr<AllocatorBase> allocator);

	/**
	* @brief Get vulkan physical device handle
	*
	* @return Lowlevel vulkan handle
	*/
	VkPhysicalDevice GetPhysicalDeviceHandle() { return _vkPhysicalDevice; }

	/**
	* @brief Check if this physical device supportes the
	*		 requested queues.
	*
	* @param[in] flags	Handle global allocator
	* @param[in] presentationSurface	Handle to surface. If not null the queue must ber able to serve presents
	*
	* @return true if physical devices matchees
	*/
	bool HasQueueCapabilites(VkQueueFlags flags, VkSurfaceKHR presentationSurface=nullptr);

	/**
	* @brief Check if this physical device supportes swap chains
	*
	* @return true if physical devices supports swwap chains
	*/
	bool HasSwapChainSupport();

	/**
	* @brief Query queue familiy index
	*
	* @param queueBit	Queue familiy bit
	*
	* @return Index of queue familiy
	*/
	uint32_t GetQueueFamilyIndex(VkQueueFlagBits queueBit);

	/**
	* @brief Query queue presentation familiy index.
	*		 If graphicsIndex != MAX_UINT try to find a queue which matches this index 
	*
	* @param[in] graphicsIndex	graphics queue index
	* @param[in] presentationSurface	Handle to surface.
	*
	* @return Index of queue presentation familiy
	*/
	uint32_t GetPresentationQueueFamilyIndex(uint32_t graphicsIndex, VkSurfaceKHR presentationSurface);

	/**
	* @brief Query device properties
	*
	* @return Struct of device proeprties
	*/
	const VkPhysicalDeviceProperties& GetPhysicalDeviceProperties() const { return _physicalDeviceProperties; }

	/**
	* @brief Query supported device features
	*
	* @return Struct of supported device features
	*/
	const VkPhysicalDeviceFeatures& GetPhysicalDeviceFeatures() const { return _physicalDeviceFeatures; }

	/**
	* @brief Query supported device features
	*
	* @param[in] swapChainInfo	Swap chain creation info
	* @param[out] familyQueueIndex	Filled with family queue index supporting presentation
	*
	* @return true if any queue familiy supports presentation
	*/
	bool PresentationQueueSupported(SwapChainInfo& swapChainInfo, uint32_t &familyQueueIndex);

	/**
	* @brief Query physical device memory proeprties
	*
	* return Pointer to VkPhysicalDeviceMemoryProperties
	*/
	const VkPhysicalDeviceMemoryProperties& GetPhysicalDeviceMemoryProperties() {
		return _physicalDeviceMemoryProperties;
	}

	/**
	* @brief Query API version number
	*
	* @param[out] major		Major API version number
	* @param[out] minor		Minor API version number
	* @param[out] patch		Patch API version number
	*
	*/
	void GetApiVersion(uint32_t& major, uint32_t& minor, uint32_t& patch);

	/**
	* @brief Query Driver version number
	*
	* @param[out] major		Major driver version number
	* @param[out] minor		Minor driver version number
	* @param[out] patch		Patch driver version number
	*
	*/
	void GetDriverVersion(uint32_t& major, uint32_t& minor, uint32_t& patch);

	/**
	* @brief Query device name
	*
	* return Pointer string
	*/
	const char* GetDeviceName();

private:
	VkPhysicalDevice _vkPhysicalDevice;	///< Handle to vulkan device
	VkPhysicalDeviceProperties _physicalDeviceProperties;	///< Properties of the device
	VkPhysicalDeviceFeatures _physicalDeviceFeatures;	///< Features of the device
	VkPhysicalDeviceMemoryProperties _physicalDeviceMemoryProperties;	///< Memory properties of the devicde
	uint32_t _physicalDeviceQueueFamilyCount;	///< Physical devices queue families available
	VkQueueFamilyProperties* _physicalDeviceQueueFamilyArray;	///< Array of device queue families
};

}

/** @}*/
