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
{

}

VulkanMemoryManager::~VulkanMemoryManager()
{

}

void VulkanMemoryManager::AllocateBufferMemory(VkMemoryRequirements& memRequirements, VkMemoryPropertyFlags properties, VulkanDeviceMemory& deviceMemory)
{
	uint32_t selectedHeapIndex = ChooseMemoryHeap(memRequirements, properties);
	if (selectedHeapIndex != ~0u)
	{
		// Allocate memory
		VkMemoryAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = selectedHeapIndex;
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

uint32_t VulkanMemoryManager::ChooseMemoryHeap(VkMemoryRequirements& memRequirements, VkMemoryPropertyFlags properties)
{
	VkPhysicalDeviceMemoryProperties deviceMemProperties;
	_pRenderDevice->GetPhysicalDeviceMemoryProperties(deviceMemProperties);

	uint32_t selectedHeapIndex = ~0u;
	for (uint32_t i = 0; i < deviceMemProperties.memoryTypeCount; i++)
	{
		if ((memRequirements.memoryTypeBits & (1 << i)) && (deviceMemProperties.memoryTypes[i].propertyFlags & properties) == properties) {
			selectedHeapIndex = i;
			break;
		}
	}

	return selectedHeapIndex;
}

}
