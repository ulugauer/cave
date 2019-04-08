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

/// @file vulkanImageView.cpp
///       Vulkan device image view

#include "vulkanImage.h"
#include "vulkanImageView.h"
#include "vulkanRenderDevice.h"
#include "vulkanConversion.h"

#include "vulkanApi.h"

#include<limits>
#include<cstring>

namespace cave
{

VulkanImageView::VulkanImageView(VulkanRenderDevice* device, HalImage* image, HalImageViewInfo& viewInfo)
    : HalImageView(device, image, viewInfo)
    , _pDevice(device)
    , _vkImageView(VK_NULL_HANDLE)
{
    VulkanImage* vkImage = static_cast<VulkanImage*>(image);

    _vkImageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    _vkImageViewCreateInfo.pNext = nullptr;
    _vkImageViewCreateInfo.flags = 0;
    _vkImageViewCreateInfo.image = vkImage->GetImage();
    _vkImageViewCreateInfo.viewType = VulkanTypeConversion::ConvertImageViewTypeToVulkan(viewInfo._type);
    _vkImageViewCreateInfo.format = VulkanTypeConversion::ConvertImageFormatToVulkan(viewInfo._format);
    _vkImageViewCreateInfo.components = VulkanTypeConversion::ConvertComponentMappingToVulkan(viewInfo._components);
    _vkImageViewCreateInfo.subresourceRange.aspectMask = VulkanTypeConversion::ConvertImageAspectFlagsToVulkan(viewInfo._subresourceRange._aspectMask);
    _vkImageViewCreateInfo.subresourceRange.baseMipLevel = viewInfo._subresourceRange._baseMipLevel;
    _vkImageViewCreateInfo.subresourceRange.levelCount = viewInfo._subresourceRange._levelCount;
    _vkImageViewCreateInfo.subresourceRange.baseArrayLayer = viewInfo._subresourceRange._baseArrayLayer;
    _vkImageViewCreateInfo.subresourceRange.layerCount = viewInfo._subresourceRange._layerCount;

    VkResult result = VulkanApi::GetApi()->vkCreateImageView(_pDevice->GetDeviceHandle(), &_vkImageViewCreateInfo, nullptr, &_vkImageView);
    if (result != VK_SUCCESS)
        throw BackendException("Error failed to create vulkan image view");

}

VulkanImageView::~VulkanImageView()
{
    if (_vkImageView)
        VulkanApi::GetApi()->vkDestroyImageView(_pDevice->GetDeviceHandle(), _vkImageView, nullptr);
}


}
