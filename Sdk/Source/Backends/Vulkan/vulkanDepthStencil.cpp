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
#include "vulkanApi.h"

#include<limits>

namespace cave
{

/**
* @brief Convert from generic stencil operation to vulkan stencil operation
*
* @param[in] op	Generic stencil operation
*
* @return Vulkan stencil operation
*/
static VkStencilOp ConvertStencilOpToVulkan(StencilOp op)
{
	VkStencilOp stencilOp = VK_STENCIL_OP_KEEP;

	switch (op)
	{
	case StencilOp::Keep:
		stencilOp = VK_STENCIL_OP_KEEP;
		break;
	case StencilOp::Zero:
		stencilOp = VK_STENCIL_OP_ZERO;
		break;
	case StencilOp::Replace:
		stencilOp = VK_STENCIL_OP_REPLACE;
		break;
	case StencilOp::IncrementClamp:
		stencilOp = VK_STENCIL_OP_INCREMENT_AND_CLAMP;
		break;
	case StencilOp::DecrementClamp:
		stencilOp = VK_STENCIL_OP_DECREMENT_AND_CLAMP;
		break;
	case StencilOp::Invert:
		stencilOp = VK_STENCIL_OP_INVERT;
		break;
	case StencilOp::IncrementWarp:
		stencilOp = VK_STENCIL_OP_INCREMENT_AND_WRAP;
		break;
	case StencilOp::DecrementWarp:
		stencilOp = VK_STENCIL_OP_DECREMENT_AND_WRAP;
		break;
	default:
		stencilOp = VK_STENCIL_OP_KEEP;
		break;
	}

	return stencilOp;
}

/**
* @brief Convert from generic compare operation to vulkan compare operation
*
* @param[in] compOp	Generic compare operation
*
* @return Vulkan compare operation
*/
static VkCompareOp ConvertDepthCompareOpToVulkan(CompareOp compOp)
{
	VkCompareOp compareOp = VK_COMPARE_OP_NEVER;

	switch (compOp)
	{
	case CompareOp::Never:
		compareOp = VK_COMPARE_OP_NEVER;
		break;
	case CompareOp::Less:
		compareOp = VK_COMPARE_OP_LESS;
		break;
	case CompareOp::Equal:
		compareOp = VK_COMPARE_OP_EQUAL;
		break;
	case CompareOp::LessEqual:
		compareOp = VK_COMPARE_OP_LESS_OR_EQUAL;
		break;
	case CompareOp::Greater:
		compareOp = VK_COMPARE_OP_GREATER;
		break;
	case CompareOp::NotEqual:
		compareOp = VK_COMPARE_OP_NOT_EQUAL;
		break;
	case CompareOp::GreaterEqual:
		compareOp = VK_COMPARE_OP_GREATER_OR_EQUAL;
		break;
	case CompareOp::Always:
		compareOp = VK_COMPARE_OP_ALWAYS;
		break;
	default:
		compareOp = VK_COMPARE_OP_NEVER;
		break;
	}

	return compareOp;
}

/**
* @brief Convert from generic stencil state to vulkan stencil state
*
* @param[in] stencilSetup			Generic stencil state
* @param[in,out] stencilOpState		Vulkan stencil state
*
*/
static void GetStencilState(HalStencilOpSetup& stencilSetup, VkStencilOpState& stencilOpState)
{
	stencilOpState.failOp = ConvertStencilOpToVulkan(stencilSetup._failOp);
	stencilOpState.passOp = ConvertStencilOpToVulkan(stencilSetup._passOp);
	stencilOpState.depthFailOp = ConvertStencilOpToVulkan(stencilSetup._depthFailOp);
	stencilOpState.compareOp = ConvertDepthCompareOpToVulkan(stencilSetup._compareOp);
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
	_depthStencilStateInfo.depthCompareOp = ConvertDepthCompareOpToVulkan(depthStencilSetup._depthCompareOp);
	_depthStencilStateInfo.depthBoundsTestEnable = depthStencilSetup._depthBoundsTestEnable;
	_depthStencilStateInfo.minDepthBounds = depthStencilSetup._minDepthBounds;
	_depthStencilStateInfo.maxDepthBounds = depthStencilSetup._maxDepthBounds;
	GetStencilState(depthStencilSetup._front, _depthStencilStateInfo.front);
	GetStencilState(depthStencilSetup._back, _depthStencilStateInfo.back);
}

VulkanDepthStencil::~VulkanDepthStencil()
{
}


}
