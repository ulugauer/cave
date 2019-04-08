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

#define align_to(_alignment, _val) (((_val + _alignment - 1) / _alignment) * _alignment)


VulkanMemoryManager::VulkanMemoryManager(VulkanInstance* instance, VulkanPhysicalDevice* physicalDevice, VulkanRenderDevice* renderDevice)
	: _pInstance(instance)
	, _pPhysicalDevice(physicalDevice)
	, _pRenderDevice(renderDevice)
	, _nonCoherentAlignment(0)
	, _vkCommandPool(VK_NULL_HANDLE)
	, _vkTransferCommandBuffer(VK_NULL_HANDLE)
	, _vkImageTransferCommandBuffer(VK_NULL_HANDLE)
	, _bufferMemoryPages(renderDevice->GetEngineAllocator())
	, _vkCopyFence(VK_NULL_HANDLE)
    , _vkCopyImageFence(VK_NULL_HANDLE)
	, _stagingMemoryPages(renderDevice->GetEngineAllocator())
	, _copyCount(0)
	, _copyWaitCount(0)
    , _copyImageCount(0)
    , _copyImageWaitCount(0)
{
	// store some physical device properties
	VkPhysicalDeviceProperties deviceProperties = physicalDevice->GetPhysicalDeviceProperties();
	_nonCoherentAlignment = deviceProperties.limits.nonCoherentAtomSize;

	// for some operations we need a command pool
	VkCommandPoolCreateInfo vkPoolCreateInfo;
	vkPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	vkPoolCreateInfo.pNext = nullptr;
	vkPoolCreateInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
	vkPoolCreateInfo.queueFamilyIndex = _pRenderDevice->GetGraphicsFamilyIndex();

	if (VulkanApi::GetApi()->vkCreateCommandPool(_pRenderDevice->GetDeviceHandle(), &vkPoolCreateInfo, nullptr, &_vkCommandPool) != VK_SUCCESS)
		throw BackendException("Error failed to create GPU device memory manager");

	// allocate command buffer used for buffer transfers
	VkCommandBufferAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandPool = _vkCommandPool;
	allocInfo.commandBufferCount = 1;

	if (VulkanApi::GetApi()->vkAllocateCommandBuffers(_pRenderDevice->GetDeviceHandle(), &allocInfo, &_vkTransferCommandBuffer) != VK_SUCCESS)
		throw BackendException("Error failed to GPU device memory manager");

	// same for image transfer
	if (VulkanApi::GetApi()->vkAllocateCommandBuffers(_pRenderDevice->GetDeviceHandle(), &allocInfo, &_vkImageTransferCommandBuffer) != VK_SUCCESS)
		throw BackendException("Error failed to create GPU device memory manager");

	// create buffer transfer sync fence
	VkFenceCreateInfo fenceInfo;
	fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceInfo.pNext = nullptr;
	fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
	if (VulkanApi::GetApi()->vkCreateFence(_pRenderDevice->GetDeviceHandle(), &fenceInfo, nullptr, &_vkCopyFence) != VK_SUCCESS)
		throw BackendException("Error failed to create GPU device memory manager");

	VulkanApi::GetApi()->vkResetFences(_pRenderDevice->GetDeviceHandle(), 1, &_vkCopyFence);

    // create image transfer sync fence
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
    if (VulkanApi::GetApi()->vkCreateFence(_pRenderDevice->GetDeviceHandle(), &fenceInfo, nullptr, &_vkCopyImageFence) != VK_SUCCESS)
        throw BackendException("Error failed to create GPU device memory manager");

    VulkanApi::GetApi()->vkResetFences(_pRenderDevice->GetDeviceHandle(), 1, &_vkCopyImageFence);

	// Allocate a page at start
	AllocateStagingPage(StagingBufferSize);
}

