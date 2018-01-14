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
	, _vkTransferCommandBuffer(VK_NULL_HANDLE)
	, _vkBufferCopyFence(VK_NULL_HANDLE)
	, _vkStagingBuffer(VK_NULL_HANDLE)
{
	// for some operations we need a command pool
	VkCommandPoolCreateInfo vkPoolCreateInfo;
	vkPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	vkPoolCreateInfo.pNext = nullptr;
	vkPoolCreateInfo.flags = 0;
	vkPoolCreateInfo.queueFamilyIndex = _pRenderDevice->GetGraphicsFamilyIndex();

	if (VulkanApi::GetApi()->vkCreateCommandPool(_pRenderDevice->GetDeviceHandle(), &vkPoolCreateInfo, nullptr, &_vkCommandPool) != VK_SUCCESS)
		throw BackendException("Error failed to GPU device memory manager");

	// allocate command buffer used for transfers
	VkCommandBufferAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandPool = _vkCommandPool;
	allocInfo.commandBufferCount = 1;

	if (VulkanApi::GetApi()->vkAllocateCommandBuffers(_pRenderDevice->GetDeviceHandle(), &allocInfo, &_vkTransferCommandBuffer) != VK_SUCCESS)
		throw BackendException("Error failed to GPU device memory manager");

	// create sync fence
	VkFenceCreateInfo fenceInfo;
	fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceInfo.pNext = nullptr;
	fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
	if (VulkanApi::GetApi()->vkCreateFence(_pRenderDevice->GetDeviceHandle(), &fenceInfo, nullptr, &_vkBufferCopyFence) != VK_SUCCESS)
		throw BackendException("Error failed to GPU device memory manager");

	// create staging buffer
	// create temporay staging buffer
	VkBufferCreateInfo stagingCreateInfo = {};
	stagingCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	stagingCreateInfo.size = StagingBufferSize;
	stagingCreateInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
	stagingCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	if (VulkanApi::GetApi()->vkCreateBuffer(_pRenderDevice->GetDeviceHandle(), &stagingCreateInfo, NULL, &_vkStagingBuffer) != VK_SUCCESS)
		return;
}

