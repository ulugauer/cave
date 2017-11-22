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

/// @file vulkanRasterizerState.cpp
///        Vulkan rasterizer state

#include "vulkanRasterizerState.h"
#include "vulkanRenderDevice.h"
#include "vulkanApi.h"

#include<limits>

namespace cave
{

/**
* @brief Convert from generic cull mode to vulkan cull mode
*
* @param[in] mode	Generic cull mode
*
* @return Vulkan cull mode
*/
static VkCullModeFlagBits ConvertCullModeToVulkan(CullMode mode)
{
	VkCullModeFlagBits modeFlag = VK_CULL_MODE_NONE;

	switch (mode)
	{
	case CullMode::Front:
		modeFlag = VK_CULL_MODE_FRONT_BIT;
		break;
	case CullMode::Back:
		modeFlag = VK_CULL_MODE_BACK_BIT;
		break;
	case CullMode::FrontAndBack:
		modeFlag = VK_CULL_MODE_FRONT_AND_BACK;
		break;
	case CullMode::None:
		modeFlag = VK_CULL_MODE_NONE;
		break;
	default:
		modeFlag = VK_CULL_MODE_NONE;
		break;
	}

	return modeFlag;
}

/**
* @brief Convert from generic front face to vulkan front face
*
* @param[in] face	Generic front face value
*
* @return Vulkan front face
*/
static VkFrontFace ConvertFrontFaceToVulkan(FrontFace face)
{
	VkFrontFace frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;

	switch (face)
	{
	case FrontFace::CounterClockwise:
		frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
		break;
	case FrontFace::Clockwise:
		frontFace = VK_FRONT_FACE_CLOCKWISE;
		break;
	default:
		frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
		break;
	}

	return frontFace;
}

/**
* @brief Convert from generic polygon mode to vulkan polygon mode
*
* @param[in] face	Generic polygon mode
*
* @return Vulkan polygon mode
*/
static VkPolygonMode ConvertPolygonModeToVulkan(PolygonMode mode)
{
	VkPolygonMode polyMode = VK_POLYGON_MODE_FILL;

	switch (mode)
	{
	case PolygonMode::Fill:
		polyMode = VK_POLYGON_MODE_FILL;
		break;
	case PolygonMode::Line:
		polyMode = VK_POLYGON_MODE_LINE;
		break;
	case PolygonMode::Point:
		polyMode = VK_POLYGON_MODE_POINT;
		break;
	default:
		polyMode = VK_POLYGON_MODE_FILL;
		break;
	}

	return polyMode;
}

VulkanRasterizerState::VulkanRasterizerState(VulkanRenderDevice* device, HalRasterizerData& rasterizerData)
	: HalRasterizerState(rasterizerData)
	, _pDevice(device)
{
	_rasterizerStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	_rasterizerStateInfo.pNext = nullptr;
	_rasterizerStateInfo.flags = 0;
	_rasterizerStateInfo.cullMode = ConvertCullModeToVulkan(rasterizerData._cullMode);
	_rasterizerStateInfo.depthBiasClamp = rasterizerData._depthBiasClamp;
	_rasterizerStateInfo.depthBiasConstantFactor = rasterizerData._depthBiasConstantFactor;
	_rasterizerStateInfo.depthBiasEnable = rasterizerData._depthBiasEnable;
	_rasterizerStateInfo.depthBiasSlopeFactor = rasterizerData._depthBiasSlopeFactor;
	_rasterizerStateInfo.depthClampEnable = rasterizerData._depthClampEnable;
	_rasterizerStateInfo.frontFace = ConvertFrontFaceToVulkan(rasterizerData._frontFace);
	_rasterizerStateInfo.lineWidth = rasterizerData._lineWidth;
	_rasterizerStateInfo.polygonMode = ConvertPolygonModeToVulkan(rasterizerData._polygonMode);
	_rasterizerStateInfo.rasterizerDiscardEnable = rasterizerData._rasterizerDiscardEnable;
}

VulkanRasterizerState::~VulkanRasterizerState()
{
}


}
