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

/// @file vulkanDynamicState.h
///       Vulkan dynamic pipeline state

#include "halDynamicState.h"

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
* @brief Vulkan vertex input state
*/
class VulkanDynamicState : public HalDynamicState
{
public:
	/**
	* @brief Constructor
	*
	* @param[in] device			Pointer to render device object
	* @param[in] dynamicStates	Array of dynamic states
	*/
	VulkanDynamicState(VulkanRenderDevice* device, caveVector<HalDynamicStates>& dynamicStates);

	/** @brief Destructor */
	virtual ~VulkanDynamicState();

	/**
	* @brief Get color blend state info
	*
	* @return Vulkan VkPipelineColorBlendStateCreateInfo
	*/
	const VkPipelineDynamicStateCreateInfo& GetColorBlendStateInfo() const
	{
		return _dynamicStateInfo;
	}

private:
	VulkanRenderDevice* _pDevice;	///< Pointer to device object
	caveVector<VkDynamicState> _dynamicStates;	///< Array of dynamic states
	VkPipelineDynamicStateCreateInfo _dynamicStateInfo;		///< Dynamic state info
};

}

/** @}*/


