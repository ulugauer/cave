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

/// @file vulkanCommandPool.cpp
///       Vulkan command pool handling

#include "vulkanCommandPool.h"
#include "vulkanRenderDevice.h"
#include "vulkanConversion.h"

#include "vulkanApi.h"

#include<limits>

namespace cave
{

VulkanCommandPool::VulkanCommandPool(VulkanRenderDevice* device, HalCommandPoolInfo& commandPoolInfo)
	: HalCommandPool(device, commandPoolInfo)
	, _pDevice(device)
	, _vkCommandPool(VK_NULL_HANDLE)
{
	VkCommandPoolCreateInfo vkPoolCreateInfo;
	vkPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	vkPoolCreateInfo.pNext = nullptr;
	vkPoolCreateInfo.flags = VulkanTypeConversion::ConvertCommandPoolUsageFlagsToVulkan(commandPoolInfo._flags);
	vkPoolCreateInfo.queueFamilyIndex = (commandPoolInfo._queueFamily == HalCommandPoolQueueFamily::Graphics) ?
		_pDevice->GetGraphicsFamilyIndex() : _pDevice->GetPresentationFamilyIndex();

	if (VulkanApi::GetApi()->vkCreateCommandPool(_pDevice->GetDeviceHandle(), &vkPoolCreateInfo, nullptr, &_vkCommandPool) != VK_SUCCESS) 
	{
		throw BackendException("Failed to create vulkan device");
	}
}

VulkanCommandPool::~VulkanCommandPool()
{
	if (_vkCommandPool != VK_NULL_HANDLE)
		VulkanApi::GetApi()->vkDestroyCommandPool(_pDevice->GetDeviceHandle(), _vkCommandPool, nullptr);
}

}
