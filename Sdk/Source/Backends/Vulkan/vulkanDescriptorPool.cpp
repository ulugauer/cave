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

/// @file vulkanDescriptorPool.cpp
///       Vulkan descriptor pool handling

#include "vulkanDescriptorPool.h"
#include "vulkanRenderDevice.h"
#include "vulkanConversion.h"

#include "vulkanApi.h"

#include<limits>

namespace cave
{

VulkanDescriptorPool::VulkanDescriptorPool(VulkanRenderDevice* device, HalDescriptorPoolInfo& descriptorPoolInfo)
	: HalDescriptorPool(device, descriptorPoolInfo)
	, _pDevice(device)
	, _vkDescriptorPool(VK_NULL_HANDLE)
{
	VkDescriptorPoolCreateInfo vkPoolInfo;
	vkPoolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	vkPoolInfo.pNext = nullptr;
	vkPoolInfo.flags = VulkanTypeConversion::ConvertDescriptorPoolFlagsToVulkan(descriptorPoolInfo._flags);
	
	caveVector<VkDescriptorPoolSize> vkPoolSizes(device->GetEngineAllocator());

	for (size_t i = 0; i < descriptorPoolInfo._poolSizeCount; ++i)
	{
		VkDescriptorPoolSize vkPoolSize;
		vkPoolSize.descriptorCount = descriptorPoolInfo._pPoolSizes[i]._descriptorCount;
		vkPoolSize.type = VulkanTypeConversion::ConvertDescriptorTypeToVulkan(descriptorPoolInfo._pPoolSizes[i]._type);
		vkPoolSizes.Push(vkPoolSize);
	}

	vkPoolInfo.pPoolSizes = vkPoolSizes.Data();
	vkPoolInfo.poolSizeCount = descriptorPoolInfo._poolSizeCount;
	// For now make this equal
	vkPoolInfo.maxSets = vkPoolInfo.poolSizeCount;

	if (VulkanApi::GetApi()->vkCreateDescriptorPool(_pDevice->GetDeviceHandle(), &vkPoolInfo, nullptr, &_vkDescriptorPool) != VK_SUCCESS)
	{
		throw BackendException("Failed to create descriptor pool");
	}
}

VulkanDescriptorPool::~VulkanDescriptorPool()
{
	if (_vkDescriptorPool != VK_NULL_HANDLE)
		VulkanApi::GetApi()->vkDestroyDescriptorPool(_pDevice->GetDeviceHandle(), _vkDescriptorPool, nullptr);
}

}
