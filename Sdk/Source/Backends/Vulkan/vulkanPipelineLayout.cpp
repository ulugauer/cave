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
#include "vulkanRenderDevice.h"
#include "vulkanConversion.h"

#include "vulkanApi.h"

#include<limits>
#include<cassert>

namespace cave
{

VulkanPipelineLayout::VulkanPipelineLayout(VulkanRenderDevice* device
		, caveVector<HalDescriptorSetLayout>& descriptorSetLayouts
		, caveVector<HalPushConstantRange>& pushConstants)
	: HalPipelineLayout(device)
	, _pDevice(device)
	, _vkDescriptorSetLayoutsInfo(device->GetEngineAllocator())
	, _vkDescriptorSetLayouts(device->GetEngineAllocator())
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
			constantRange.stageFlags = VulkanTypeConversion::ConvertBlendOpToVulkan(pushConstants[i]._shaderStagesFlags);
			constantRange.offset = pushConstants[i]._offset;
			constantRange.size = pushConstants[i]._size;
			_vkPushConstantRange.Push(constantRange);
		}
	}

	// first convert descriptor layouts
	for (size_t l = 0; l < descriptorSetLayouts.Size(); ++l)
	{
		_vkDescriptorSetLayoutsInfo.Reserve(descriptorSetLayouts.Size());
		HalDescriptorSetLayout& descriptorSetLayout = descriptorSetLayouts[l];

		VkDescriptorSetLayoutCreateInfo descriptorSetLayoutCreateInfo;
		descriptorSetLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		descriptorSetLayoutCreateInfo.pNext = nullptr;
		descriptorSetLayoutCreateInfo.flags = 0;
		descriptorSetLayoutCreateInfo.bindingCount = descriptorSetLayout._bindingCount;
		// allocte memory
		size_t descriptorSetBindingsArraySize = descriptorSetLayout._bindingCount * sizeof(VkDescriptorSetLayoutBinding);
		VkDescriptorSetLayoutBinding *descriptorBindings = static_cast<VkDescriptorSetLayoutBinding*>(device->GetEngineAllocator()->Allocate(descriptorSetBindingsArraySize, 4));

		if (descriptorBindings)
		{
			for (size_t i = 0; i < descriptorSetLayout._bindingCount; ++i)
			{
				//_vkDescriptorSetLayouts.Push(descriptorSetLayoutCreateInfo);
				const HalDescriptorSetLayoutBinding& layoutBinding = descriptorSetLayout._pBindings[i];
				VkDescriptorSetLayoutBinding& vkLayoutBinidng = descriptorBindings[i];
				
				vkLayoutBinidng.binding = layoutBinding._binding;
				vkLayoutBinidng.descriptorCount = layoutBinding._descriptorCount;
				vkLayoutBinidng.pImmutableSamplers = nullptr; // not supported yet
				vkLayoutBinidng.descriptorType = VulkanTypeConversion::ConvertDescriptorTypeToVulkan(layoutBinding._descriptorType);
				vkLayoutBinidng.stageFlags = VulkanTypeConversion::ConvertBlendOpToVulkan(layoutBinding._stageFlags);
			}

			descriptorSetLayoutCreateInfo.pBindings = descriptorBindings;
			_vkDescriptorSetLayoutsInfo.Push(descriptorSetLayoutCreateInfo);
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
	if (!_vkDescriptorSetLayoutsInfo.Empty())
		_vkDescriptorSetLayoutsInfo.Clear();

	if (!_vkDescriptorSetLayouts.Empty())
	{
		for (size_t i = 0; i < _vkDescriptorSetLayouts.Size(); ++i)
		{
			if (_vkDescriptorSetLayouts[i] != VK_NULL_HANDLE)
				VulkanApi::GetApi()->vkDestroyDescriptorSetLayout(_pDevice->GetDeviceHandle(), _vkDescriptorSetLayouts[i], nullptr);
		}
		_vkDescriptorSetLayouts.Clear();
	}

	if (_vkPipelineLayout != VK_NULL_HANDLE)
		VulkanApi::GetApi()->vkDestroyPipelineLayout(_pDevice->GetDeviceHandle(), _vkPipelineLayout, nullptr);
}

VkPipelineLayout VulkanPipelineLayout::GetPipelineLayout()
{
	if (_vkPipelineLayout == VK_NULL_HANDLE)
	{
		// first create descriptor set layouts
		for (size_t i = 0; i < _vkDescriptorSetLayoutsInfo.Size(); ++i)
		{
			VkDescriptorSetLayout vkDescriptorLayout = VK_NULL_HANDLE;
			VulkanApi::GetApi()->vkCreateDescriptorSetLayout(_pDevice->GetDeviceHandle(), &_vkDescriptorSetLayoutsInfo[i], nullptr, &vkDescriptorLayout);
			if (vkDescriptorLayout != VK_NULL_HANDLE)
				_vkDescriptorSetLayouts.Push(vkDescriptorLayout);
		}
		_vkPipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(_vkDescriptorSetLayouts.Size());
		_vkPipelineLayoutInfo.pSetLayouts = (_vkDescriptorSetLayouts.Empty()) ? nullptr : _vkDescriptorSetLayouts.Data();

		// create
		VulkanApi::GetApi()->vkCreatePipelineLayout(_pDevice->GetDeviceHandle(), &_vkPipelineLayoutInfo, nullptr, &_vkPipelineLayout);
		assert(_vkPipelineLayout != VK_NULL_HANDLE);
	}

	return _vkPipelineLayout;
}

}
