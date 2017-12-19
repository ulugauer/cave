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
	, _vkRenderPass(nullptr)
{
	// Setup render pass struct
	_vkRenderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	_vkRenderPassInfo.pNext = nullptr;
	_vkRenderPassInfo.flags = 0;

	// Convert attachments to native
	_vkRenderPassInfo.pAttachments = nullptr;
	_vkRenderPassInfo.attachmentCount = renderPassInfo._attachmentCount;
	if (renderPassInfo._attachmentCount > 0)
	{
		// allocte memory
		size_t attachmentArraySize = renderPassInfo._attachmentCount * sizeof(VkAttachmentDescription);
		VkAttachmentDescription *attachments = static_cast<VkAttachmentDescription*>(device->GetEngineAllocator()->Allocate(attachmentArraySize, 4));
		if (attachments)
		{
			for (size_t i = 0; i < renderPassInfo._attachmentCount; ++i)
			{
				attachments[i].flags = 0;
				attachments[i].format = VulkanTypeConversion::ConvertImageFormatToVulkan(renderPassInfo._pAttachments[i]._format);
				attachments[i].samples = VulkanTypeConversion::ConvertSampleCountToVulkan(renderPassInfo._pAttachments[i]._samples);
				attachments[i].loadOp = VulkanTypeConversion::ConvertAttachmentLoadOpToVulkan(renderPassInfo._pAttachments[i]._loadOp);
				attachments[i].storeOp = VulkanTypeConversion::ConvertAttachmentStoreOpToVulkan(renderPassInfo._pAttachments[i]._storeOp);
				attachments[i].stencilLoadOp = VulkanTypeConversion::ConvertAttachmentLoadOpToVulkan(renderPassInfo._pAttachments[i]._loadStencilOp);
				attachments[i].stencilStoreOp = VulkanTypeConversion::ConvertAttachmentStoreOpToVulkan(renderPassInfo._pAttachments[i]._storeStencilOp);
				attachments[i].initialLayout = VulkanTypeConversion::ConvertImageLayoutToVulkan(renderPassInfo._pAttachments[i]._initialLayout);
				attachments[i].finalLayout = VulkanTypeConversion::ConvertImageLayoutToVulkan(renderPassInfo._pAttachments[i]._finalLayout);
			}

			_vkRenderPassInfo.pAttachments = attachments;
		}
	}

	// Convert to native subpass description
	_vkRenderPassInfo.pSubpasses = nullptr;
	_vkRenderPassInfo.subpassCount = renderPassInfo._subpassCount;
	if (renderPassInfo._subpassCount > 0)
	{
		// allocte memory
		size_t subpassArraySize = renderPassInfo._subpassCount * sizeof(VkSubpassDescription);
		VkSubpassDescription *subpass = static_cast<VkSubpassDescription*>(device->GetEngineAllocator()->Allocate(subpassArraySize, 4));
		if (subpass)
		{
			for (size_t i = 0; i < renderPassInfo._subpassCount; ++i)
			{
				subpass[i].pipelineBindPoint = VulkanTypeConversion::ConvertPipelineBindPointToVulkan(renderPassInfo._pSubpasses[i]._pipelineBindPoint);
				subpass[i].colorAttachmentCount = renderPassInfo._pSubpasses[i]._colorAttachmentCount;
				subpass[i].pColorAttachments = nullptr;
				subpass[i].pResolveAttachments = nullptr;
				subpass[i].pDepthStencilAttachment = nullptr;
				// allocte memory
				// Convert to native attachment reference
				size_t colorRefArraySize = renderPassInfo._pSubpasses[i]._colorAttachmentCount * sizeof(VkAttachmentReference);
				VkAttachmentReference *colorRef = static_cast<VkAttachmentReference *>(device->GetEngineAllocator()->Allocate(colorRefArraySize, 4));
				if (colorRef)
				{
					for (size_t j = 0; j < renderPassInfo._pSubpasses[i]._colorAttachmentCount; ++j)
					{
						colorRef[j].attachment = renderPassInfo._pSubpasses[i]._pColorAttachments[j]._attachment;
						colorRef[j].layout = VulkanTypeConversion::ConvertImageLayoutToVulkan(renderPassInfo._pSubpasses[i]._pColorAttachments[j]._layout);
					}

					subpass[i].pColorAttachments = colorRef;
				}

				if (renderPassInfo._pSubpasses[i]._pResolveAttachments)
				{
					VkAttachmentReference *resolveRef = static_cast<VkAttachmentReference *>(device->GetEngineAllocator()->Allocate(colorRefArraySize, 4));
					if (resolveRef)
					{
						for (size_t j = 0; j < renderPassInfo._pSubpasses[i]._colorAttachmentCount; ++j)
						{
							resolveRef[j].attachment = renderPassInfo._pSubpasses[i]._pColorAttachments[j]._attachment;
							resolveRef[j].layout = VulkanTypeConversion::ConvertImageLayoutToVulkan(renderPassInfo._pSubpasses[i]._pColorAttachments[j]._layout);
						}

						subpass[i].pResolveAttachments = resolveRef;
					}
				}

				if (renderPassInfo._pSubpasses[i]._pDepthStencilAttachment)
				{
					size_t depthStencilArraySize = sizeof(VkAttachmentReference); // only one
					VkAttachmentReference  *depthStencilRef = static_cast<VkAttachmentReference *>(device->GetEngineAllocator()->Allocate(depthStencilArraySize, 4));
					if (depthStencilRef)
					{
						depthStencilRef->attachment = renderPassInfo._pSubpasses[i]._pDepthStencilAttachment->_attachment;
						depthStencilRef->layout = VulkanTypeConversion::ConvertImageLayoutToVulkan(renderPassInfo._pSubpasses[i]._pDepthStencilAttachment->_layout);
						
						subpass[i].pDepthStencilAttachment = depthStencilRef;
					}
				}

				// the below is not handled yet
				assert(renderPassInfo._pSubpasses[i]._inputAttachmentCount == 0);
				subpass[i].inputAttachmentCount = 0;
				subpass[i].pInputAttachments = nullptr;
				assert(renderPassInfo._pSubpasses[i]._preserveAttachmentCount == 0);
				subpass[i].preserveAttachmentCount = 0;
				subpass[i].pPreserveAttachments = nullptr;
			}

			_vkRenderPassInfo.pSubpasses = subpass;
		}
	}

	// convert dependencies
	_vkRenderPassInfo.dependencyCount = renderPassInfo._dependencyCount;
	if (renderPassInfo._dependencyCount > 0)
	{
		// allocte memory
		size_t dependencyArraySize = renderPassInfo._dependencyCount * sizeof(VkAttachmentDescription);
		VkSubpassDependency *dependencies = static_cast<VkSubpassDependency*>(device->GetEngineAllocator()->Allocate(dependencyArraySize, 4));
		if (dependencies)
		{
			for (size_t i = 0; i < renderPassInfo._dependencyCount; ++i)
			{
				dependencies[i].dependencyFlags = VulkanTypeConversion::ConvertDependencyFlagsToVulkan(renderPassInfo._pDependencies[i]._dependencyFlags);
				dependencies[i].srcSubpass = renderPassInfo._pDependencies[i]._srcSubpass;
				dependencies[i].dstSubpass = renderPassInfo._pDependencies[i]._dstSubpass;
				dependencies[i].srcStageMask = VulkanTypeConversion::ConvertPipelineFlagsToVulkan(renderPassInfo._pDependencies[i]._srcStageMask);
				dependencies[i].srcAccessMask = VulkanTypeConversion::ConvertAccessFlagsToVulkan(renderPassInfo._pDependencies[i]._srcAccessMask);
				dependencies[i].dstStageMask = VulkanTypeConversion::ConvertPipelineFlagsToVulkan(renderPassInfo._pDependencies[i]._dstStageMask);
				dependencies[i].dstAccessMask = VulkanTypeConversion::ConvertAccessFlagsToVulkan(renderPassInfo._pDependencies[i]._dstAccessMask);
			}
		}

		_vkRenderPassInfo.pDependencies = dependencies;
	}
}

