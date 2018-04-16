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

/// @file vulkanPipelineLayout.h
///       Vulkan pipeline layout

#include "halPipelineLayout.h"

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
* @brief Vulkan layout sets
*/
class VulkanPipelineLayout : public HalPipelineLayout
{
public:
	/**
	* @brief Constructor
	*
	* @param[in] device			Pointer to device object
	* @param[in] descriptorSet	Pointer to descriptor set object
	* @param[in] pushConstants	Pipeline push constant ranges array
	*/
	VulkanPipelineLayout(VulkanRenderDevice* device
		, HalDescriptorSet* descriptorSet
		, caveVector<HalPushConstantRange>& pushConstants);

	/** @brief Destructor */
	virtual ~VulkanPipelineLayout();

	/**
	* @brief Get pipeline layout object
	*		 This function creates the object if not yet done
	*
	* @return Vulkan VkPipelineLayout object
	*/
	VkPipelineLayout GetPipelineLayout();

private:
	VulkanRenderDevice* _pDevice;		///< Pointer to device object
	caveVector<VkPushConstantRange> _vkPushConstantRange;	///< Array of vulkan push constant ranges
	VkPipelineLayout _vkPipelineLayout;	///< Vulkan pipeline layout handle
	VkPipelineLayoutCreateInfo _vkPipelineLayoutInfo;	///< Vulkan layoute set setup
};

}

/** @}*/


