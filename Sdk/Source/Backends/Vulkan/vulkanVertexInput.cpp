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
#include "vulkanConversion.h"
#include "vulkanApi.h"

#include<limits>
#include <algorithm>

namespace cave
{


VulkanVertexInput::VulkanVertexInput(VulkanRenderDevice* device, HalVertexInputStateInfo& vertexInputState)
	: HalVertexInput(vertexInputState)
	, _pDevice(device)
	, _bindingsDescArray(nullptr)
	, _attributesDescArray(nullptr)
	, _bindingBase(0)
{
	// Setup a default state
	_vkVertexInputStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	_vkVertexInputStateInfo.pNext = nullptr;
	_vkVertexInputStateInfo.flags = 0;
	_vkVertexInputStateInfo.vertexBindingDescriptionCount = vertexInputState._vertexBindingDescriptionCount;
	_vkVertexInputStateInfo.vertexAttributeDescriptionCount = vertexInputState._vertexAttributeDescriptionCount;
	_vkVertexInputStateInfo.pVertexAttributeDescriptions = nullptr;

	if (vertexInputState._vertexBindingDescriptionCount)
	{
		_bindingsDescArray = AllocateArray<VkVertexInputBindingDescription>(*device->GetEngineAllocator(), vertexInputState._vertexBindingDescriptionCount);
		if (_bindingsDescArray)
		{
			for (uint32_t i = 0; i < vertexInputState._vertexBindingDescriptionCount; i++)
			{
				_bindingsDescArray[i].binding = vertexInputState._pVertexBindingDescriptions[i]._binding;
				_bindingsDescArray[i].stride = vertexInputState._pVertexBindingDescriptions[i]._stride;
				_bindingsDescArray[i].inputRate = VulkanTypeConversion::ConvertVertexInputRateToVulkan(vertexInputState._pVertexBindingDescriptions[i]._inputRate);
				_bindingBase = (std::min)(_bindingBase, _bindingsDescArray[i].binding);
			}
		}
	}

	_attributesDescArray = nullptr;
	if (vertexInputState._vertexAttributeDescriptionCount)
	{
		_attributesDescArray = AllocateArray<VkVertexInputAttributeDescription>(*device->GetEngineAllocator(), vertexInputState._vertexAttributeDescriptionCount);
		if (_attributesDescArray)
		{
			for (uint32_t i = 0; i < vertexInputState._vertexAttributeDescriptionCount; i++)
			{
				_attributesDescArray[i].location = vertexInputState._pVertexAttributeDescriptions[i]._location;
				_attributesDescArray[i].binding = vertexInputState._pVertexAttributeDescriptions[i]._binding;
				_attributesDescArray[i].format = VulkanTypeConversion::ConvertImageFormatToVulkan(vertexInputState._pVertexAttributeDescriptions[i]._format);
				_attributesDescArray[i].offset = vertexInputState._pVertexAttributeDescriptions[i]._offset;
			}
		}
	}

	_vkVertexInputStateInfo.pVertexBindingDescriptions = _bindingsDescArray;
	_vkVertexInputStateInfo.pVertexAttributeDescriptions = _attributesDescArray;
}

VulkanVertexInput::~VulkanVertexInput()
{
	if (_bindingsDescArray)
		DeallocateArray<VkVertexInputBindingDescription>(*_pDevice->GetEngineAllocator(), _bindingsDescArray);
	if (_attributesDescArray)
		DeallocateArray<VkVertexInputAttributeDescription>(*_pDevice->GetEngineAllocator(), _attributesDescArray);
}

}
