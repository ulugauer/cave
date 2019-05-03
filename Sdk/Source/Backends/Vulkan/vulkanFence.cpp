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

/// @file vulkanFence.cpp
///       vulkan fence abstraction

#include "vulkanFence.h"
#include "vulkanRenderDevice.h"
#include "vulkanConversion.h"
#include "vulkanApi.h"

#include<limits>

namespace cave
{


VulkanFence::VulkanFence(VulkanRenderDevice* device, HalFenceDesc& fenceDesc)
    : HalFence()
    , _pDevice(device)
    , _vkFence(VK_NULL_HANDLE)
{
    VkFenceCreateInfo createInfo;
    createInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    createInfo.pNext = nullptr;
    createInfo.flags = VulkanTypeConversion::ConvertFenceCreateFlagsToVulkan(fenceDesc._createFlags);

    VulkanApi::GetApi()->vkCreateFence(_pDevice->GetDeviceHandle(), &createInfo, nullptr, &_vkFence);
    assert(_vkFence != VK_NULL_HANDLE);
}

VulkanFence::~VulkanFence()
{
    if (_vkFence != VK_NULL_HANDLE)
        VulkanApi::GetApi()->vkDestroyFence(_pDevice->GetDeviceHandle(), _vkFence, nullptr);
}


}
