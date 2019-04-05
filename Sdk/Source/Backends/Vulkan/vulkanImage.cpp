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
        memManager->ReleaseImageMemory(_deviceMemory);
    }
}

void VulkanImage::Bind()
{
    if (_deviceMemory._vkDeviceMemory != VK_NULL_HANDLE)
        return;

    VulkanMemoryManager* memManager = _pDevice->GetMemoryManager();

    VkMemoryRequirements memRequirements;
    VulkanApi::GetApi()->vkGetImageMemoryRequirements(_pDevice->GetDeviceHandle(), _vkImage, &memRequirements);

    // allcoate memory
    memManager->AllocateImageMemory(memRequirements, _vkMemProperties, _deviceMemory);

    // bind Memory to image object
    if (VulkanApi::GetApi()->vkBindImageMemory(_pDevice->GetDeviceHandle(), _vkImage, _deviceMemory._vkDeviceMemory, 0) != VK_SUCCESS)
        throw BackendException("Error failed to bind device memory");
}

void VulkanImage::Update(const void* data)
{
    if (_deviceMemory._vkDeviceMemory == VK_NULL_HANDLE)
        return;

    // compute buffers size required for upload
    VulkanImageSizeInfo imageSizeInfo = VulkanTypeConversion::GetImageSizeInfo(_vkCreateInfo.format);
    uint64_t size = 0;
    for (uint32_t i = 0; i < _vkCreateInfo.mipLevels; i++)
    {
        uint32_t width = _vkCreateInfo.extent.width >> i;
        uint32_t height = _vkCreateInfo.extent.height >> i;
        // clamp size
        width = (width > 0) ? width : 1;
        height = (height > 0) ? height : 1;

        uint32_t mipSize = width * height * imageSizeInfo._elementSize;
        if (imageSizeInfo._compressed && mipSize < imageSizeInfo._blockSize)
            mipSize = imageSizeInfo._blockSize;

        size += mipSize;
    }

    VulkanMemoryManager* memManager = _pDevice->GetMemoryManager();

    VulkanStagingBufferInfo stagingBufferInfo;
    memManager->GetStagingBuffer(size, stagingBufferInfo);

    if (stagingBufferInfo.GetMappedAddress() == nullptr)
        return;

    // copy data to staging buffer
    std::memcpy(stagingBufferInfo.GetMappedAddress(), data, (size_t)size);

    // upload level by level to hardware
    caveVector<VkBufferImageCopy> imageCopyArray(_pDevice->GetEngineAllocator(), _vkCreateInfo.mipLevels);
    uint32_t bufferOffset = 0;

    // setup copy regions
    for (uint32_t i = 0; i < _vkCreateInfo.mipLevels; i++)
    {
        uint32_t width = _vkCreateInfo.extent.width >> i;
        uint32_t height = _vkCreateInfo.extent.height >> i;
        // clamp size
        width = (width > 0) ? width : 1;
        height = (height > 0) ? height : 1;

        uint32_t mipSize = width * height * imageSizeInfo._elementSize;
        if (imageSizeInfo._compressed && mipSize < imageSizeInfo._blockSize)
            mipSize = imageSizeInfo._blockSize;

        imageCopyArray[i].bufferOffset = bufferOffset + stagingBufferInfo.GetOffset();
        imageCopyArray[i].bufferRowLength = 0;
        imageCopyArray[i].bufferImageHeight = 0;

        imageCopyArray[i].imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        imageCopyArray[i].imageSubresource.baseArrayLayer = 0;
        imageCopyArray[i].imageSubresource.layerCount = 1;
        imageCopyArray[i].imageSubresource.mipLevel = i;

        imageCopyArray[i].imageOffset = { 0, 0, 0 };
        imageCopyArray[i].imageExtent = { width, height, 1 };

        bufferOffset += mipSize;
    }
}


}
