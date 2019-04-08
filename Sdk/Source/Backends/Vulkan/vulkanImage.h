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

/// @file vulkanImage.h
///       Vulkan device image

#include "halImage.h"
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
* @brief Vulkan device image
*/
class VulkanImage : public HalImage
{
public:
	/**
	* @brief Constructor
	*
	* @param[in] device			Pointer to render device object
	* @param[in] imageInfo		Image create info
	*/
	VulkanImage(VulkanRenderDevice* device, HalImageInfo& imageInfo);

	/** @brief Destructor */
	virtual ~VulkanImage();

	/**
	* @brief Bind Binds a buffer and allocates memory
	*
	*/
	void Bind() override;

	/**
	* @brief Copy data to buffer
	*
	* @param[in] pData		Pointer to image data
	*
	*/
	virtual void Update(const void* pData) override;

	/**
	* @brief Get pipeline layout object
	*
	* @return Vulkan image object
	*/
	VkImage GetImage() { return _vkImage; }

private:
	VulkanRenderDevice * _pDevice;	///< Pointer to device object
	VkImage _vkImage;	///< Low level vulkan handle
	uint32_t* _familyIndicesArray;		///< Familiy indices array. Only required for images in non-exclusive mode
	VkMemoryPropertyFlags _vkMemProperties;	///< Vulkan memory properties
	VkImageCreateInfo _vkCreateInfo;	///< Vulkan image creation info
	VulkanDeviceMemory _deviceMemory;	///< Allocate device memory info
};

}

/** @}*/


