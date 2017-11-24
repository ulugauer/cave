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

/// @file vulkanDepthStencil.h
///       Vulkan depth stencil state

#include "halDepthStencil.h"

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
* @brief Vulkan depth stencil state
*/
class VulkanDepthStencil : public HalDepthStencil
{
public:
	/**
	* @brief Constructor
	*
	* @param[in] device	Pointer to device object
	* @param[in] depthStencilSetup	Depth Stencil setup struct
	*
	*/
	VulkanDepthStencil(VulkanRenderDevice* device, HalDepthStencilSetup& depthStencilSetup);

	/** @brief Destructor */
	virtual ~VulkanDepthStencil();


private:
	VulkanRenderDevice* _pDevice;	///< Pointer to device object
	VkPipelineDepthStencilStateCreateInfo _depthStencilStateInfo;	///< Depth stencil state
};

}

/** @}*/


