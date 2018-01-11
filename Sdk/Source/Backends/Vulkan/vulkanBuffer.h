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

/// @file vulkanBuffer.h
///       Vulkan device buffer

#include "halBuffer.h"
#include "vulkanMemoryManager.h"

#include "vulkan.h"

/** \addtogroup backend
*  @{
*
*/

namespace cave
{

///< forwards
class VulkanRenderDevice;

/**
* @brief Vulkan device buffer
*/
class VulkanBuffer : public HalBuffer
{
public:
	/**
	* @brief Constructor
	*
	* @param[in] device			Pointer to render device object
	* @param[in] bufferInfo		Buffer create info
	*/
	VulkanBuffer(VulkanRenderDevice* device, HalBufferInfo& bufferInfo);

	/** @brief Destructor */
	virtual ~VulkanBuffer();

	/**
	* @brief Bind Binds a buffer and allocates memory
	*
	*/
	void Bind() override;

	/**
	* @brief Copy data to buffer
	*
	* @param[in] offset		Start offset from where to copy data
	* @param[in] size		The size of the memory range to copy
	* @param[in] pData		Pointer to data
	*
	*/
	virtual void Update(uint64_t offset, uint64_t size, const void* pData) override;

	/**
	* @brief Map buffer to virtual memory address
	*
	* @param[in] offset		Start offset from memory start
	* @param[in] size		The size of the memory range to map from offset
	* @param[out] ppData	Points to a pointer in which is returned a host-accessible pointer to the beginning of the mapped range
	*
	*/
	void Map(uint64_t offset, uint64_t size, void** ppData) override;

	/**
	* @brief Unmap previously mapped buffer
	*
	*/
	void Unmap() override;

	/**
	* @brief Get pipeline layout object
	*
	* @return Vulkan buffer object
	*/
	VkBuffer GetBuffer() { return _vkBuffer; }

private:
	VulkanRenderDevice* _pDevice;	///< Pointer to device object
	VkBuffer _vkBuffer;	///< Low level vulkan handle
	uint32_t* _familyIndicesArray;		///< Familiy indices array. Only required for buffers in non-exclusive mode
	VkMemoryPropertyFlags _vkMemProperties;	///< Vulkan memory properties
	VkBufferCreateInfo _vkCreateInfo;	///< Vulkan buffer creation info
	VulkanDeviceMemory _deviceMemory;	///< Allocate device memory info
};

}

/** @}*/