VulkanMemoryManager::~VulkanMemoryManager()
{
	// Wait until the device is idle before deleting
	VulkanApi::GetApi()->vkQueueWaitIdle(_pRenderDevice->GetGraphicsQueue());

	if (_vkTransferCommandBuffer != VK_NULL_HANDLE)
		VulkanApi::GetApi()->vkFreeCommandBuffers(_pRenderDevice->GetDeviceHandle(), _vkCommandPool, 1, &_vkTransferCommandBuffer);

	if (_vkImageTransferCommandBuffer != VK_NULL_HANDLE)
		VulkanApi::GetApi()->vkFreeCommandBuffers(_pRenderDevice->GetDeviceHandle(), _vkCommandPool, 1, &_vkImageTransferCommandBuffer);

	if (_vkCommandPool != VK_NULL_HANDLE)
		VulkanApi::GetApi()->vkDestroyCommandPool(_pRenderDevice->GetDeviceHandle(), _vkCommandPool, nullptr);

    if (_vkCopyFence != VK_NULL_HANDLE)
        VulkanApi::GetApi()->vkDestroyFence(_pRenderDevice->GetDeviceHandle(), _vkCopyFence, nullptr);

    if (_vkCopyImageFence != VK_NULL_HANDLE)
        VulkanApi::GetApi()->vkDestroyFence(_pRenderDevice->GetDeviceHandle(), _vkCopyImageFence, nullptr);

    // Release staging memory pages
    CaveList<VulkanDeviceMemoryPageEntry>::Iterator pageIter = _stagingMemoryPages.begin();
    for (; pageIter != _stagingMemoryPages.end(); pageIter++)
    {
        ReleaseStagingMemory(pageIter->_deviceMemory);
        if (pageIter->_vkBuffer != VK_NULL_HANDLE)
            VulkanApi::GetApi()->vkDestroyBuffer(_pRenderDevice->GetDeviceHandle(), pageIter->_vkBuffer, nullptr);
    }

    // Release buffer memory pages
	CaveList<VulkanDeviceMemoryPageEntry>::Iterator pageBufferIter = _bufferMemoryPages.begin();
	for (; pageBufferIter != _bufferMemoryPages.end(); pageBufferIter++)
	{
		if (pageBufferIter->_deviceMemory._vkDeviceMemory != VK_NULL_HANDLE)
			VulkanApi::GetApi()->vkFreeMemory(_pRenderDevice->GetDeviceHandle(), pageBufferIter->_deviceMemory._vkDeviceMemory, nullptr);
	}
}

void VulkanMemoryManager::AllocateBufferMemory(VkMemoryRequirements& memRequirements, VkMemoryPropertyFlags properties, VulkanDeviceMemory& deviceMemory)
{
	uint32_t memoryTypeIndex = ChooseMemoryType(memRequirements, properties);
	if (memoryTypeIndex != ~0u)
	{
		// Search for a place in a memory page
		CaveList<VulkanDeviceMemoryPageEntry>::Iterator pageBufferIter = _bufferMemoryPages.begin();
		for (; pageBufferIter != _bufferMemoryPages.end(); pageBufferIter++)
		{
			size_t currentOffset = align_to(memRequirements.alignment, pageBufferIter->_deviceMemory._offset);
			if ((currentOffset + memRequirements.size) < pageBufferIter->_deviceMemory._size && 
				memoryTypeIndex == pageBufferIter->_deviceMemory._memoryTypeIndex)
			{
				// Found a match. Fill in values
				deviceMemory._offset = currentOffset;
				deviceMemory._size = memRequirements.size;
				deviceMemory._vkDeviceMemory = pageBufferIter->_deviceMemory._vkDeviceMemory;

				// update page entries
				pageBufferIter->_allocationCount++;
				pageBufferIter->_deviceMemory._offset = currentOffset + memRequirements.size;

				return;
			}
		}

		// Allocate new memory page
		// new page
		_bufferMemoryPages.PushBack(VulkanDeviceMemoryPageEntry());
		CaveList<VulkanDeviceMemoryPageEntry>::Iterator last = _bufferMemoryPages.tail();

		uint64_t allocSize = (memRequirements.size < StagingBufferSize) ? StagingBufferSize : memRequirements.size;
		VkMemoryAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = allocSize;
		allocInfo.memoryTypeIndex = memoryTypeIndex;
		if (VulkanApi::GetApi()->vkAllocateMemory(_pRenderDevice->GetDeviceHandle(), &allocInfo, nullptr, &last->_deviceMemory._vkDeviceMemory) != VK_SUCCESS)
		{
			throw BackendException("Error failed to allocate device memory");
		}
		last->_allocationCount++;
		last->_deviceMemory._memoryTypeIndex = memoryTypeIndex;
		last->_deviceMemory._size = allocSize;
		last->_deviceMemory._offset = memRequirements.size;

		deviceMemory._offset = 0;
		deviceMemory._size = memRequirements.size;
		deviceMemory._vkDeviceMemory = last->_deviceMemory._vkDeviceMemory;
	}
}

