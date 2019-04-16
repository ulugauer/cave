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

/// @file vulkanFrameBuffer.cpp
///       Vulkan device image view

#include "vulkanFrameBuffer.h"
#include "vulkanImageView.h"
#include "vulkanRenderDevice.h"
#include "vulkanConversion.h"

#include "vulkanApi.h"

#include<limits>
#include<cstring>

namespace cave
{

VulkanFrameBuffer::VulkanFrameBuffer(VulkanRenderDevice* device, VkRenderPass renderPass,
    uint32_t width, uint32_t height, uint32_t layers, caveVector<VkImageView>& vkImageViews)
    : HalFrameBuffer(device, width, height, layers)
    , _pDevice(device)
    , _vkFrameBuffer(VK_NULL_HANDLE)
{
    VkFramebufferCreateInfo vkFramebufferCreateInfo = { VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO, nullptr, 0 };
    vkFramebufferCreateInfo.renderPass = renderPass;
    vkFramebufferCreateInfo.height = height;
    vkFramebufferCreateInfo.width = width;
    vkFramebufferCreateInfo.layers = layers;
    vkFramebufferCreateInfo.attachmentCount = static_cast<uint32_t>(vkImageViews.Size());
    vkFramebufferCreateInfo.pAttachments = vkImageViews.Data();

    VkResult result = VulkanApi::GetApi()->vkCreateFramebuffer(_pDevice->GetDeviceHandle(), &vkFramebufferCreateInfo, nullptr, &_vkFrameBuffer);

    if (result != VK_SUCCESS)
        throw BackendException("Error failed to create vulkan framebuffer");

}

VulkanFrameBuffer::~VulkanFrameBuffer()
{
    if (_vkFrameBuffer)
        VulkanApi::GetApi()->vkDestroyFramebuffer(_pDevice->GetDeviceHandle(), _vkFrameBuffer, nullptr);
}


}
