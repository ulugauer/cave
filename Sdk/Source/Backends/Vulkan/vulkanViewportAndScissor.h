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

/// @file vulkanViewportAndScissor.h
///       Vulkan viewport and scissor state

#include "halViewportAndScissor.h"
#include "osPlatformLib.h"

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
* @brief Vulkan viewport and scissor state
*/
class VulkanViewportAndScissor : public HalViewportAndScissor
{
public:
	/**
	* @brief Constructor
	*
	* @param[in] device		Pointer to device object
	* @param[in] viewport	Viewport dimension
	* @param[in] scissor	Scissor dimension
	*
	*/
	VulkanViewportAndScissor(VulkanRenderDevice* device, HalViewport& viewport, HalScissor& scissor);

	/** @brief Destructor */
	virtual ~VulkanViewportAndScissor();

	/**
	* @brief Get viewport state info
	*
	* @return Vulkan VkPipelineRasterizationStateCreateInfo
	*/
	const VkPipelineViewportStateCreateInfo& GetViewportStateInfo() const
	{
		return _vkViewportStateInfo;
	}

private:
	VulkanRenderDevice* _pDevice;	///< Pointer to device object
	VkViewport _vkViewport;			///< Vulkan viewport struct
	VkRect2D _vkScissorRect2D;		///< Vulkan scissor rect
	VkPipelineViewportStateCreateInfo _vkViewportStateInfo;	///< Viewport and scissor setup state
};

}

/** @}*/