VulkanRenderPass::~VulkanRenderPass()
{
	for (size_t i = 0; i < _vkRenderPassInfo.subpassCount; ++i)
	{
		if (_vkRenderPassInfo.pSubpasses[i].pColorAttachments)
			_pDevice->GetEngineAllocator()->Deallocate((void *)_vkRenderPassInfo.pSubpasses[i].pColorAttachments);
		if (_vkRenderPassInfo.pSubpasses[i].pResolveAttachments)
			_pDevice->GetEngineAllocator()->Deallocate((void *)_vkRenderPassInfo.pSubpasses[i].pResolveAttachments);
		if (_vkRenderPassInfo.pSubpasses[i].pDepthStencilAttachment)
			_pDevice->GetEngineAllocator()->Deallocate((void *)_vkRenderPassInfo.pSubpasses[i].pDepthStencilAttachment);
		if (_vkRenderPassInfo.pSubpasses[i].pInputAttachments)
			_pDevice->GetEngineAllocator()->Deallocate((void *)_vkRenderPassInfo.pSubpasses[i].pInputAttachments);
		if (_vkRenderPassInfo.pSubpasses[i].pPreserveAttachments)
			_pDevice->GetEngineAllocator()->Deallocate((void *)_vkRenderPassInfo.pSubpasses[i].pPreserveAttachments);
	}

	if (_vkRenderPassInfo.pAttachments != nullptr)
		_pDevice->GetEngineAllocator()->Deallocate((void *)_vkRenderPassInfo.pAttachments);
	if (_vkRenderPassInfo.pSubpasses != nullptr)
		_pDevice->GetEngineAllocator()->Deallocate((void *)_vkRenderPassInfo.pSubpasses);
	if (_vkRenderPassInfo.pDependencies != nullptr)
		_pDevice->GetEngineAllocator()->Deallocate((void *)_vkRenderPassInfo.pDependencies);

	if (_vkRenderPass != VK_NULL_HANDLE)
		VulkanApi::GetApi()->vkDestroyRenderPass(_pDevice->GetDeviceHandle(), _vkRenderPass, nullptr);
}

VkRenderPass VulkanRenderPass::GetRenderPass()
{
	if (_vkRenderPass == VK_NULL_HANDLE)
	{
		// create
		VulkanApi::GetApi()->vkCreateRenderPass(_pDevice->GetDeviceHandle(), &_vkRenderPassInfo, nullptr, &_vkRenderPass);
		assert(_vkRenderPass != VK_NULL_HANDLE);
	}

	return _vkRenderPass;
}

}
