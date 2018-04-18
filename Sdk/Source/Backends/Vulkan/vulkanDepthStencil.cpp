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

/// @file vulkanDepthStencil.cpp
///       Vulkan depth stencil state

#include "vulkanDepthStencil.h"
#include "vulkanRenderDevice.h"
#include "vulkanConversion.h"
#include "vulkanApi.h"

#include<limits>

namespace cave
{

/**
* @brief Convert from generic stencil state to vulkan stencil state
*
* @param[in] stencilSetup			Generic stencil state
* @param[in,out] stencilOpState		Vulkan stencil state
*
*/
static void GetStencilState(HalStencilOpSetup& stencilSetup, VkStencilOpState& stencilOpState)
{
	stencilOpState.failOp = VulkanTypeConversion::ConvertStencilOpToVulkan(stencilSetup._failOp);
	stencilOpState.passOp = VulkanTypeConversion::ConvertStencilOpToVulkan(stencilSetup._passOp);
	stencilOpState.depthFailOp = VulkanTypeConversion::ConvertStencilOpToVulkan(stencilSetup._depthFailOp);
	stencilOpState.compareOp = VulkanTypeConversion::ConvertDepthCompareOpToVulkan(stencilSetup._compareOp);
	stencilOpState.compareMask = stencilSetup._compareMask;
	stencilOpState.writeMask = stencilSetup._writeMask;
	stencilOpState.reference = stencilSetup._reference;
}

VulkanDepthStencil::VulkanDepthStencil(VulkanRenderDevice* device, HalDepthStencilSetup& depthStencilSetup)
	: HalDepthStencil(depthStencilSetup)
	, _pDevice(device)
{
	// Setup depth stencil state
	_depthStencilStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
	_depthStencilStateInfo.pNext = nullptr;
	_depthStencilStateInfo.flags = 0;
	_depthStencilStateInfo.depthTestEnable = depthStencilSetup._depthTestEnable;
	_depthStencilStateInfo.depthWriteEnable = depthStencilSetup._depthWriteEnable;
	_depthStencilStateInfo.depthCompareOp = VulkanTypeConversion::ConvertDepthCompareOpToVulkan(depthStencilSetup._depthCompareOp);
	_depthStencilStateInfo.depthBoundsTestEnable = depthStencilSetup._depthBoundsTestEnable;
	_depthStencilStateInfo.minDepthBounds = depthStencilSetup._minDepthBounds;
	_depthStencilStateInfo.maxDepthBounds = depthStencilSetup._maxDepthBounds;
	_depthStencilStateInfo.stencilTestEnable = depthStencilSetup._stencilTestEnable;
	GetStencilState(depthStencilSetup._front, _depthStencilStateInfo.front);
	GetStencilState(depthStencilSetup._back, _depthStencilStateInfo.back);
}

VulkanDepthStencil::~VulkanDepthStencil()
{
}


}
