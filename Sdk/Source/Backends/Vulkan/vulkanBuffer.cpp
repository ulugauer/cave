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

/// @file vulkanBuffer.cpp
///       Vulkan device buffer

#include "vulkanBuffer.h"
#include "vulkanRenderDevice.h"
#include "vulkanConversion.h"

#include "vulkanApi.h"

#include<limits>
#include<cstring>

namespace cave
{

VulkanBuffer::VulkanBuffer(VulkanRenderDevice* device, HalBufferInfo& bufferInfo)
	: HalBuffer(device, bufferInfo)
	, _pDevice(device)
	, _vkBuffer(VK_NULL_HANDLE)
	, _familyIndicesArray(nullptr)
	, _vkMemProperties(0)
{
	_vkCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	_vkCreateInfo.pNext = nullptr;
	_vkCreateInfo.flags = VulkanTypeConversion::ConvertBufferCreateFlagsToVulkan(bufferInfo._create);
	_vkCreateInfo.size = bufferInfo._size;
	_vkCreateInfo.usage = VulkanTypeConversion::ConvertBufferUsageFlagsToVulkan(bufferInfo._usage);
	_vkCreateInfo.sharingMode = VulkanTypeConversion::ConvertSharedModeFlagsToVulkan(bufferInfo._shareMode);
	_vkCreateInfo.queueFamilyIndexCount = bufferInfo._queueFamilyIndexCount;

	if (bufferInfo._queueFamilyIndexCount)
	{
		_familyIndicesArray = AllocateArray<uint32_t>(*device->GetEngineAllocator(), bufferInfo._queueFamilyIndexCount);

		if (_familyIndicesArray)
		{
			for (uint32_t i = 0; i < bufferInfo._queueFamilyIndexCount; i++)
			{
				_familyIndicesArray[i] = bufferInfo._queueFamilyIndices[i];
			}
		}
	}
	
	_vkMemProperties = VulkanTypeConversion::ConvertMemoryPropertiesToVulkan(bufferInfo._properties);
	_vkCreateInfo.pQueueFamilyIndices = _familyIndicesArray;

	VkResult result = VulkanApi::GetApi()->vkCreateBuffer(_pDevice->GetDeviceHandle(), &_vkCreateInfo, nullptr, &_vkBuffer);
	if (result != VK_SUCCESS)
		throw BackendException("Error failed to create vulkan buffer");
}

VulkanBuffer::~VulkanBuffer()
{
	if (_vkCreateInfo.pQueueFamilyIndices)
		DeallocateDelete(*_pDevice->GetEngineAllocator(), *_familyIndicesArray);

	if (_vkBuffer != VK_NULL_HANDLE)
		VulkanApi::GetApi()->vkDestroyBuffer(_pDevice->GetDeviceHandle(), _vkBuffer, nullptr);

	if (_deviceMemory._vkDeviceMemory != VK_NULL_HANDLE)
	{
		VulkanMemoryManager* memManager = _pDevice->GetMemoryManager();
		memManager->ReleaseBufferMemory(_deviceMemory);
	}
}

void VulkanBuffer::Bind()
{
	if (_deviceMemory._vkDeviceMemory != VK_NULL_HANDLE)
		return;

	VulkanMemoryManager* memManager = _pDevice->GetMemoryManager();

	// query memory requirements for this buffer
	VkMemoryRequirements memRequirements;
	VulkanApi::GetApi()->vkGetBufferMemoryRequirements(_pDevice->GetDeviceHandle(), _vkBuffer, &memRequirements);

	// allcoate memory
	memManager->AllocateBufferMemory(memRequirements, _vkMemProperties, _deviceMemory);

	// bind Memory to buffer object
	if (VulkanApi::GetApi()->vkBindBufferMemory(_pDevice->GetDeviceHandle(), _vkBuffer, _deviceMemory._vkDeviceMemory, 0) != VK_SUCCESS)
		throw BackendException("Error failed to bind device memory");
}

void VulkanBuffer::Update(uint64_t offset, uint64_t size, const void* pData)
{
	if (_deviceMemory._vkDeviceMemory == VK_NULL_HANDLE)
		return;

	VulkanMemoryManager* memManager = _pDevice->GetMemoryManager();

	VulkanStagingBufferInfo stagingBufferInfo;
	memManager->GetStagingBuffer(size, stagingBufferInfo);

	if (stagingBufferInfo.GetMappedAddress() == nullptr)
		return;

	// copy data to staging buffer
	std::memcpy(stagingBufferInfo.GetMappedAddress(), pData, (size_t)size);

	// Flush memory if needed
	if (stagingBufferInfo.NeedsFlush())
		memManager->FlushStagingMemory(stagingBufferInfo._statgingMemory);

	// copy buffer
	memManager->CopyBuffer(stagingBufferInfo._stagingBuffer, _vkBuffer, stagingBufferInfo.GetOffset(), offset, size);
}

void VulkanBuffer::Map(uint64_t offset, uint64_t size, void** ppData)
{
	*ppData = nullptr;

	if (_deviceMemory._vkDeviceMemory == VK_NULL_HANDLE)
		return;

	if ((size > _vkCreateInfo.size) || (offset + size > _vkCreateInfo.size))
		return;

	VulkanApi::GetApi()->vkMapMemory(_pDevice->GetDeviceHandle(), _deviceMemory._vkDeviceMemory, offset, size, 0, ppData);
}

void VulkanBuffer::Unmap()
{
	VulkanApi::GetApi()->vkUnmapMemory(_pDevice->GetDeviceHandle(), _deviceMemory._vkDeviceMemory);
}

size_t VulkanBuffer::GetDataAlignment()
{
	VkMemoryRequirements reqs;
	VulkanApi::GetApi()->vkGetBufferMemoryRequirements(_pDevice->GetDeviceHandle(), _vkBuffer, &reqs);
	return reqs.alignment;
}

}
