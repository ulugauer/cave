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

/// @file vulkanColorBlend.h
///       Vulkan color blending state

#include "halColorBlend.h"
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
* @brief Vulkan color blend state
*/
class VulkanColorBlend : public HalColorBlend
{
public:
	/**
	* @brief Constructor
	*
	* @param[in] device				Pointer to device object
	* @param[in] colorBlendState	Color blending state
	* @param[in] blendAttachments	Color blend attachment state
	*/
	VulkanColorBlend(VulkanRenderDevice* device, HalColorBlendState& colorBlendState
					, caveVector<HalColorBlendAttachment>& blendAttachments);

	/** @brief Destructor */
	virtual ~VulkanColorBlend();

	/**
	* @brief Get color blend state info
	*
	* @return Vulkan VkPipelineColorBlendStateCreateInfo
	*/
	const VkPipelineColorBlendStateCreateInfo& GetColorBlendStateInfo() const
	{
		return _colorBlendStateStateInfo;
	}

private:
	VulkanRenderDevice* _pDevice;	///< Pointer to device object
	caveVector<VkPipelineColorBlendAttachmentState> _colorBlendAttachments;	///< Array of blending attachment states
	VkPipelineColorBlendStateCreateInfo	_colorBlendStateStateInfo;		///< Color blend state info
};

}

/** @}*/