void VulkanMemoryManager::ReleaseBufferMemory(VulkanDeviceMemory& deviceMemory)
{
    if (deviceMemory._vkDeviceMemory)
    {
        // Search for a place in a memory page
        CaveList<VulkanDeviceMemoryPageEntry>::Iterator pageBufferIter = _bufferMemoryPages.begin();
        for (; pageBufferIter != _bufferMemoryPages.end(); pageBufferIter++)
        {
            if (pageBufferIter->_deviceMemory._vkDeviceMemory == deviceMemory._vkDeviceMemory)
            {
                // found page
                pageBufferIter->_allocationCount--;
                break;
            }
        }

        if (pageBufferIter != _bufferMemoryPages.end())
        {
            // Last allocation release page
            if (pageBufferIter->_allocationCount == 0)
            {
                VulkanApi::GetApi()->vkFreeMemory(_pRenderDevice->GetDeviceHandle(), pageBufferIter->_deviceMemory._vkDeviceMemory, nullptr);
                _bufferMemoryPages.Erase(pageBufferIter);
            }
            else
            {
                // Check if it was the last allcoation done on this page
                // If this is the case we can rollback the offset
                if (deviceMemory._offset + deviceMemory._size == pageBufferIter->_deviceMemory._offset)
                    pageBufferIter->_deviceMemory._offset = deviceMemory._offset;
            }
        }

        deviceMemory._offset = 0;
        deviceMemory._size = 0;
        deviceMemory._vkDeviceMemory = nullptr;
    }
}

void VulkanMemoryManager::AllocateImageMemory(VkMemoryRequirements& memRequirements, VkMemoryPropertyFlags properties, VulkanDeviceMemory& deviceMemory)
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

void VulkanMemoryManager::ReleaseImageMemory(VulkanDeviceMemory& deviceMemory)
{
	if (deviceMemory._size && deviceMemory._vkDeviceMemory)
	{
		VulkanApi::GetApi()->vkFreeMemory(_pRenderDevice->GetDeviceHandle(), deviceMemory._vkDeviceMemory, nullptr);
		deviceMemory._offset = 0;
		deviceMemory._size = 0;
		deviceMemory._vkDeviceMemory = nullptr;
	}
}

void VulkanMemoryManager::SubmitCopies()
{
    if (_copyCount > 0)
    {
        VulkanApi::GetApi()->vkEndCommandBuffer(_vkTransferCommandBuffer);

        // submit job
        VkSubmitInfo submitInfo = {};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &_vkTransferCommandBuffer;

        VulkanApi::GetApi()->vkQueueSubmit(_pRenderDevice->GetGraphicsQueue(), 1, &submitInfo, _vkCopyFence);

        _copyCount = 0;
    }
    if (_copyImageCount > 0)
    {
        VulkanApi::GetApi()->vkEndCommandBuffer(_vkImageTransferCommandBuffer);

        // submit job
        VkSubmitInfo submitInfo = {};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &_vkImageTransferCommandBuffer;

        VulkanApi::GetApi()->vkQueueSubmit(_pRenderDevice->GetGraphicsQueue(), 1, &submitInfo, _vkCopyImageFence);

        _copyImageCount = 0;
    }
}

void VulkanMemoryManager::WaitForCopies()
{
	SubmitCopies(); // just in case it did not happen already

    if (_copyWaitCount > 0)
    {
        VkResult result = VulkanApi::GetApi()->vkGetFenceStatus(_pRenderDevice->GetDeviceHandle(), _vkCopyFence);
        if (result == VK_NOT_READY)
        {
            // Wait for fence
            VulkanApi::GetApi()->vkWaitForFences(_pRenderDevice->GetDeviceHandle(), 1, &_vkCopyFence, VK_TRUE, (std::numeric_limits<uint64_t>::max)());
        }

        _copyWaitCount = 0;

        // Reset fence for re-use
        VulkanApi::GetApi()->vkResetFences(_pRenderDevice->GetDeviceHandle(), 1, &_vkCopyFence);

        // we re-use the command buffer which is safe after we waited for the fence
        VulkanApi::GetApi()->vkResetCommandBuffer(_vkTransferCommandBuffer, 0);
    }

    if (_copyImageWaitCount > 0)
    {
        VkResult result = VulkanApi::GetApi()->vkGetFenceStatus(_pRenderDevice->GetDeviceHandle(), _vkCopyImageFence);
        if (result == VK_NOT_READY)
        {
            // Wait for fence
            VulkanApi::GetApi()->vkWaitForFences(_pRenderDevice->GetDeviceHandle(), 1, &_vkCopyImageFence, VK_TRUE, (std::numeric_limits<uint64_t>::max)());
        }

        _copyImageWaitCount = 0;

        // Reset fence for re-use
        VulkanApi::GetApi()->vkResetFences(_pRenderDevice->GetDeviceHandle(), 1, &_vkCopyImageFence);

        // we re-use the command buffer which is safe after we waited for the fence
        VulkanApi::GetApi()->vkResetCommandBuffer(_vkImageTransferCommandBuffer, 0);
    }

	// Make staging buffers available again
	// Reset first page remove others
	CaveList<VulkanDeviceMemoryPageEntry>::Iterator begin = _stagingMemoryPages.begin();
	begin->_deviceMemory._offset = 0;
	begin++;
	
	CaveList<VulkanDeviceMemoryPageEntry>::Iterator pageIter = begin;
	for (; pageIter != _stagingMemoryPages.end(); pageIter++)
	{
		ReleaseStagingMemory(pageIter->_deviceMemory);
		if (pageIter->_vkBuffer != VK_NULL_HANDLE)
			VulkanApi::GetApi()->vkDestroyBuffer(_pRenderDevice->GetDeviceHandle(), pageIter->_vkBuffer, nullptr);

		_stagingMemoryPages.Erase(pageIter);
	}
}

