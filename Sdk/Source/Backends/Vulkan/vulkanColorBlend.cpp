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

/// @file vulkanColorBlend.cpp
///       Vulkan color blending state

#include "vulkanColorBlend.h"
#include "vulkanRenderDevice.h"

#include "vulkanApi.h"

#include<limits>

namespace cave
{

/**
* @brief Convert from generic blend factor to vulkan blend factor
*
* @param[in] factor	Generic blend factor
*
* @return Vulkan blend factor
*/
static VkBlendFactor ConvertBlendFactorToVulkan(BlendFactor factor)
{
	VkBlendFactor blendFactor = VK_BLEND_FACTOR_ZERO;

	switch (factor)
	{
	case BlendFactor::Zero:
		blendFactor = VK_BLEND_FACTOR_ZERO;
		break;
	case BlendFactor::One:
		blendFactor = VK_BLEND_FACTOR_ONE;
		break;
	case BlendFactor::SrcColor:
		blendFactor = VK_BLEND_FACTOR_SRC_COLOR;
		break;
	case BlendFactor::OneMinusSrcColor:
		blendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_COLOR;
		break;
	case BlendFactor::DstColor:
		blendFactor = VK_BLEND_FACTOR_DST_COLOR;
		break;
	case BlendFactor::OneMinusDstColor:
		blendFactor = VK_BLEND_FACTOR_ONE_MINUS_DST_COLOR;
		break;
	case BlendFactor::SrcAlpha:
		blendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
		break;
	case BlendFactor::OneMinusSrcAlpha:
		blendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
		break;
	case BlendFactor::DstAlpha:
		blendFactor = VK_BLEND_FACTOR_DST_ALPHA;
		break;
	case BlendFactor::OneMinusDstAlpha:
		blendFactor = VK_BLEND_FACTOR_ONE_MINUS_DST_ALPHA;
		break;
	case BlendFactor::ConstantColor:
		blendFactor = VK_BLEND_FACTOR_CONSTANT_COLOR;
		break;
	case BlendFactor::OneMinusConstantColor:
		blendFactor = VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_COLOR;
		break;
	case BlendFactor::ConstantAlpha:
		blendFactor = VK_BLEND_FACTOR_CONSTANT_ALPHA;
		break;
	case BlendFactor::OneMinusConstantAlpha:
		blendFactor = VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_ALPHA;
		break;
	case BlendFactor::SrcAlphaSaturate:
		blendFactor = VK_BLEND_FACTOR_SRC_ALPHA_SATURATE;
		break;
	case BlendFactor::Src1Color:
		blendFactor = VK_BLEND_FACTOR_SRC1_COLOR;
		break;
	case BlendFactor::OneMinusSrc1Color:
		blendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC1_COLOR;
		break;
	case BlendFactor::Src1Alpha:
		blendFactor = VK_BLEND_FACTOR_SRC1_ALPHA;
		break;
	case BlendFactor::OneMinusSrc1Alpha:
		blendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC1_ALPHA;
		break;
	}

	return blendFactor;
}

/**
* @brief Convert from generic logical operation to vulkan logical operationr
*
* @param[in] op		Generic logical operation
*
* @return Vulkan logical operation
*/
static VkLogicOp ConvertLogicOpToVulkan(LogicOp op)
{
	VkLogicOp logicOp = VK_LOGIC_OP_CLEAR;

	switch (op)
	{
	case LogicOp::Clear:
		logicOp = VK_LOGIC_OP_CLEAR;
		break;
	case LogicOp::And:
		logicOp = VK_LOGIC_OP_AND;
		break;
	case LogicOp::AndReverse:
		logicOp = VK_LOGIC_OP_AND_REVERSE;
		break;
	case LogicOp::Copy:
		logicOp = VK_LOGIC_OP_COPY;
		break;
	case LogicOp::AndInverted:
		logicOp = VK_LOGIC_OP_AND_INVERTED;
		break;
	case LogicOp::NoOp:
		logicOp = VK_LOGIC_OP_NO_OP;
		break;
	case LogicOp::Xor:
		logicOp = VK_LOGIC_OP_XOR;
		break;
	case LogicOp::Or:
		logicOp = VK_LOGIC_OP_OR;
		break;
	case LogicOp::Nor:
		logicOp = VK_LOGIC_OP_NOR;
		break;
	case LogicOp::Equivalent:
		logicOp = VK_LOGIC_OP_EQUIVALENT;
		break;
	case LogicOp::Invert:
		logicOp = VK_LOGIC_OP_INVERT;
		break;
	case LogicOp::OrReverse:
		logicOp = VK_LOGIC_OP_OR_REVERSE;
		break;
	case LogicOp::CopyInverted:
		logicOp = VK_LOGIC_OP_COPY_INVERTED;
		break;
	case LogicOp::OrInverted:
		logicOp = VK_LOGIC_OP_OR_INVERTED;
		break;
	case LogicOp::Nand:
		logicOp = VK_LOGIC_OP_NAND;
		break;
	case LogicOp::Set:
		logicOp = VK_LOGIC_OP_SET;
		break;
	default:
		logicOp = VK_LOGIC_OP_CLEAR;
		break;
	}

	return logicOp;
}

/**
* @brief Convert from generic blend operation to vulkan blend operationr
*
* @param[in] op		Generic blend operation
*
* @return Vulkan blend operation
*/
static VkBlendOp ConvertBlendOpToVulkan(BlendOp op)
{
	VkBlendOp blendOp = VK_BLEND_OP_ADD;

	switch (op)
	{
	case BlendOp::Add:
		blendOp = VK_BLEND_OP_ADD;
		break;
	case BlendOp::Subtract:
		blendOp = VK_BLEND_OP_SUBTRACT;
		break;
	case BlendOp::ReverseSubtract:
		blendOp = VK_BLEND_OP_REVERSE_SUBTRACT;
		break;
	case BlendOp::Min:
		blendOp = VK_BLEND_OP_MIN;
		break;
	case BlendOp::Max:
		blendOp = VK_BLEND_OP_MAX;
		break;
	default:
		blendOp = VK_BLEND_OP_ADD;
		break;
	}

	return blendOp;
}

/**
* @brief Convert from generic color components to vulkan color components
*
* @param[in] colorComponentFlags	Generic color components
*
* @return Vulkan color components
*/
static VkColorComponentFlags ConvertColorComponentToVulkan(ColorComponentFlags colorComponentFlags)
{
	VkColorComponentFlags flags = 0;

	if (colorComponentFlags & static_cast<uint32_t>(ColorComponents::Red))
		flags |= VK_COLOR_COMPONENT_R_BIT;
	if (colorComponentFlags & static_cast<uint32_t>(ColorComponents::Green))
		flags |= VK_COLOR_COMPONENT_G_BIT;
	if (colorComponentFlags & static_cast<uint32_t>(ColorComponents::Blue))
		flags |= VK_COLOR_COMPONENT_B_BIT;
	if (colorComponentFlags & static_cast<uint32_t>(ColorComponents::Alpha))
		flags |= VK_COLOR_COMPONENT_A_BIT;

	return flags;
}

VulkanColorBlend::VulkanColorBlend(VulkanRenderDevice* device
								, HalColorBlendState& colorBlendState
								, caveVector<HalColorBlendAttachment>& blendAttachments)
	: HalColorBlend(device, colorBlendState, blendAttachments)
	, _pDevice(device)
	, _colorBlendAttachments(device->GetEngineAllocator())
{
	// first convert attachments
	if (!blendAttachments.Empty())
	{
		_colorBlendAttachments.Reserve(blendAttachments.Size());
		for (size_t i = 0; i < blendAttachments.Size(); ++i)
		{
			VkPipelineColorBlendAttachmentState attachment;
			attachment.alphaBlendOp = ConvertBlendOpToVulkan(blendAttachments[i]._alphaBlendOp);
			attachment.colorBlendOp = ConvertBlendOpToVulkan(blendAttachments[i]._colorBlendOp);
			attachment.blendEnable = blendAttachments[i]._blendEnable;
			attachment.colorWriteMask = ConvertColorComponentToVulkan(blendAttachments[i]._colorWriteMask);
			attachment.dstAlphaBlendFactor = ConvertBlendFactorToVulkan(blendAttachments[i]._dstAlphaBlendFactor);
			attachment.dstColorBlendFactor = ConvertBlendFactorToVulkan(blendAttachments[i]._dstColorBlendFactor);
			attachment.srcAlphaBlendFactor = ConvertBlendFactorToVulkan(blendAttachments[i]._srcAlphaBlendFactor);
			attachment.srcColorBlendFactor = ConvertBlendFactorToVulkan(blendAttachments[i]._srcColorBlendFactor);

			_colorBlendAttachments.Push(attachment);
		}
	}

	_colorBlendStateStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	_colorBlendStateStateInfo.pNext = nullptr;
	_colorBlendStateStateInfo.flags = 0;
	_colorBlendStateStateInfo.logicOpEnable = colorBlendState._logicOpEnable;
	_colorBlendStateStateInfo.logicOp = ConvertLogicOpToVulkan(colorBlendState._logicOp);
	_colorBlendStateStateInfo.attachmentCount = static_cast<uint32_t>(blendAttachments.Size());
	_colorBlendStateStateInfo.pAttachments = _colorBlendAttachments.Data();
	_colorBlendStateStateInfo.blendConstants[0] = colorBlendState._blendConstants[0];
	_colorBlendStateStateInfo.blendConstants[1] = colorBlendState._blendConstants[1];
	_colorBlendStateStateInfo.blendConstants[2] = colorBlendState._blendConstants[2];
	_colorBlendStateStateInfo.blendConstants[3] = colorBlendState._blendConstants[3];
}

VulkanColorBlend::~VulkanColorBlend()
{
	if (!_colorBlendAttachments.Empty())
		_colorBlendAttachments.Clear();
}


}
