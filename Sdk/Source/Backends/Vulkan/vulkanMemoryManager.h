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

/// @file vulkanMemoryManager.h
///       Manage device memory

#include "halTypes.h"
#include "osPlatformLib.h"

#include "vulkan.h"

/** \addtogroup backend
*  @{
*
*/

namespace cave
{

///< forwards
class VulkanInstance;
class VulkanPhysicalDevice;
class VulkanRenderDevice;

/**
* Vulkan device memory allocation
*/
struct VulkanDeviceMemory
{
	VulkanDeviceMemory()
	{
		_offset = _size = 0;
		_vkDeviceMemory = VK_NULL_HANDLE;
	}

	uint64_t _offset;	///< Memory offset
	uint64_t _size;		///< Actual allocated size
	VkDeviceMemory _vkDeviceMemory;		///< Vulkan device memory handle
};

/**
* Vulkan device memory manager
*/
class VulkanMemoryManager
{
public:
	/**
	* @brief Constructor
	*
	* @param[in] instance			Pointer to instance object
	* @param[in] physicalDevice		Pointer to physical device
	* @param[in] renderDevice		Pointer to logical device
	*
	*/
	VulkanMemoryManager(VulkanInstance* instance, VulkanPhysicalDevice* physicalDevice, VulkanRenderDevice* renderDevice);
	/** @brief Destructor */
	virtual ~VulkanMemoryManager();

	/**
	* @brief Allocate device memory for any buffer usage
	*
	* @param[in] memRequirements	VkMemoryRequirements struct
	* @param[in] properties			VkMemoryPropertyFlags required memory properties
	* @param[out] deviceMemory		Filled in VulkanDeviceMemory struct on success
	*
	*/
	void AllocateBufferMemory(VkMemoryRequirements& memRequirements, VkMemoryPropertyFlags properties, VulkanDeviceMemory& deviceMemory);

	/**
	* @brief Release device memory back to system
	*
	* @param[in] deviceMemory		VulkanDeviceMemory struct returned on AllocateBufferMemory call
	*
	*/
	void ReleaseBufferMemory(VulkanDeviceMemory& deviceMemory);

private:
	uint32_t ChooseMemoryHeap(VkMemoryRequirements& memRequirements, VkMemoryPropertyFlags properties);

private:
	VulkanInstance* _pInstance;	///< Pointer to instance object
	VulkanPhysicalDevice* _pPhysicalDevice;	///< Pointer to physical device
	VulkanRenderDevice* _pRenderDevice;	///< Pointer to logical device
};

}

/** @}*/

