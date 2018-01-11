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

/// @file vulkanMemoryManager.cpp
///       Manage device memory

#include "vulkanMemoryManager.h"
#include "vulkanInstance.h"
#include "vulkanRenderDevice.h"
#include "vulkanPhysicalDevice.h"

#include "vulkanApi.h"

#include<limits>

namespace cave
{

VulkanMemoryManager::VulkanMemoryManager(VulkanInstance* instance, VulkanPhysicalDevice* physicalDevice, VulkanRenderDevice* renderDevice)
	: _pInstance(instance)
	, _pPhysicalDevice(physicalDevice)
	, _pRenderDevice(renderDevice)
	, _vkCommandPool(VK_NULL_HANDLE)
{
	// for some operations we need a command pool
	VkCommandPoolCreateInfo vkPoolCreateInfo;
	vkPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	vkPoolCreateInfo.pNext = nullptr;
	vkPoolCreateInfo.flags = 0;
	vkPoolCreateInfo.queueFamilyIndex = _pRenderDevice->GetGraphicsFamilyIndex();

	VulkanApi::GetApi()->vkCreateCommandPool(_pRenderDevice->GetDeviceHandle(), &vkPoolCreateInfo, nullptr, &_vkCommandPool);
}

VulkanMemoryManager::~VulkanMemoryManager()
{
	if (_vkCommandPool != VK_NULL_HANDLE)
		VulkanApi::GetApi()->vkDestroyCommandPool(_pRenderDevice->GetDeviceHandle(), _vkCommandPool, nullptr);
}

void VulkanMemoryManager::AllocateBufferMemory(VkMemoryRequirements& memRequirements, VkMemoryPropertyFlags properties, VulkanDeviceMemory& deviceMemory)
{
	uint32_t memoryTypeIndex = ChooseMemoryType(memRequirements, properties);
	if (memoryTypeIndex != ~0u)
	{
		// Allocate memory
		VkMemoryAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = memoryTypeIndex;
		if (VulkanApi::GetApi()->vkAllocateMemory(_pRenderDevice->GetDeviceHandle(), &allocInfo, nullptr, &deviceMemory._vkDeviceMemory) != VK_SUCCESS)
		{
			throw BackendException("Error failed to allocate device memory");
		}

		deviceMemory._offset = 0;
		deviceMemory._size = memRequirements.size;
	}
}

void VulkanMemoryManager::ReleaseBufferMemory(VulkanDeviceMemory& deviceMemory)
{
	if (deviceMemory._size && deviceMemory._vkDeviceMemory)
	{
		VulkanApi::GetApi()->vkFreeMemory(_pRenderDevice->GetDeviceHandle(), deviceMemory._vkDeviceMemory, nullptr);
		deviceMemory._offset = 0;
		deviceMemory._size = 0;
		deviceMemory._vkDeviceMemory = nullptr;
	}
}

void VulkanMemoryManager::AllocateStagingMemory(VkMemoryRequirements& memRequirements, VulkanDeviceMemory& deviceMemory)
{
	VkMemoryPropertyFlags properties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
	uint32_t memoryTypeIndex = ChooseMemoryType(memRequirements, properties);
	if (memoryTypeIndex != ~0u)
	{
		// Allocate memory
		VkMemoryAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = memoryTypeIndex;
		if (VulkanApi::GetApi()->vkAllocateMemory(_pRenderDevice->GetDeviceHandle(), &allocInfo, nullptr, &deviceMemory._vkDeviceMemory) != VK_SUCCESS)
		{
			return;
		}
		
		if (VulkanApi::GetApi()->vkMapMemory(_pRenderDevice->GetDeviceHandle(), deviceMemory._vkDeviceMemory, 0, memRequirements.size, 0, &deviceMemory._mappedAddress) != VK_SUCCESS)
		{
			return;
		}
		
		deviceMemory._offset = 0;
		deviceMemory._size = memRequirements.size;
	}
}

void VulkanMemoryManager::ReleaseStagingMemory(VulkanDeviceMemory& deviceMemory)
{
	if (deviceMemory._size && deviceMemory._vkDeviceMemory)
	{
		VulkanApi::GetApi()->vkUnmapMemory(_pRenderDevice->GetDeviceHandle(), deviceMemory._vkDeviceMemory);
		VulkanApi::GetApi()->vkFreeMemory(_pRenderDevice->GetDeviceHandle(), deviceMemory._vkDeviceMemory, nullptr);
		deviceMemory._offset = 0;
		deviceMemory._size = 0;
		deviceMemory._vkDeviceMemory = nullptr;
	}
}

uint32_t VulkanMemoryManager::ChooseMemoryType(VkMemoryRequirements& memRequirements, VkMemoryPropertyFlags properties)
{
	const VkPhysicalDeviceMemoryProperties& deviceMemProperties = _pPhysicalDevice->GetPhysicalDeviceMemoryProperties();

	uint32_t memoryTypeIndex = ~0u;
	for (uint32_t i = 0; i < deviceMemProperties.memoryTypeCount; i++)
	{
		if ((memRequirements.memoryTypeBits & (1 << i)) && (deviceMemProperties.memoryTypes[i].propertyFlags & properties) == properties) {
			memoryTypeIndex = i;
			break;
		}
	}

	return memoryTypeIndex;
}

void VulkanMemoryManager::CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, uint64_t srcOffset, uint64_t dstOffset, uint64_t size)
{
	if (_vkCommandPool == VK_NULL_HANDLE)
		return;

	VkCommandBufferAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandPool = _vkCommandPool;
	allocInfo.commandBufferCount = 1;

	VkCommandBuffer commandBuffer;
	if (VulkanApi::GetApi()->vkAllocateCommandBuffers(_pRenderDevice->GetDeviceHandle(), &allocInfo, &commandBuffer) != VK_SUCCESS)
		return;

	VkCommandBufferBeginInfo beginInfo = {};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

	if (VulkanApi::GetApi()->vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS)
	{
		VulkanApi::GetApi()->vkFreeCommandBuffers(_pRenderDevice->GetDeviceHandle(), _vkCommandPool, 1, &commandBuffer);
		return;
	}

	VkBufferCopy copyRegion = {};
	copyRegion.srcOffset = srcOffset;
	copyRegion.dstOffset = dstOffset;
	copyRegion.size = size;
	VulkanApi::GetApi()->vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

	// sync copy
	VkMemoryBarrier memoryBarrier =
	{
		VK_STRUCTURE_TYPE_MEMORY_BARRIER, nullptr, VK_ACCESS_TRANSFER_WRITE_BIT, VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT
	};
	VulkanApi::GetApi()->vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_VERTEX_INPUT_BIT, 0, 1, &memoryBarrier, 0, nullptr, 0, nullptr);

	VulkanApi::GetApi()->vkEndCommandBuffer(commandBuffer);
	
	// submit job
	VkSubmitInfo submitInfo = {};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffer;

	VulkanApi::GetApi()->vkQueueSubmit(_pRenderDevice->GetGraphicsQueue(), 1, &submitInfo, VK_NULL_HANDLE);

	VulkanApi::GetApi()->vkFreeCommandBuffers(_pRenderDevice->GetDeviceHandle(), _vkCommandPool, 1, &commandBuffer);
}

}
