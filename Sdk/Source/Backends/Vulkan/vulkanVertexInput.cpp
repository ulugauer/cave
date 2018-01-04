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

namespace cave
{


VulkanVertexInput::VulkanVertexInput(VulkanRenderDevice* device, HalVertexInputStateInfo& vertexInputState)
	: HalVertexInput(vertexInputState)
	, _pDevice(device)
{
	// Setup a default state
	_vkVertexInputStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	_vkVertexInputStateInfo.pNext = nullptr;
	_vkVertexInputStateInfo.flags = 0;
	_vkVertexInputStateInfo.vertexBindingDescriptionCount = vertexInputState._vertexBindingDescriptionCount;
	_vkVertexInputStateInfo.vertexAttributeDescriptionCount = vertexInputState._vertexAttributeDescriptionCount;
	_vkVertexInputStateInfo.pVertexAttributeDescriptions = nullptr;

	VkVertexInputBindingDescription* bindingsArray = nullptr;
	if (vertexInputState._vertexBindingDescriptionCount)
	{
		bindingsArray = AllocateArray<VkVertexInputBindingDescription>(*device->GetEngineAllocator(), vertexInputState._vertexBindingDescriptionCount);
		if (bindingsArray)
		{
			for (uint32_t i = 0; i < vertexInputState._vertexBindingDescriptionCount; i++)
			{
				bindingsArray[i].binding = vertexInputState._pVertexBindingDescriptions[i]._binding;
				bindingsArray[i].stride = vertexInputState._pVertexBindingDescriptions[i]._stride;
				bindingsArray[i].inputRate = VulkanTypeConversion::ConvertVertexInputRateToVulkan(vertexInputState._pVertexBindingDescriptions[i]._inputRate);
			}
		}
	}

	VkVertexInputAttributeDescription* attributesArray = nullptr;
	if (vertexInputState._vertexAttributeDescriptionCount)
	{
		attributesArray = AllocateArray<VkVertexInputAttributeDescription>(*device->GetEngineAllocator(), vertexInputState._vertexAttributeDescriptionCount);
		if (attributesArray)
		{
			for (uint32_t i = 0; i < vertexInputState._vertexBindingDescriptionCount; i++)
			{
				attributesArray[i].location = vertexInputState._pVertexAttributeDescriptions[i]._location;
				attributesArray[i].binding = vertexInputState._pVertexAttributeDescriptions[i]._binding;
				attributesArray[i].format = VulkanTypeConversion::ConvertImageFormatToVulkan(vertexInputState._pVertexAttributeDescriptions[i]._format);
				attributesArray[i].offset = vertexInputState._pVertexAttributeDescriptions[i]._offset;
			}
		}
	}

	_vkVertexInputStateInfo.pVertexBindingDescriptions = bindingsArray;
	_vkVertexInputStateInfo.pVertexAttributeDescriptions = attributesArray;
}

VulkanVertexInput::~VulkanVertexInput()
{
	if (_vkVertexInputStateInfo.pVertexBindingDescriptions)
		DeallocateDelete(*_pDevice->GetEngineAllocator(), _vkVertexInputStateInfo.pVertexBindingDescriptions);
	if (_vkVertexInputStateInfo.pVertexAttributeDescriptions)
		DeallocateDelete(*_pDevice->GetEngineAllocator(), _vkVertexInputStateInfo.pVertexAttributeDescriptions);
}

}
