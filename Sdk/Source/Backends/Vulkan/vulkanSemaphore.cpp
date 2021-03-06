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

/// @file vulkanSemaphore.cpp
///       vulkan semaphore abstraction

#include "vulkanSemaphore.h"
#include "vulkanRenderDevice.h"
#include "vulkanApi.h"

#include<limits>

namespace cave
{


VulkanSemaphore::VulkanSemaphore(VulkanRenderDevice* device, HalSemaphoreDesc& )
	: HalSemaphore()
	, _pDevice(device)
{
	VkSemaphoreCreateInfo createInfo;
	createInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
	createInfo.pNext = nullptr;
	createInfo.flags = 0;

	VulkanApi::GetApi()->vkCreateSemaphore(_pDevice->GetDeviceHandle(), &createInfo, nullptr, &_vkSemaphore);
	assert(_vkSemaphore != VK_NULL_HANDLE);
}

VulkanSemaphore::~VulkanSemaphore()
{
	if (_vkSemaphore != VK_NULL_HANDLE)
		VulkanApi::GetApi()->vkDestroySemaphore(_pDevice->GetDeviceHandle(), _vkSemaphore, nullptr);
}


}
