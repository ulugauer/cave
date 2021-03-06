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

/// @file vulkanVertexInput.h
///       Vulkan vertex input state

#include "halVertexInput.h"
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
* @brief Vulkan vertex input state
*/
class VulkanVertexInput : public HalVertexInput
{
public:
	/**
	* @brief Constructor
	*
	* @param[in] device				Pointer to device object
	* @param[in] vertexInputState	Vertex input setup struct
	*
	*/
	VulkanVertexInput(VulkanRenderDevice* device, HalVertexInputStateInfo& vertexInputState);

	/** @brief Destructor */
	virtual ~VulkanVertexInput();

	/**
	* @brief Get start of binding index
	*		 This basically tells you where your vertex buffers should be bound
	*
	* @return first binding point
	*/
	const uint32_t GetBindingBase() const
	{
		return _bindingBase;
	}

	/**
	* @brief Get vertex input state info
	*
	* @return Vulkan VkPipelineRasterizationStateCreateInfo
	*/
	const VkPipelineVertexInputStateCreateInfo& GetVertexInputStateInfo() const
	{
		return _vkVertexInputStateInfo;
	}
	
private:
	VulkanRenderDevice* _pDevice;	///< Pointer to device object
	VkVertexInputBindingDescription* _bindingsDescArray;	///< Bindings description array
	VkVertexInputAttributeDescription* _attributesDescArray;	///< Attribute description array
	uint32_t _bindingBase;	///< First binding point
	VkPipelineVertexInputStateCreateInfo _vkVertexInputStateInfo;		///< Vertex setup state info
};

}

/** @}*/


