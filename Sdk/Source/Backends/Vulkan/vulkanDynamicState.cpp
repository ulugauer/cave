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

/// @file vulkanDynamicState.cpp
///        Vulkan dynamic pipeline state

#include "vulkanDynamicState.h"
#include "vulkanRenderDevice.h"
#include "vulkanApi.h"

#include<limits>

namespace cave
{

/**
* @brief Convert from generic dynamic state to vulkan bdynamic state
*
* @param[in] op		Generic bdynamic state
*
* @return Vulkan dynamic state value
*/
static VkDynamicState ConvertDynamicStateToVulkan(DynamicState state)
{
	VkDynamicState dynamicState = VK_DYNAMIC_STATE_VIEWPORT;

	switch (state)
	{
	case DynamicState::Viewport:
		dynamicState = VK_DYNAMIC_STATE_VIEWPORT;
		break;
	case DynamicState::Scissor:
		dynamicState = VK_DYNAMIC_STATE_SCISSOR;
		break;
	case DynamicState::LineWidth:
		dynamicState = VK_DYNAMIC_STATE_LINE_WIDTH;
		break;
	case DynamicState::DepthBias:
		dynamicState = VK_DYNAMIC_STATE_DEPTH_BIAS;
		break;
	case DynamicState::BlendConstants:
		dynamicState = VK_DYNAMIC_STATE_BLEND_CONSTANTS;
		break;
	case DynamicState::DepthBounds:
		dynamicState = VK_DYNAMIC_STATE_DEPTH_BOUNDS;
		break;
	case DynamicState::StencilCompareMask:
		dynamicState = VK_DYNAMIC_STATE_STENCIL_COMPARE_MASK;
		break;
	case DynamicState::StencilWriteMask:
		dynamicState = VK_DYNAMIC_STATE_STENCIL_WRITE_MASK;
		break;
	case DynamicState::StencilReference:
		dynamicState = VK_DYNAMIC_STATE_STENCIL_REFERENCE;
		break;
	default:
		dynamicState = VK_DYNAMIC_STATE_VIEWPORT;
		break;
	}

	return dynamicState;
}

VulkanDynamicState::VulkanDynamicState(VulkanRenderDevice* device, caveVector<DynamicState>& dynamicStates)
	: HalDynamicState(device, dynamicStates)
	, _pDevice(device)
	, _dynamicStates(device->GetEngineAllocator())
{
	// first convert attachments
	if (!dynamicStates.Empty())
	{
		_dynamicStates.Reserve(dynamicStates.Size());
		for (size_t i = 0; i < dynamicStates.Size(); ++i)
		{
			VkDynamicState vkDynamicState = ConvertDynamicStateToVulkan(dynamicStates[i]);
			_dynamicStates.Push(vkDynamicState);
		}
	}
	// Setup state
	_dynamicStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	_dynamicStateInfo.pNext = nullptr;
	_dynamicStateInfo.flags = 0;
	_dynamicStateInfo.dynamicStateCount = static_cast<uint32_t>(_dynamicStates.Size());
	_dynamicStateInfo.pDynamicStates = _dynamicStates.Data();
}

VulkanDynamicState::~VulkanDynamicState()
{
	if (!_dynamicStates.Empty())
		_dynamicStates.Clear();
}

}
