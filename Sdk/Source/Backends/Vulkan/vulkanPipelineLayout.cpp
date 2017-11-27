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

#include "vulkanApi.h"

#include<limits>
#include<cassert>

namespace cave
{


/**
* @brief Convert from generic shader stage flags to vulkan blend shader stage flags
*
* @param[in] flags		Generic shader stage flags
*
* @return Vulkan shader stage flags
*/
static VkShaderStageFlags ConvertBlendOpToVulkan(ShaderStagesFlags flags)
{
	VkShaderStageFlags shaderStageFlags = 0;

	if (flags & static_cast<uint32_t>(ShaderStages::Vertex))
		shaderStageFlags |= VK_SHADER_STAGE_VERTEX_BIT;
	if (flags & static_cast<uint32_t>(ShaderStages::TessellationControl))
		shaderStageFlags |= VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
	if (flags & static_cast<uint32_t>(ShaderStages::TessellationEvaluation))
		shaderStageFlags |= VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;
	if (flags & static_cast<uint32_t>(ShaderStages::Geometry))
		shaderStageFlags |= VK_SHADER_STAGE_GEOMETRY_BIT;
	if (flags & static_cast<uint32_t>(ShaderStages::Fragment))
		shaderStageFlags |= VK_SHADER_STAGE_FRAGMENT_BIT;
	if (flags & static_cast<uint32_t>(ShaderStages::Compute))
		shaderStageFlags |= VK_SHADER_STAGE_COMPUTE_BIT;

	assert(shaderStageFlags);
	return shaderStageFlags;
}

VulkanPipelineLayout::VulkanPipelineLayout(VulkanRenderDevice* device
		, caveVector<HalDescriptorSetLayout>& descriptorSetLayouts
		, caveVector<HalPushConstantRange>& pushConstants)
	: HalPipelineLayout(device, descriptorSetLayouts, pushConstants)
	, _pDevice(device)
	, _vkDescriptorLayouts(device->GetEngineAllocator())
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
			constantRange.stageFlags = ConvertBlendOpToVulkan(pushConstants[i]._shaderStagesFlags);
			constantRange.offset = pushConstants[i]._offset;
			constantRange.size = pushConstants[i]._size;
			_vkPushConstantRange.Push(constantRange);
		}
	}

	// first convert descriptor layouts
	if (!descriptorSetLayouts.Empty())
	{
		_vkDescriptorLayouts.Reserve(descriptorSetLayouts.Size());
		for (size_t i = 0; i < descriptorSetLayouts.Size(); ++i)
		{
			VkDescriptorSetLayout setLayout;
			setLayout = (VkDescriptorSetLayout)(descriptorSetLayouts[i]._descriptorLayoutSet);
			_vkDescriptorLayouts.Push(setLayout);
		}
	}

	// setup struct
	_vkPipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	_vkPipelineLayoutInfo.pNext = nullptr;
	_vkPipelineLayoutInfo.flags = 0;
	_vkPipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(_vkDescriptorLayouts.Size());
	_vkPipelineLayoutInfo.pSetLayouts = (_vkDescriptorLayouts.Empty()) ? nullptr : _vkDescriptorLayouts.Data();
	_vkPipelineLayoutInfo.pushConstantRangeCount = static_cast<uint32_t>(_vkPushConstantRange.Size());
	_vkPipelineLayoutInfo.pPushConstantRanges = (_vkPushConstantRange.Empty()) ? nullptr : _vkPushConstantRange.Data();
}

VulkanPipelineLayout::~VulkanPipelineLayout()
{
	if (!_vkPushConstantRange.Empty())
		_vkPushConstantRange.Clear();
	if (!_vkDescriptorLayouts.Empty())
		_vkDescriptorLayouts.Clear();

	if (_vkPipelineLayout != VK_NULL_HANDLE)
		VulkanApi::GetApi()->vkDestroyPipelineLayout(_pDevice->GetDeviceHandle(), _vkPipelineLayout, nullptr);
}

VkPipelineLayout VulkanPipelineLayout::GetPipelineLayout()
{
	if (_vkPipelineLayout == VK_NULL_HANDLE)
	{
		// create
		VulkanApi::GetApi()->vkCreatePipelineLayout(_pDevice->GetDeviceHandle(), &_vkPipelineLayoutInfo, nullptr, &_vkPipelineLayout);
		assert(_vkPipelineLayout != VK_NULL_HANDLE);
	}

	return _vkPipelineLayout;
}

}
