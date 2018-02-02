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
		assert(false);
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
	case HalImageFormat::B8G8R8A8UNorm:
		vkImageFormat = VK_FORMAT_B8G8R8A8_UNORM;
		break;
	case HalImageFormat::B8G8R8A8SNorm:
		vkImageFormat = VK_FORMAT_B8G8R8A8_SRGB;
		break;
	case HalImageFormat::R32G32SFloat:
		vkImageFormat = VK_FORMAT_R32G32_SFLOAT;
		break;
	case HalImageFormat::R32G32B32SFloat:
		vkImageFormat = VK_FORMAT_R32G32B32_SFLOAT;
		break;
	default:
		vkImageFormat = VK_FORMAT_UNDEFINED;
		assert(false);
		break;
	}

	return vkImageFormat;
}

HalImageFormat 
VulkanTypeConversion::ConvertImageFormatFromVulkan(VkFormat imageFormat)
{
	HalImageFormat format = HalImageFormat::Undefined;
	
	switch (imageFormat)
	{
	case VK_FORMAT_R8G8B8A8_UNORM:
		format = HalImageFormat::R8G8B8A8UNorm;
		break;
	case VK_FORMAT_R8G8B8A8_SNORM:
		format = HalImageFormat::R8G8B8A8SNorm;
		break;
	case VK_FORMAT_B8G8R8A8_UNORM:
		format = HalImageFormat::B8G8R8A8UNorm;
		break;
	case VK_FORMAT_B8G8R8A8_SRGB:
		format = HalImageFormat::B8G8R8A8SNorm;
		break;
	case VK_FORMAT_R32G32_SFLOAT:
		format = HalImageFormat::R32G32SFloat;
		break;
	case VK_FORMAT_R32G32B32_SFLOAT:
		format = HalImageFormat::R32G32B32SFloat;
		break;
	default:
		format = HalImageFormat::Undefined;
		assert(false);
		break;
	}

	return format;
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
		assert(false);
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
		assert(false);
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
		assert(false);
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
		assert(false);
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
		assert(false);
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
		assert(false);
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

VkDynamicState 
VulkanTypeConversion::ConvertDynamicStateToVulkan(HalDynamicStates state)
{
	VkDynamicState dynamicState = VK_DYNAMIC_STATE_VIEWPORT;

	switch (state)
	{
	case HalDynamicStates::Viewport:
		dynamicState = VK_DYNAMIC_STATE_VIEWPORT;
		break;
	case HalDynamicStates::Scissor:
		dynamicState = VK_DYNAMIC_STATE_SCISSOR;
		break;
	case HalDynamicStates::LineWidth:
		dynamicState = VK_DYNAMIC_STATE_LINE_WIDTH;
		break;
	case HalDynamicStates::DepthBias:
		dynamicState = VK_DYNAMIC_STATE_DEPTH_BIAS;
		break;
	case HalDynamicStates::BlendConstants:
		dynamicState = VK_DYNAMIC_STATE_BLEND_CONSTANTS;
		break;
	case HalDynamicStates::DepthBounds:
		dynamicState = VK_DYNAMIC_STATE_DEPTH_BOUNDS;
		break;
	case HalDynamicStates::StencilCompareMask:
		dynamicState = VK_DYNAMIC_STATE_STENCIL_COMPARE_MASK;
		break;
	case HalDynamicStates::StencilWriteMask:
		dynamicState = VK_DYNAMIC_STATE_STENCIL_WRITE_MASK;
		break;
	case HalDynamicStates::StencilReference:
		dynamicState = VK_DYNAMIC_STATE_STENCIL_REFERENCE;
		break;
	default:
		dynamicState = VK_DYNAMIC_STATE_VIEWPORT;
		assert(false);
		break;
	}

	return dynamicState;
}

VkShaderStageFlags 
VulkanTypeConversion::ConvertBlendOpToVulkan(HalShaderStagesFlags flags)
{
	VkShaderStageFlags shaderStageFlags = 0;

	if (flags & static_cast<uint32_t>(HalShaderStages::Vertex))
		shaderStageFlags |= VK_SHADER_STAGE_VERTEX_BIT;
	if (flags & static_cast<uint32_t>(HalShaderStages::TessellationControl))
		shaderStageFlags |= VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
	if (flags & static_cast<uint32_t>(HalShaderStages::TessellationEvaluation))
		shaderStageFlags |= VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;
	if (flags & static_cast<uint32_t>(HalShaderStages::Geometry))
		shaderStageFlags |= VK_SHADER_STAGE_GEOMETRY_BIT;
	if (flags & static_cast<uint32_t>(HalShaderStages::Fragment))
		shaderStageFlags |= VK_SHADER_STAGE_FRAGMENT_BIT;
	if (flags & static_cast<uint32_t>(HalShaderStages::Compute))
		shaderStageFlags |= VK_SHADER_STAGE_COMPUTE_BIT;

	assert(shaderStageFlags);
	return shaderStageFlags;
}

VkAttachmentLoadOp 
VulkanTypeConversion::ConvertAttachmentLoadOpToVulkan(HalAttachmentLoadOperation loadOp)
{
	VkAttachmentLoadOp vkLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;

	switch (loadOp)
	{
	case HalAttachmentLoadOperation::Clear:
		vkLoadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		break;
	case HalAttachmentLoadOperation::DontCare:
		vkLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		break;
	case HalAttachmentLoadOperation::Load:
		vkLoadOp = VK_ATTACHMENT_LOAD_OP_LOAD;
		break;
	default:
		vkLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		assert(false);
		break;
	}

	return vkLoadOp;
}

VkAttachmentStoreOp 
VulkanTypeConversion::ConvertAttachmentStoreOpToVulkan(HalAttachmentStoreOperation storeOp)
{
	VkAttachmentStoreOp vkStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;

	switch (storeOp)
	{
	case HalAttachmentStoreOperation::DontCare:
		vkStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		break;
	case HalAttachmentStoreOperation::Store:
		vkStoreOp = VK_ATTACHMENT_STORE_OP_STORE;
		break;
	default:
		vkStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		assert(false);
		break;
	}

	return vkStoreOp;
}

VkImageLayout 
VulkanTypeConversion::ConvertImageLayoutToVulkan(HalImageLayout imageLayout)
{
	VkImageLayout vkImageLayout = VK_IMAGE_LAYOUT_UNDEFINED;

	switch (imageLayout)
	{
	case HalImageLayout::Undefined:
		vkImageLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		break;
	case HalImageLayout::General:
		vkImageLayout = VK_IMAGE_LAYOUT_GENERAL;
		break;
	case HalImageLayout::ColorAttachment:
		vkImageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
		break;
	case HalImageLayout::DepthStencilAttachment:
		vkImageLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
		break;
	case HalImageLayout::DepthStencilReadOnly:
		vkImageLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL;
		break;
	case HalImageLayout::ShaderReadOnly:
		vkImageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		break;
	case HalImageLayout::TransferSrc:
		vkImageLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
		break;
	case HalImageLayout::TransferDst:
		vkImageLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
		break;
	case HalImageLayout::Preinitialized:
		vkImageLayout = VK_IMAGE_LAYOUT_PREINITIALIZED;
		break;
	case HalImageLayout::PresentSrcKHR:
		vkImageLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
		break;
	case HalImageLayout::SharedPresentSrcKHR:
		vkImageLayout = VK_IMAGE_LAYOUT_SHARED_PRESENT_KHR;
		break;
	default:
		vkImageLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		assert(false);
		break;
	}

	return vkImageLayout;
}

VkCommandPoolCreateFlags 
VulkanTypeConversion::ConvertCommandPoolUsageFlagsToVulkan(HalCommandPoolUsageFlags usageFlags)
{
	VkCommandPoolCreateFlags createFlags = 0;

	if (usageFlags & static_cast<uint32_t>(HalCommandPoolUsage::CreateTransient))
		createFlags |= VK_COMMAND_POOL_CREATE_TRANSIENT_BIT;
	if (usageFlags & static_cast<uint32_t>(HalCommandPoolUsage::ResetCommandBuffer))
		createFlags |= VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

	return createFlags;
}

VkCommandBufferLevel 
VulkanTypeConversion::ConvertCommandBufferLevelToVulkan(HalCommandBufferLevel level)
{
	VkCommandBufferLevel vkLevel = VK_COMMAND_BUFFER_LEVEL_PRIMARY;

	switch (level)
	{
	case HalCommandBufferLevel::PrimaryLevel:
		vkLevel = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		break;
	case HalCommandBufferLevel::SecondaryLevel:
		vkLevel = VK_COMMAND_BUFFER_LEVEL_SECONDARY;
		break;
	default:
		vkLevel = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		assert(false);
		break;
	}

	return vkLevel;
}

VkCommandBufferUsageFlags 
VulkanTypeConversion::ConvertCommandBufferUsageFlagsToVulkan(HalCommandBufferUsageFlags usageFlags)
{
	VkCommandBufferUsageFlags vkUsageFlags = 0;

	if (usageFlags & static_cast<uint32_t>(HalCommandBufferUsage::OneTimeSubmit))
		vkUsageFlags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
	if (usageFlags & static_cast<uint32_t>(HalCommandBufferUsage::RenderPassContinue))
		vkUsageFlags |= VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT;
	if (usageFlags & static_cast<uint32_t>(HalCommandBufferUsage::SimultaneousUse))
		vkUsageFlags |= VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;

	return vkUsageFlags;
}

VkSubpassContents 
VulkanTypeConversion::ConvertSubpassContentToVulkan(HalSubpassContents content)
{
	VkSubpassContents vkContent = VK_SUBPASS_CONTENTS_INLINE;

	switch (content)
	{
	case HalSubpassContents::Inline:
		vkContent = VK_SUBPASS_CONTENTS_INLINE;
		break;
	case HalSubpassContents::SecondarCommandBuffer:
		vkContent = VK_SUBPASS_CONTENTS_SECONDARY_COMMAND_BUFFERS;
		break;
	default:
		assert(false);
		vkContent = VK_SUBPASS_CONTENTS_INLINE;
		break;
	}

	return vkContent;
}

VkDependencyFlags 
VulkanTypeConversion::ConvertDependencyFlagsToVulkan(HalDependencyFlags dependency)
{
	VkDependencyFlags vkFlags = 0;
	if (dependency & static_cast<uint32_t>(HalDependencyBits::DependencyByRegion))
		vkFlags |= VK_DEPENDENCY_BY_REGION_BIT;

	return vkFlags;
}

VkPipelineStageFlags 
VulkanTypeConversion::ConvertPipelineFlagsToVulkan(HalPipelineStageFlags pipelineStage)
{
	VkPipelineStageFlags vkFlags = 0;

	if (pipelineStage & static_cast<uint32_t>(HalPipelineStageBits::TopOfPipe))
		vkFlags |= VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
	if (pipelineStage & static_cast<uint32_t>(HalPipelineStageBits::DrawIndirect))
		vkFlags |= VK_PIPELINE_STAGE_DRAW_INDIRECT_BIT;
	if (pipelineStage & static_cast<uint32_t>(HalPipelineStageBits::VertexInput))
		vkFlags |= VK_PIPELINE_STAGE_VERTEX_INPUT_BIT;
	if (pipelineStage & static_cast<uint32_t>(HalPipelineStageBits::DrawIndirect))
		vkFlags |= VK_PIPELINE_STAGE_DRAW_INDIRECT_BIT;
	if (pipelineStage & static_cast<uint32_t>(HalPipelineStageBits::VertexShader))
		vkFlags |= VK_PIPELINE_STAGE_VERTEX_SHADER_BIT;
	if (pipelineStage & static_cast<uint32_t>(HalPipelineStageBits::TessellationControlShader))
		vkFlags |= VK_PIPELINE_STAGE_TESSELLATION_CONTROL_SHADER_BIT;
	if (pipelineStage & static_cast<uint32_t>(HalPipelineStageBits::TessellationEvaluationShader))
		vkFlags |= VK_PIPELINE_STAGE_TESSELLATION_EVALUATION_SHADER_BIT;
	if (pipelineStage & static_cast<uint32_t>(HalPipelineStageBits::GeometryShader))
		vkFlags |= VK_PIPELINE_STAGE_GEOMETRY_SHADER_BIT;
	if (pipelineStage & static_cast<uint32_t>(HalPipelineStageBits::FragmentShader))
		vkFlags |= VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
	if (pipelineStage & static_cast<uint32_t>(HalPipelineStageBits::EarlyFragmentTests))
		vkFlags |= VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
	if (pipelineStage & static_cast<uint32_t>(HalPipelineStageBits::LateFragmentTests))
		vkFlags |= VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
	if (pipelineStage & static_cast<uint32_t>(HalPipelineStageBits::ColorAttachmentOutput))
		vkFlags |= VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	if (pipelineStage & static_cast<uint32_t>(HalPipelineStageBits::ComputeShader))
		vkFlags |= VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT;
	if (pipelineStage & static_cast<uint32_t>(HalPipelineStageBits::Transfer))
		vkFlags |= VK_PIPELINE_STAGE_TRANSFER_BIT;
	if (pipelineStage & static_cast<uint32_t>(HalPipelineStageBits::BottomOfPipe))
		vkFlags |= VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
	if (pipelineStage & static_cast<uint32_t>(HalPipelineStageBits::Host))
		vkFlags |= VK_PIPELINE_STAGE_HOST_BIT;

	return vkFlags;
}

VkAccessFlags 
VulkanTypeConversion::ConvertAccessFlagsToVulkan(HalAccessFlags access)
{
	VkAccessFlags vkFlags = 0;

	if (access & static_cast<uint32_t>(HalAccessBits::IndirectCommandRead))
		vkFlags |= VK_ACCESS_INDIRECT_COMMAND_READ_BIT;
	if (access & static_cast<uint32_t>(HalAccessBits::IndexRead))
		vkFlags |= VK_ACCESS_INDEX_READ_BIT;
	if (access & static_cast<uint32_t>(HalAccessBits::VertexAttributeRead))
		vkFlags |= VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT;
	if (access & static_cast<uint32_t>(HalAccessBits::UniformRead))
		vkFlags |= VK_ACCESS_UNIFORM_READ_BIT;
	if (access & static_cast<uint32_t>(HalAccessBits::InputAttachmentRead))
		vkFlags |= VK_ACCESS_INPUT_ATTACHMENT_READ_BIT;
	if (access & static_cast<uint32_t>(HalAccessBits::ShaderRead))
		vkFlags |= VK_ACCESS_SHADER_READ_BIT;
	if (access & static_cast<uint32_t>(HalAccessBits::ShaderWrite))
		vkFlags |= VK_ACCESS_SHADER_WRITE_BIT;
	if (access & static_cast<uint32_t>(HalAccessBits::ColorAttachmentRead))
		vkFlags |= VK_ACCESS_COLOR_ATTACHMENT_READ_BIT;
	if (access & static_cast<uint32_t>(HalAccessBits::ColorAttachmentWrite))
		vkFlags |= VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
	if (access & static_cast<uint32_t>(HalAccessBits::DepthStencilAttachmentRead))
		vkFlags |= VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT;
	if (access & static_cast<uint32_t>(HalAccessBits::DepthStencilAttachmentWrite))
		vkFlags |= VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
	if (access & static_cast<uint32_t>(HalAccessBits::TransferRead))
		vkFlags |= VK_ACCESS_TRANSFER_READ_BIT;
	if (access & static_cast<uint32_t>(HalAccessBits::TransferWrite))
		vkFlags |= VK_ACCESS_TRANSFER_WRITE_BIT;
	if (access & static_cast<uint32_t>(HalAccessBits::HostRead))
		vkFlags |= VK_ACCESS_HOST_READ_BIT;
	if (access & static_cast<uint32_t>(HalAccessBits::HostWrite))
		vkFlags |= VK_ACCESS_HOST_WRITE_BIT;
	if (access & static_cast<uint32_t>(HalAccessBits::MemoryRead))
		vkFlags |= VK_ACCESS_MEMORY_READ_BIT;
	if (access & static_cast<uint32_t>(HalAccessBits::MemoryWrite))
		vkFlags |= VK_ACCESS_MEMORY_WRITE_BIT;

	return vkFlags;
}

VkBufferCreateFlags 
VulkanTypeConversion::ConvertBufferCreateFlagsToVulkan(HalBufferCreateFlags create)
{
	VkBufferCreateFlags vkFlags = 0;

	if (create & static_cast<uint32_t>(HalBufferCreate::SparseBinding))
		vkFlags |= VK_BUFFER_CREATE_SPARSE_BINDING_BIT;
	if (create & static_cast<uint32_t>(HalBufferCreate::SparseResidency))
		vkFlags |= VK_BUFFER_CREATE_SPARSE_RESIDENCY_BIT;
	if (create & static_cast<uint32_t>(HalBufferCreate::SparseAliased))
		vkFlags |= VK_BUFFER_CREATE_SPARSE_ALIASED_BIT;

	return vkFlags;
}

VkBufferUsageFlags 
VulkanTypeConversion::ConvertBufferUsageFlagsToVulkan(HalBufferUsageFlags usage)
{
	VkBufferUsageFlags vkFlags = 0;

	if (usage & static_cast<uint32_t>(HalBufferUsage::TransferSource))
		vkFlags |= VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
	if (usage & static_cast<uint32_t>(HalBufferUsage::TransferDest))
		vkFlags |= VK_BUFFER_USAGE_TRANSFER_DST_BIT;
	if (usage & static_cast<uint32_t>(HalBufferUsage::UniformTexelBuffer))
		vkFlags |= VK_BUFFER_USAGE_UNIFORM_TEXEL_BUFFER_BIT;
	if (usage & static_cast<uint32_t>(HalBufferUsage::StorageTexelBuffer))
		vkFlags |= VK_BUFFER_USAGE_STORAGE_TEXEL_BUFFER_BIT;
	if (usage & static_cast<uint32_t>(HalBufferUsage::UniformBuffer))
		vkFlags |= VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
	if (usage & static_cast<uint32_t>(HalBufferUsage::StorageBuffer))
		vkFlags |= VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
	if (usage & static_cast<uint32_t>(HalBufferUsage::IndexBuffer))
		vkFlags |= VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
	if (usage & static_cast<uint32_t>(HalBufferUsage::VertexBuffer))
		vkFlags |= VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
	if (usage & static_cast<uint32_t>(HalBufferUsage::IndirectBuffer))
		vkFlags |= VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT;

	assert(vkFlags);	// assert if zero
	return vkFlags;
}

VkSharingMode  
VulkanTypeConversion::ConvertSharedModeFlagsToVulkan(HalBufferShareMode mode)
{
	VkSharingMode vkMode = VK_SHARING_MODE_EXCLUSIVE;

	if (mode == HalBufferShareMode::Comcurrent)
		vkMode = VK_SHARING_MODE_CONCURRENT;

	return vkMode;
}

VkVertexInputRate 
VulkanTypeConversion::ConvertVertexInputRateToVulkan(HalVertexInputRate rate)
{
	VkVertexInputRate vkRate = VK_VERTEX_INPUT_RATE_VERTEX;

	if (rate == HalVertexInputRate::Instance)
		vkRate = VK_VERTEX_INPUT_RATE_INSTANCE;

	return vkRate;
}

VkMemoryPropertyFlags
VulkanTypeConversion::ConvertMemoryPropertiesToVulkan(HalMemoryPropertyFlags properties)
{
	VkMemoryPropertyFlags vkFlags = 0;

	if (properties & static_cast<uint32_t>(HalMemoryPropertyBits::DeviceLocal))
		vkFlags |= VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
	if (properties & static_cast<uint32_t>(HalMemoryPropertyBits::HostVisible))
		vkFlags |= VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;
	if (properties & static_cast<uint32_t>(HalMemoryPropertyBits::HostCoherent))
		vkFlags |= VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
	if (properties & static_cast<uint32_t>(HalMemoryPropertyBits::HostCached))
		vkFlags |= VK_MEMORY_PROPERTY_HOST_CACHED_BIT;
	if (properties & static_cast<uint32_t>(HalMemoryPropertyBits::LazilyAllocated))
		vkFlags |= VK_MEMORY_PROPERTY_LAZILY_ALLOCATED_BIT;
	
	return vkFlags;
}

VkIndexType 
VulkanTypeConversion::ConvertIndexTypeToVulkan(HalIndexType indexType)
{
	VkIndexType vkIndexType;

	switch (indexType)
	{
	case HalIndexType::UInt16:
		vkIndexType = VK_INDEX_TYPE_UINT16;
		break;
	case  HalIndexType::UInt32:
		vkIndexType = VK_INDEX_TYPE_UINT32;
		break;
	default:
		vkIndexType = VK_INDEX_TYPE_UINT16;
		assert(false);
		break;
	}

	return vkIndexType;
}

}
