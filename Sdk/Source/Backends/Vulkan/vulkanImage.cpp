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

#include "vulkanImage.h"
#include "vulkanRenderDevice.h"
#include "vulkanConversion.h"

#include "vulkanApi.h"

#include<limits>
#include<cstring>

namespace cave
{

VulkanImage::VulkanImage(VulkanRenderDevice* device, HalImageInfo& imageInfo)
	: HalImage(device, imageInfo)
	, _pDevice(device)
	, _vkImage(VK_NULL_HANDLE)
	, _familyIndicesArray(nullptr)
	, _vkMemProperties(0)
{

	_vkCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	_vkCreateInfo.pNext = nullptr;
	_vkCreateInfo.imageType = VK_IMAGE_TYPE_2D;
	_vkCreateInfo.format = VulkanTypeConversion::ConvertImageFormatToVulkan(imageInfo._format);
	_vkCreateInfo.extent.width = imageInfo._width;
	_vkCreateInfo.extent.height = imageInfo._height;
	_vkCreateInfo.extent.depth = imageInfo._depth;
	_vkCreateInfo.mipLevels = imageInfo._level;
	_vkCreateInfo.arrayLayers = imageInfo._slices;
	_vkCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
	_vkCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	_vkCreateInfo.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
	_vkCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	_vkCreateInfo.samples = VulkanTypeConversion::ConvertSampleCountToVulkan(imageInfo._sampleCount);
	_vkCreateInfo.flags = 0; // Optional
	_vkCreateInfo.queueFamilyIndexCount = imageInfo._queueFamilyIndexCount;
	_vkCreateInfo.pQueueFamilyIndices = imageInfo._queueFamilyIndices;

	VkResult result = VulkanApi::GetApi()->vkCreateImage(_pDevice->GetDeviceHandle(), &_vkCreateInfo, nullptr, &_vkImage);
	if (result != VK_SUCCESS)
		throw BackendException("Error failed to create vulkan image");
	
}

VulkanImage::~VulkanImage()
{
	if (_vkImage != VK_NULL_HANDLE)
		VulkanApi::GetApi()->vkDestroyImage(_pDevice->GetDeviceHandle(), _vkImage, nullptr);

	if (_deviceMemory._vkDeviceMemory != VK_NULL_HANDLE)
	{
		VulkanMemoryManager* memManager = _pDevice->GetMemoryManager();
		memManager->ReleaseBufferMemory(_deviceMemory);
	}
}

void VulkanImage::Bind()
{
	if (_deviceMemory._vkDeviceMemory != VK_NULL_HANDLE)
		return;

	VulkanMemoryManager* memManager = _pDevice->GetMemoryManager();

	VkMemoryRequirements memRequirements;
	VulkanApi::GetApi()->vkGetImageMemoryRequirements(_pDevice->GetDeviceHandle(), _vkImage, &memRequirements);

	VkMemoryAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRequirements.size;

	// allcoate memory
	memManager->AllocateBufferMemory(memRequirements, _vkMemProperties, _deviceMemory);

	// bind Memory to image object
	if (VulkanApi::GetApi()->vkBindImageMemory(_pDevice->GetDeviceHandle(), _vkImage, _deviceMemory._vkDeviceMemory, 0) != VK_SUCCESS)
		throw BackendException("Error failed to bind device memory");
}

void VulkanImage::Update(const void* )
{
	
}


}
