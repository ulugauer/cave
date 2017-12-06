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

/// @file vulkanConversion.cpp
///       Hal types to vulkan types

#include "vulkanConversion.h"
#include "vulkanRenderDevice.h"

#include "vulkanApi.h"

#include<limits>
#include<cassert>

namespace cave
{

VkPipelineBindPoint 
VulkanTypeConversion::ConvertPipelineBindPointToVulkan(HalPipelineBindPoints pipelineBindPoint)
{
	VkPipelineBindPoint vkBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;

	switch (pipelineBindPoint)
	{
	case HalPipelineBindPoints::Graphics:
		vkBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		break;
	case HalPipelineBindPoints::Compute:
		vkBindPoint = VK_PIPELINE_BIND_POINT_COMPUTE;
		break;
	default:
		vkBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		break;
	}

	return vkBindPoint;
}

VkFormat 
VulkanTypeConversion::ConvertImageFormatToVulkan(HalImageFormat imageFormat)
{
	VkFormat vkImageFormat = VK_FORMAT_UNDEFINED;

	switch (imageFormat)
	{
	case HalImageFormat::R8G8B8A8UNorm:
		vkImageFormat = VK_FORMAT_R8G8B8A8_UNORM;
		break;
	case HalImageFormat::R8G8B8A8SNorm:
		vkImageFormat = VK_FORMAT_R8G8B8A8_SNORM;
		break;
	default:
		vkImageFormat = VK_FORMAT_UNDEFINED;
		break;
	}

	assert(vkImageFormat != VK_FORMAT_UNDEFINED);
	return vkImageFormat;
}

VkSampleCountFlagBits 
VulkanTypeConversion::ConvertSampleCountToVulkan(HalSampleCount sampleCount)
{
	VkSampleCountFlagBits vkSampleCount = VK_SAMPLE_COUNT_1_BIT;

	switch (sampleCount)
	{
	case HalSampleCount::SampleCount1:
		vkSampleCount = VK_SAMPLE_COUNT_1_BIT;
		break;
	case HalSampleCount::SampleCount2:
		vkSampleCount = VK_SAMPLE_COUNT_2_BIT;
		break;
	case HalSampleCount::SampleCount4:
		vkSampleCount = VK_SAMPLE_COUNT_4_BIT;
		break;
	case HalSampleCount::SampleCount8:
		vkSampleCount = VK_SAMPLE_COUNT_8_BIT;
		break;
	case HalSampleCount::SampleCount16:
		vkSampleCount = VK_SAMPLE_COUNT_16_BIT;
		break;
	case HalSampleCount::SampleCount32:
		vkSampleCount = VK_SAMPLE_COUNT_32_BIT;
		break;
	case HalSampleCount::SampleCount64:
		vkSampleCount = VK_SAMPLE_COUNT_64_BIT;
		break;
	default:
		vkSampleCount = VK_SAMPLE_COUNT_1_BIT;
		break;
	}

	return vkSampleCount;
}

VkCullModeFlagBits 
VulkanTypeConversion::ConvertCullModeToVulkan(HalCullMode mode)
{
	VkCullModeFlagBits modeFlag = VK_CULL_MODE_NONE;

	switch (mode)
	{
	case HalCullMode::Front:
		modeFlag = VK_CULL_MODE_FRONT_BIT;
		break;
	case HalCullMode::Back:
		modeFlag = VK_CULL_MODE_BACK_BIT;
		break;
	case HalCullMode::FrontAndBack:
		modeFlag = VK_CULL_MODE_FRONT_AND_BACK;
		break;
	case HalCullMode::NoCull:
		modeFlag = VK_CULL_MODE_NONE;
		break;
	default:
		modeFlag = VK_CULL_MODE_NONE;
		break;
	}

	return modeFlag;
}

VkFrontFace 
VulkanTypeConversion::ConvertFrontFaceToVulkan(HalFrontFace face)
{
	VkFrontFace frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;

	switch (face)
	{
	case HalFrontFace::CounterClockwise:
		frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
		break;
	case HalFrontFace::Clockwise:
		frontFace = VK_FRONT_FACE_CLOCKWISE;
		break;
	default:
		frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
		break;
	}

	return frontFace;
}

VkPolygonMode 
VulkanTypeConversion::ConvertPolygonModeToVulkan(HalPolygonMode mode)
{
	VkPolygonMode polyMode = VK_POLYGON_MODE_FILL;

	switch (mode)
	{
	case HalPolygonMode::Fill:
		polyMode = VK_POLYGON_MODE_FILL;
		break;
	case HalPolygonMode::Line:
		polyMode = VK_POLYGON_MODE_LINE;
		break;
	case HalPolygonMode::Point:
		polyMode = VK_POLYGON_MODE_POINT;
		break;
	default:
		polyMode = VK_POLYGON_MODE_FILL;
		break;
	}

	return polyMode;
}

VkStencilOp 
VulkanTypeConversion::ConvertStencilOpToVulkan(HalStencilOp op)
{
	VkStencilOp stencilOp = VK_STENCIL_OP_KEEP;

	switch (op)
	{
	case HalStencilOp::Keep:
		stencilOp = VK_STENCIL_OP_KEEP;
		break;
	case HalStencilOp::Zero:
		stencilOp = VK_STENCIL_OP_ZERO;
		break;
	case HalStencilOp::Replace:
		stencilOp = VK_STENCIL_OP_REPLACE;
		break;
	case HalStencilOp::IncrementClamp:
		stencilOp = VK_STENCIL_OP_INCREMENT_AND_CLAMP;
		break;
	case HalStencilOp::DecrementClamp:
		stencilOp = VK_STENCIL_OP_DECREMENT_AND_CLAMP;
		break;
	case HalStencilOp::Invert:
		stencilOp = VK_STENCIL_OP_INVERT;
		break;
	case HalStencilOp::IncrementWarp:
		stencilOp = VK_STENCIL_OP_INCREMENT_AND_WRAP;
		break;
	case HalStencilOp::DecrementWarp:
		stencilOp = VK_STENCIL_OP_DECREMENT_AND_WRAP;
		break;
	default:
		stencilOp = VK_STENCIL_OP_KEEP;
		break;
	}

	return stencilOp;
}

VkCompareOp 
VulkanTypeConversion::ConvertDepthCompareOpToVulkan(HalCompareOp compOp)
{
	VkCompareOp compareOp = VK_COMPARE_OP_NEVER;

	switch (compOp)
	{
	case HalCompareOp::Never:
		compareOp = VK_COMPARE_OP_NEVER;
		break;
	case HalCompareOp::Less:
		compareOp = VK_COMPARE_OP_LESS;
		break;
	case HalCompareOp::Equal:
		compareOp = VK_COMPARE_OP_EQUAL;
		break;
	case HalCompareOp::LessEqual:
		compareOp = VK_COMPARE_OP_LESS_OR_EQUAL;
		break;
	case HalCompareOp::Greater:
		compareOp = VK_COMPARE_OP_GREATER;
		break;
	case HalCompareOp::NotEqual:
		compareOp = VK_COMPARE_OP_NOT_EQUAL;
		break;
	case HalCompareOp::GreaterEqual:
		compareOp = VK_COMPARE_OP_GREATER_OR_EQUAL;
		break;
	case HalCompareOp::AlwaysPass:
		compareOp = VK_COMPARE_OP_ALWAYS;
		break;
	default:
		compareOp = VK_COMPARE_OP_NEVER;
		break;
	}

	return compareOp;
}

VkBlendFactor 
VulkanTypeConversion::ConvertBlendFactorToVulkan(HalBlendFactor factor)
{
	VkBlendFactor blendFactor = VK_BLEND_FACTOR_ZERO;

	switch (factor)
	{
	case HalBlendFactor::Zero:
		blendFactor = VK_BLEND_FACTOR_ZERO;
		break;
	case HalBlendFactor::One:
		blendFactor = VK_BLEND_FACTOR_ONE;
		break;
	case HalBlendFactor::SrcColor:
		blendFactor = VK_BLEND_FACTOR_SRC_COLOR;
		break;
	case HalBlendFactor::OneMinusSrcColor:
		blendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_COLOR;
		break;
	case HalBlendFactor::DstColor:
		blendFactor = VK_BLEND_FACTOR_DST_COLOR;
		break;
	case HalBlendFactor::OneMinusDstColor:
		blendFactor = VK_BLEND_FACTOR_ONE_MINUS_DST_COLOR;
		break;
	case HalBlendFactor::SrcAlpha:
		blendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
		break;
	case HalBlendFactor::OneMinusSrcAlpha:
		blendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
		break;
	case HalBlendFactor::DstAlpha:
		blendFactor = VK_BLEND_FACTOR_DST_ALPHA;
		break;
	case HalBlendFactor::OneMinusDstAlpha:
		blendFactor = VK_BLEND_FACTOR_ONE_MINUS_DST_ALPHA;
		break;
	case HalBlendFactor::ConstantColor:
		blendFactor = VK_BLEND_FACTOR_CONSTANT_COLOR;
		break;
	case HalBlendFactor::OneMinusConstantColor:
		blendFactor = VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_COLOR;
		break;
	case HalBlendFactor::ConstantAlpha:
		blendFactor = VK_BLEND_FACTOR_CONSTANT_ALPHA;
		break;
	case HalBlendFactor::OneMinusConstantAlpha:
		blendFactor = VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_ALPHA;
		break;
	case HalBlendFactor::SrcAlphaSaturate:
		blendFactor = VK_BLEND_FACTOR_SRC_ALPHA_SATURATE;
		break;
	case HalBlendFactor::Src1Color:
		blendFactor = VK_BLEND_FACTOR_SRC1_COLOR;
		break;
	case HalBlendFactor::OneMinusSrc1Color:
		blendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC1_COLOR;
		break;
	case HalBlendFactor::Src1Alpha:
		blendFactor = VK_BLEND_FACTOR_SRC1_ALPHA;
		break;
	case HalBlendFactor::OneMinusSrc1Alpha:
		blendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC1_ALPHA;
		break;
	default:
		blendFactor = VK_BLEND_FACTOR_ZERO;
		assert(false);
		break;
	}

	return blendFactor;
}

VkLogicOp 
VulkanTypeConversion::ConvertLogicOpToVulkan(HalLogicOp op)
{
	VkLogicOp logicOp = VK_LOGIC_OP_CLEAR;

	switch (op)
	{
	case HalLogicOp::Clear:
		logicOp = VK_LOGIC_OP_CLEAR;
		break;
	case HalLogicOp::And:
		logicOp = VK_LOGIC_OP_AND;
		break;
	case HalLogicOp::AndReverse:
		logicOp = VK_LOGIC_OP_AND_REVERSE;
		break;
	case HalLogicOp::Copy:
		logicOp = VK_LOGIC_OP_COPY;
		break;
	case HalLogicOp::AndInverted:
		logicOp = VK_LOGIC_OP_AND_INVERTED;
		break;
	case HalLogicOp::NoOp:
		logicOp = VK_LOGIC_OP_NO_OP;
		break;
	case HalLogicOp::Xor:
		logicOp = VK_LOGIC_OP_XOR;
		break;
	case HalLogicOp::Or:
		logicOp = VK_LOGIC_OP_OR;
		break;
	case HalLogicOp::Nor:
		logicOp = VK_LOGIC_OP_NOR;
		break;
	case HalLogicOp::Equivalent:
		logicOp = VK_LOGIC_OP_EQUIVALENT;
		break;
	case HalLogicOp::Invert:
		logicOp = VK_LOGIC_OP_INVERT;
		break;
	case HalLogicOp::OrReverse:
		logicOp = VK_LOGIC_OP_OR_REVERSE;
		break;
	case HalLogicOp::CopyInverted:
		logicOp = VK_LOGIC_OP_COPY_INVERTED;
		break;
	case HalLogicOp::OrInverted:
		logicOp = VK_LOGIC_OP_OR_INVERTED;
		break;
	case HalLogicOp::Nand:
		logicOp = VK_LOGIC_OP_NAND;
		break;
	case HalLogicOp::Set:
		logicOp = VK_LOGIC_OP_SET;
		break;
	default:
		logicOp = VK_LOGIC_OP_CLEAR;
		assert(false);
		break;
	}

	return logicOp;
}

VkBlendOp 
VulkanTypeConversion::ConvertBlendOpToVulkan(HalBlendOp op)
{
	VkBlendOp blendOp = VK_BLEND_OP_ADD;

	switch (op)
	{
	case HalBlendOp::Add:
		blendOp = VK_BLEND_OP_ADD;
		break;
	case HalBlendOp::Subtract:
		blendOp = VK_BLEND_OP_SUBTRACT;
		break;
	case HalBlendOp::ReverseSubtract:
		blendOp = VK_BLEND_OP_REVERSE_SUBTRACT;
		break;
	case HalBlendOp::Min:
		blendOp = VK_BLEND_OP_MIN;
		break;
	case HalBlendOp::Max:
		blendOp = VK_BLEND_OP_MAX;
		break;
	default:
		blendOp = VK_BLEND_OP_ADD;
		assert(false);
		break;
	}

	return blendOp;
}

VkColorComponentFlags 
VulkanTypeConversion::ConvertColorComponentToVulkan(HalColorComponentFlags colorComponentFlags)
{
	VkColorComponentFlags flags = 0;

	if (colorComponentFlags & static_cast<uint32_t>(HalColorComponents::Red))
		flags |= VK_COLOR_COMPONENT_R_BIT;
	if (colorComponentFlags & static_cast<uint32_t>(HalColorComponents::Green))
		flags |= VK_COLOR_COMPONENT_G_BIT;
	if (colorComponentFlags & static_cast<uint32_t>(HalColorComponents::Blue))
		flags |= VK_COLOR_COMPONENT_B_BIT;
	if (colorComponentFlags & static_cast<uint32_t>(HalColorComponents::Alpha))
		flags |= VK_COLOR_COMPONENT_A_BIT;

	assert(flags != 0);
	return flags;
}

}
