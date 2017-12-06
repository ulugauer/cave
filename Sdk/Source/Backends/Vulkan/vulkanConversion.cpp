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

}