VulkanMemoryManager::~VulkanMemoryManager()
{
	// Wait until the device is idle before deleting
	VulkanApi::GetApi()->vkQueueWaitIdle(_pRenderDevice->GetGraphicsQueue());

	ReleaseStagingMemory(_stagingBufferDeviceMemeory);

	if (_vkStagingBuffer != VK_NULL_HANDLE)
		VulkanApi::GetApi()->vkDestroyBuffer(_pRenderDevice->GetDeviceHandle(), _vkStagingBuffer, nullptr);

	if (_vkTransferCommandBuffer != VK_NULL_HANDLE)
		VulkanApi::GetApi()->vkFreeCommandBuffers(_pRenderDevice->GetDeviceHandle(), _vkCommandPool, 1, &_vkTransferCommandBuffer);

	if (_vkCommandPool != VK_NULL_HANDLE)
		VulkanApi::GetApi()->vkDestroyCommandPool(_pRenderDevice->GetDeviceHandle(), _vkCommandPool, nullptr);

	if (_vkBufferCopyFence != VK_NULL_HANDLE)
		VulkanApi::GetApi()->vkDestroyFence(_pRenderDevice->GetDeviceHandle(), _vkBufferCopyFence, nullptr);
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

void VulkanMemoryManager::WaitForCopies()
{
	VkResult result = VulkanApi::GetApi()->vkGetFenceStatus(_pRenderDevice->GetDeviceHandle(), _vkBufferCopyFence);
	if (result == VK_SUCCESS)
		return; // nothing to wait for
	if (result == VK_NOT_READY)
	{
		// Wait and reset
		VulkanApi::GetApi()->vkWaitForFences(_pRenderDevice->GetDeviceHandle(), 1, &_vkBufferCopyFence, VK_TRUE, (std::numeric_limits<uint64_t>::max)());
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

void VulkanMemoryManager::GetStagingBuffer(uint64_t size, VulkanStagingBufferInfo& stagingBufferInfo)
{
	// Get available stating buffer
	VkBuffer stagingBuffer = FindStagingBuffer(size, stagingBufferInfo._statgingMemory);
	if (stagingBuffer != VK_NULL_HANDLE)
	{
		stagingBufferInfo._stagingBuffer = stagingBuffer;
	}
}

void VulkanMemoryManager::ReleaseStagingBuffer(VulkanStagingBufferInfo& )
{
	// nothing to do right now
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

VkBuffer VulkanMemoryManager::FindStagingBuffer(uint64_t size, VulkanDeviceMemory& memoryInfo)
{
	// For now we support only a single staging buffer with fixed size
	if (size > StagingBufferSize)
		return VK_NULL_HANDLE;

	if (_stagingBufferDeviceMemeory._vkDeviceMemory != VK_NULL_HANDLE)
	{
		// Fill in data
		memoryInfo._offset = _stagingBufferDeviceMemeory._offset;
		memoryInfo._size = size;
		memoryInfo._vkDeviceMemory = _stagingBufferDeviceMemeory._vkDeviceMemory;
		memoryInfo._mappedAddress = _stagingBufferDeviceMemeory._mappedAddress;
		return _vkStagingBuffer;
	}

	// lazy allocation
	VkMemoryRequirements memRequirements;
	VulkanApi::GetApi()->vkGetBufferMemoryRequirements(_pRenderDevice->GetDeviceHandle(), _vkStagingBuffer, &memRequirements);
	AllocateStagingMemory(memRequirements, _stagingBufferDeviceMemeory);
	if (_stagingBufferDeviceMemeory._mappedAddress == nullptr)
		return VK_NULL_HANDLE;

	// Fill in data
	memoryInfo._offset = _stagingBufferDeviceMemeory._offset;
	memoryInfo._size = size;
	memoryInfo._vkDeviceMemory = _stagingBufferDeviceMemeory._vkDeviceMemory;
	memoryInfo._mappedAddress = _stagingBufferDeviceMemeory._mappedAddress;

	VulkanApi::GetApi()->vkBindBufferMemory(_pRenderDevice->GetDeviceHandle(), _vkStagingBuffer, _stagingBufferDeviceMemeory._vkDeviceMemory, 0);
	return _vkStagingBuffer;
}

void VulkanMemoryManager::CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, uint64_t srcOffset, uint64_t dstOffset, uint64_t size)
{
	if (_vkCommandPool == VK_NULL_HANDLE || _vkTransferCommandBuffer == VK_NULL_HANDLE)
		return;

	// wait unitl last copy command has finished
	if (VulkanApi::GetApi()->vkWaitForFences(_pRenderDevice->GetDeviceHandle(), 1, &_vkBufferCopyFence, VK_TRUE, (std::numeric_limits<uint64_t>::max)()) != VK_SUCCESS)
		return;

	VkCommandBufferBeginInfo beginInfo = {};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

	if (VulkanApi::GetApi()->vkBeginCommandBuffer(_vkTransferCommandBuffer, &beginInfo) != VK_SUCCESS)
	{
		return;
	}

	VkBufferCopy copyRegion = {};
	copyRegion.srcOffset = srcOffset;
	copyRegion.dstOffset = dstOffset;
	copyRegion.size = size;
	VulkanApi::GetApi()->vkCmdCopyBuffer(_vkTransferCommandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

	// sync copy
	VkMemoryBarrier memoryBarrier =
	{
		VK_STRUCTURE_TYPE_MEMORY_BARRIER, nullptr, VK_ACCESS_TRANSFER_WRITE_BIT, VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT
	};
	VulkanApi::GetApi()->vkCmdPipelineBarrier(_vkTransferCommandBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_VERTEX_INPUT_BIT, 0, 1, &memoryBarrier, 0, nullptr, 0, nullptr);

	VulkanApi::GetApi()->vkEndCommandBuffer(_vkTransferCommandBuffer);

	// Reset fence for re-use
	VulkanApi::GetApi()->vkResetFences(_pRenderDevice->GetDeviceHandle(), 1, &_vkBufferCopyFence);
	// submit job
	VkSubmitInfo submitInfo = {};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &_vkTransferCommandBuffer;

	VulkanApi::GetApi()->vkQueueSubmit(_pRenderDevice->GetGraphicsQueue(), 1, &submitInfo, _vkBufferCopyFence);
}

}
