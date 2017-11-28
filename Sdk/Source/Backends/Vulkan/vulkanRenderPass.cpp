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

/// @file vulkanRenderPass.cpp
///       Hardware render pass abstraction

#include "vulkanRenderPass.h"
#include "vulkanRenderDevice.h"
#include "vulkanConversion.h"
#include "Memory/allocatorBase.h"

#include "vulkanApi.h"

#include<limits>

namespace cave
{


VulkanRenderPass::VulkanRenderPass(VulkanRenderDevice* device, HalRenderPassInfo& renderPassInfo)
	: HalRenderPass(device, renderPassInfo)
	, _pDevice(device)
{
	// Setup render pass struct
	_vkRenderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	_vkRenderPassInfo.pNext = nullptr;
	_vkRenderPassInfo.flags = 0;

	// Convert attachments to native
	_vkRenderPassInfo.attachmentCount = renderPassInfo._attachmentCount;
	if (renderPassInfo._attachmentCount > 0)
	{
		// allocte memory
		size_t attachmentArraySize = renderPassInfo._attachmentCount * sizeof(VkAttachmentDescription);
		_vkRenderPassInfo.pAttachments = static_cast<VkAttachmentDescription*>(device->GetEngineAllocator()->Allocate(attachmentArraySize, 4));
		if (_vkRenderPassInfo.pAttachments)
		{
			for (size_t i = 0; i < renderPassInfo._attachmentCount; ++i)
			{

			}
		}
	}

}

VulkanRenderPass::~VulkanRenderPass()
{
	if (_vkRenderPassInfo.pAttachments != nullptr)
		_pDevice->GetEngineAllocator()->Deallocate((void *)_vkRenderPassInfo.pAttachments);
}


}
