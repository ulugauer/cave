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

/// @file vulkanVertexInput.cpp
///        Vulkan vertex input state

#include "vulkanVertexInput.h"
#include "vulkanRenderDevice.h"
#include "vulkanApi.h"

#include<limits>

namespace cave
{


VulkanVertexInput::VulkanVertexInput(VulkanRenderDevice* device)
	: HalVertexInput()
	, _pDevice(device)
{
	// Setup a default state
	_VertexInputStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	_VertexInputStateInfo.pNext = nullptr;
	_VertexInputStateInfo.flags = 0;
	_VertexInputStateInfo.vertexBindingDescriptionCount = 0;
	_VertexInputStateInfo.pVertexBindingDescriptions = nullptr;
	_VertexInputStateInfo.vertexAttributeDescriptionCount = 0;
	_VertexInputStateInfo.pVertexAttributeDescriptions = nullptr;
}

VulkanVertexInput::~VulkanVertexInput()
{
}


}
