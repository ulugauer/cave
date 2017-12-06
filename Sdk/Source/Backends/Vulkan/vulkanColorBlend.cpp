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

/// @file vulkanColorBlend.cpp
///       Vulkan color blending state

#include "vulkanColorBlend.h"
#include "vulkanRenderDevice.h"
#include "vulkanConversion.h"

#include "vulkanApi.h"

#include<limits>

namespace cave
{

VulkanColorBlend::VulkanColorBlend(VulkanRenderDevice* device
								, HalColorBlendState& colorBlendState
								, caveVector<HalColorBlendAttachment>& blendAttachments)
	: HalColorBlend(device, colorBlendState, blendAttachments)
	, _pDevice(device)
	, _colorBlendAttachments(device->GetEngineAllocator())
{
	// first convert attachments
	if (!blendAttachments.Empty())
	{
		_colorBlendAttachments.Reserve(blendAttachments.Size());
		for (size_t i = 0; i < blendAttachments.Size(); ++i)
		{
			VkPipelineColorBlendAttachmentState attachment;
			attachment.alphaBlendOp = VulkanTypeConversion::ConvertBlendOpToVulkan(blendAttachments[i]._alphaBlendOp);
			attachment.colorBlendOp = VulkanTypeConversion::ConvertBlendOpToVulkan(blendAttachments[i]._colorBlendOp);
			attachment.blendEnable = blendAttachments[i]._blendEnable;
			attachment.colorWriteMask = VulkanTypeConversion::ConvertColorComponentToVulkan(blendAttachments[i]._colorWriteMask);
			attachment.dstAlphaBlendFactor = VulkanTypeConversion::ConvertBlendFactorToVulkan(blendAttachments[i]._dstAlphaBlendFactor);
			attachment.dstColorBlendFactor = VulkanTypeConversion::ConvertBlendFactorToVulkan(blendAttachments[i]._dstColorBlendFactor);
			attachment.srcAlphaBlendFactor = VulkanTypeConversion::ConvertBlendFactorToVulkan(blendAttachments[i]._srcAlphaBlendFactor);
			attachment.srcColorBlendFactor = VulkanTypeConversion::ConvertBlendFactorToVulkan(blendAttachments[i]._srcColorBlendFactor);

			_colorBlendAttachments.Push(attachment);
		}
	}

	_colorBlendStateStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	_colorBlendStateStateInfo.pNext = nullptr;
	_colorBlendStateStateInfo.flags = 0;
	_colorBlendStateStateInfo.logicOpEnable = colorBlendState._logicOpEnable;
	_colorBlendStateStateInfo.logicOp = VulkanTypeConversion::ConvertLogicOpToVulkan(colorBlendState._logicOp);
	_colorBlendStateStateInfo.attachmentCount = static_cast<uint32_t>(blendAttachments.Size());
	_colorBlendStateStateInfo.pAttachments = _colorBlendAttachments.Data();
	_colorBlendStateStateInfo.blendConstants[0] = colorBlendState._blendConstants[0];
	_colorBlendStateStateInfo.blendConstants[1] = colorBlendState._blendConstants[1];
	_colorBlendStateStateInfo.blendConstants[2] = colorBlendState._blendConstants[2];
	_colorBlendStateStateInfo.blendConstants[3] = colorBlendState._blendConstants[3];
}

VulkanColorBlend::~VulkanColorBlend()
{
	if (!_colorBlendAttachments.Empty())
		_colorBlendAttachments.Clear();
}


}
