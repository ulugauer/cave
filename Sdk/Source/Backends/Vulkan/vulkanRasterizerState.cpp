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
#include "vulkanConversion.h"
#include "vulkanApi.h"

#include<limits>

namespace cave
{

VulkanRasterizerState::VulkanRasterizerState(VulkanRenderDevice* device, HalRasterizerSetup& rasterizerState)
	: HalRasterizerState(rasterizerState)
	, _pDevice(device)
{
	_rasterizerStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	_rasterizerStateInfo.pNext = nullptr;
	_rasterizerStateInfo.flags = 0;
	_rasterizerStateInfo.cullMode = VulkanTypeConversion::ConvertCullModeToVulkan(rasterizerState._cullMode);
	_rasterizerStateInfo.depthBiasClamp = rasterizerState._depthBiasClamp;
	_rasterizerStateInfo.depthBiasConstantFactor = rasterizerState._depthBiasConstantFactor;
	_rasterizerStateInfo.depthBiasEnable = rasterizerState._depthBiasEnable;
	_rasterizerStateInfo.depthBiasSlopeFactor = rasterizerState._depthBiasSlopeFactor;
	_rasterizerStateInfo.depthClampEnable = rasterizerState._depthClampEnable;
	_rasterizerStateInfo.frontFace = VulkanTypeConversion::ConvertFrontFaceToVulkan(rasterizerState._frontFace);
	_rasterizerStateInfo.lineWidth = rasterizerState._lineWidth;
	_rasterizerStateInfo.polygonMode = VulkanTypeConversion::ConvertPolygonModeToVulkan(rasterizerState._polygonMode);
	_rasterizerStateInfo.rasterizerDiscardEnable = rasterizerState._rasterizerDiscardEnable;
}

VulkanRasterizerState::~VulkanRasterizerState()
{
}


}
