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

/// @file vulkanDescriptorSet.cpp
///        Vulkan descriptor set layout

#include "vulkanDescriptorSet.h"
#include "vulkanRenderDevice.h"
#include "vulkanConversion.h"

#include "vulkanApi.h"

#include<limits>
#include<cassert>

namespace cave
{

VulkanDescriptorSet::VulkanDescriptorSet(VulkanRenderDevice* device
	, caveVector<HalDescriptorSetLayout>& descriptorSetLayouts)
	: HalDescriptorSet(device)
	, _pDevice(device)
	, _vkDescriptorSetLayoutsInfo(device->GetEngineAllocator())
	, _vkDescriptorSetLayouts(device->GetEngineAllocator())
{
	//Convert descriptor layouts
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
}

VulkanDescriptorSet::~VulkanDescriptorSet()
{
	for (size_t i = 0; i < _vkDescriptorSetLayoutsInfo.Size(); ++i)
	{
		if (_vkDescriptorSetLayoutsInfo[i].pBindings != nullptr)
			_pDevice->GetEngineAllocator()->Deallocate((void *)_vkDescriptorSetLayoutsInfo[i].pBindings);
	}
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
}

caveVector<VkDescriptorSetLayout>& VulkanDescriptorSet::GetDescriptorSets()
{
	if (_vkDescriptorSetLayouts.Empty())
	{
		// Create descriptor set layouts
		for (size_t i = 0; i < _vkDescriptorSetLayoutsInfo.Size(); ++i)
		{
			VkDescriptorSetLayout vkDescriptorLayout = VK_NULL_HANDLE;
			VulkanApi::GetApi()->vkCreateDescriptorSetLayout(_pDevice->GetDeviceHandle(), &_vkDescriptorSetLayoutsInfo[i], nullptr, &vkDescriptorLayout);
			if (vkDescriptorLayout != VK_NULL_HANDLE)
				_vkDescriptorSetLayouts.Push(vkDescriptorLayout);
		}
	}

	return _vkDescriptorSetLayouts;
}

}