void VulkanMemoryManager::AllocateStagingMemory(VkMemoryRequirements& memRequirements, VulkanDeviceMemory& deviceMemory)
{
	VkMemoryPropertyFlags properties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
	uint32_t memoryTypeIndex = ChooseMemoryType(memRequirements, properties);
	if (memoryTypeIndex == ~0u)
	{
		// if we failed try again without coherent bit
		properties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;
		memoryTypeIndex = ChooseMemoryType(memRequirements, properties);
		deviceMemory._needsFlush = true; // data needs to be flushed before used
	}

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

void VulkanMemoryManager::FlushStagingMemory(VulkanDeviceMemory& deviceMemory)
{
	VkMappedMemoryRange stagingRange;
	stagingRange.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
	stagingRange.pNext = nullptr;
	stagingRange.memory = deviceMemory._vkDeviceMemory;
	stagingRange.offset = deviceMemory._offset;

	// We must align size
	uint64_t alignedSize = (deviceMemory._size - 1) - ((deviceMemory._size - 1) % _nonCoherentAlignment) + _nonCoherentAlignment;
	stagingRange.size = alignedSize;
	VulkanApi::GetApi()->vkFlushMappedMemoryRanges(_pRenderDevice->GetDeviceHandle(), 1, &stagingRange);
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
	// iterate over the pages and find a spot
	CaveList<VulkanDeviceMemoryPageEntry>::Iterator pageIter = _stagingMemoryPages.begin();
	for (; pageIter != _stagingMemoryPages.end(); pageIter++)
	{
		VkMemoryRequirements reqs;
		VulkanApi::GetApi()->vkGetBufferMemoryRequirements(_pRenderDevice->GetDeviceHandle(), pageIter->_vkBuffer, &reqs);
		size_t currentOffset = align_to(reqs.alignment, pageIter->_deviceMemory._offset);
		if ((currentOffset + size) < pageIter->_deviceMemory._size)
		{
			// found a space
			// Fill in data
			memoryInfo._offset = currentOffset;
			memoryInfo._size = size;
			memoryInfo._vkDeviceMemory = pageIter->_deviceMemory._vkDeviceMemory;
			memoryInfo._mappedAddress = static_cast<uint8_t*>(pageIter->_deviceMemory._mappedAddress) + currentOffset;
			// update entries
			pageIter->_deviceMemory._offset = currentOffset + size;

			return pageIter->_vkBuffer;
		}
	}

	// alocate new page
	if (pageIter == _stagingMemoryPages.end())
	{
		uint64_t allocSize = (size < StagingBufferSize) ? StagingBufferSize : size;
		AllocateStagingPage(allocSize);
		CaveList<VulkanDeviceMemoryPageEntry>::Iterator last = _stagingMemoryPages.tail();
		VkMemoryRequirements reqs;
		VulkanApi::GetApi()->vkGetBufferMemoryRequirements(_pRenderDevice->GetDeviceHandle(), last->_vkBuffer, &reqs);
		size_t currentOffset = align_to(reqs.alignment, last->_deviceMemory._offset);
		if ((currentOffset + size) < last->_deviceMemory._size)
		{
			// found a space
			// Fill in data
			memoryInfo._offset = currentOffset;
			memoryInfo._size = size;
			memoryInfo._vkDeviceMemory = last->_deviceMemory._vkDeviceMemory;
			memoryInfo._mappedAddress = static_cast<uint8_t*>(last->_deviceMemory._mappedAddress) + currentOffset;
			// update entries
			last->_deviceMemory._offset = currentOffset + size;

			return last->_vkBuffer;
		}
	}

	// no success 
	return VK_NULL_HANDLE;
}

void VulkanMemoryManager::AllocateStagingPage(uint64_t pageSize)
{
	// new entry
	_stagingMemoryPages.PushBack(VulkanDeviceMemoryPageEntry());
	CaveList<VulkanDeviceMemoryPageEntry>::Iterator last = _stagingMemoryPages.tail();

	// create staging buffer
	VkBufferCreateInfo stagingCreateInfo = {};
	stagingCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	stagingCreateInfo.size = pageSize;
	stagingCreateInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
	stagingCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	if (VulkanApi::GetApi()->vkCreateBuffer(_pRenderDevice->GetDeviceHandle(), &stagingCreateInfo, NULL, &last->_vkBuffer) != VK_SUCCESS)
		return;

	VulkanDeviceMemory deviceMemory;
	VkMemoryRequirements memRequirements;
	VulkanApi::GetApi()->vkGetBufferMemoryRequirements(_pRenderDevice->GetDeviceHandle(), last->_vkBuffer, &memRequirements);
	AllocateStagingMemory(memRequirements, last->_deviceMemory);

	VulkanApi::GetApi()->vkBindBufferMemory(_pRenderDevice->GetDeviceHandle(), last->_vkBuffer, last->_deviceMemory._vkDeviceMemory, 0);
}

void VulkanMemoryManager::CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, uint64_t srcOffset, uint64_t dstOffset, uint64_t size)
{
	if (_vkCommandPool == VK_NULL_HANDLE || _vkTransferCommandBuffer == VK_NULL_HANDLE)
		return;

	if (_copyCount == 0)
	{
		// If we already submited some copies we need to wait before restart
		if (_copyWaitCount > 0)
			WaitForCopies();

		VkCommandBufferBeginInfo beginInfo = {};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

		VulkanApi::GetApi()->vkBeginCommandBuffer(_vkTransferCommandBuffer, &beginInfo);
	}

	VkBufferCopy copyRegion = {};
	copyRegion.srcOffset = srcOffset;
	copyRegion.dstOffset = dstOffset;
	copyRegion.size = size;
	VulkanApi::GetApi()->vkCmdCopyBuffer(_vkTransferCommandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

	_copyCount++;
	_copyWaitCount++;
}

void VulkanMemoryManager::CopyBufferToImage(VkBuffer srcBuffer, VkImage dstImage, caveVector<VkBufferImageCopy>& regions)
{
    if (_vkCommandPool == VK_NULL_HANDLE || _vkImageTransferCommandBuffer == VK_NULL_HANDLE)
        return;

    // First copy in frame
    if (_copyImageCount == 0)
    {
        // If we already submited some copies we need to wait before restart
        if (_copyImageWaitCount > 0)
            WaitForCopies();

        VkCommandBufferBeginInfo beginInfo = {};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

        VulkanApi::GetApi()->vkBeginCommandBuffer(_vkImageTransferCommandBuffer, &beginInfo);
    }

    // transition for copy
    VkImageMemoryBarrier barrier = {};
    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.image = dstImage;
    barrier.subresourceRange.aspectMask = regions[0].imageSubresource.aspectMask;   // the same for all
    barrier.subresourceRange.baseMipLevel = 0;
    barrier.subresourceRange.levelCount = static_cast<uint32_t>(regions.Size());
    barrier.subresourceRange.baseArrayLayer = 0;
    barrier.subresourceRange.layerCount = 1;
    barrier.srcAccessMask = 0;
    barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT; // We are going to write to the texture

    VulkanApi::GetApi()->vkCmdPipelineBarrier(_vkImageTransferCommandBuffer,
        VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT,
        0,
        0, nullptr, // no memory barriers
        0, nullptr, // no buffer barriers
        1, &barrier);

    VulkanApi::GetApi()->vkCmdCopyBufferToImage(_vkImageTransferCommandBuffer, srcBuffer, dstImage,
        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, static_cast<uint32_t>(regions.Size()), regions.Data());

    // transition for shader usage
    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
    barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.image = dstImage;
    barrier.subresourceRange.aspectMask = regions[0].imageSubresource.aspectMask;   // the same for all
    barrier.subresourceRange.baseMipLevel = 0;
    barrier.subresourceRange.levelCount = static_cast<uint32_t>(regions.Size());
    barrier.subresourceRange.baseArrayLayer = 0;
    barrier.subresourceRange.layerCount = 1;
    barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT; // Next access is shader reads

    VulkanApi::GetApi()->vkCmdPipelineBarrier(_vkImageTransferCommandBuffer,
        VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_VERTEX_SHADER_BIT,
        0,
        0, nullptr, // no memory barriers
        0, nullptr, // no buffer barriers
        1, &barrier);

    _copyImageCount++;
    _copyImageWaitCount++;
}

}
