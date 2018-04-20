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

/// @file vulkanPipelineLayout.cpp
///        Vulkan pipeline layout

#include "vulkanPipelineLayout.h"
#include "vulkanDescriptorSet.h"
#include "vulkanRenderDevice.h"
#include "vulkanConversion.h"

#include "vulkanApi.h"

#include<limits>
#include<cassert>

namespace cave
{

VulkanPipelineLayout::VulkanPipelineLayout(VulkanRenderDevice* device
		, HalDescriptorSet* descriptorSet
		, caveVector<HalPushConstantRange>& pushConstants)
	: HalPipelineLayout(device, descriptorSet)
	, _pDevice(device)
	, _vkPushConstantRange(device->GetEngineAllocator())
	, _vkPipelineLayout(VK_NULL_HANDLE)
{
	// first convert push constant ranges
	if (!pushConstants.Empty())
	{
		_vkPushConstantRange.Reserve(pushConstants.Size());
		for (size_t i = 0; i < pushConstants.Size(); ++i)
		{
			assert(pushConstants[i]._size);
			assert((pushConstants[i]._size % 4) == 0);
			assert((pushConstants[i]._offset % 4) == 0);
			VkPushConstantRange constantRange;
			constantRange.stageFlags = VulkanTypeConversion::ConvertShaderStagesToVulkan(pushConstants[i]._shaderStagesFlags);
			constantRange.offset = pushConstants[i]._offset;
			constantRange.size = pushConstants[i]._size;
			_vkPushConstantRange.Push(constantRange);
		}
	}

	// setup struct
	_vkPipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	_vkPipelineLayoutInfo.pNext = nullptr;
	_vkPipelineLayoutInfo.flags = 0;
	_vkPipelineLayoutInfo.setLayoutCount = 0;
	_vkPipelineLayoutInfo.pSetLayouts = nullptr; // setup later
	_vkPipelineLayoutInfo.pushConstantRangeCount = static_cast<uint32_t>(_vkPushConstantRange.Size());
	_vkPipelineLayoutInfo.pPushConstantRanges = (_vkPushConstantRange.Empty()) ? nullptr : _vkPushConstantRange.Data();
}

VulkanPipelineLayout::~VulkanPipelineLayout()
{
	if (!_vkPushConstantRange.Empty())
		_vkPushConstantRange.Clear();

	if (_vkPipelineLayout != VK_NULL_HANDLE)
		VulkanApi::GetApi()->vkDestroyPipelineLayout(_pDevice->GetDeviceHandle(), _vkPipelineLayout, nullptr);
}

VkPipelineLayout VulkanPipelineLayout::GetPipelineLayout()
{
	if (_vkPipelineLayout == VK_NULL_HANDLE)
	{
		// query descriptor set layouts
		if (_pDescriptorSet)
		{
			VulkanDescriptorSet* vkDescriptor = static_cast<VulkanDescriptorSet*>(_pDescriptorSet);
			caveVector<VkDescriptorSetLayout>& descriptorSets = vkDescriptor->GetDescriptorSetLayouts();

			_vkPipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(descriptorSets.Size());
			_vkPipelineLayoutInfo.pSetLayouts = (descriptorSets.Empty()) ? nullptr : descriptorSets.Data();
		}

		// create
		VulkanApi::GetApi()->vkCreatePipelineLayout(_pDevice->GetDeviceHandle(), &_vkPipelineLayoutInfo, nullptr, &_vkPipelineLayout);
		assert(_vkPipelineLayout != VK_NULL_HANDLE);
	}

	return _vkPipelineLayout;
}

}
