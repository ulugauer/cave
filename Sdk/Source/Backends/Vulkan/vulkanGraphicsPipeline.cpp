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

/// @file vulkanGraphicsPipeline.cpp
///       Vulkan graphics pipeline

#include "vulkanGraphicsPipeline.h"
#include "vulkanRenderDevice.h"
#include "vulkanConversion.h"
#include "vulkanShader.h"
#include "vulkanVertexInput.h"
#include "vulkanInputAssembly.h"
#include "vulkanViewportAndScissor.h"
#include "vulkanRasterizerState.h"
#include "vulkanMultisample.h"
#include "vulkanDepthStencil.h"
#include "vulkanColorBlend.h"
#include "vulkanDynamicState.h"
#include "vulkanPipelineLayout.h"
#include "vulkanRenderPass.h"

#include "vulkanApi.h"

#include<limits>
#include<cassert>

namespace cave
{

VulkanGraphicsPipeline::VulkanGraphicsPipeline(VulkanRenderDevice* device
	, HalGraphicsPipelineInfo& graphicsPipelineInfo)
	: HalGraphicsPipeline(device, graphicsPipelineInfo)
	, _pDevice(device)
	, _vkPipeline(VK_NULL_HANDLE)
{
	_vkGraphicsPipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	_vkGraphicsPipelineInfo.pNext = nullptr;
	_vkGraphicsPipelineInfo.flags = 0;

	_vkGraphicsPipelineInfo.pTessellationState = nullptr;
	_vkGraphicsPipelineInfo.subpass = graphicsPipelineInfo._subpass;
	_vkGraphicsPipelineInfo.basePipelineIndex = graphicsPipelineInfo._basePipelineIndex;
	_vkGraphicsPipelineInfo.basePipelineHandle = nullptr;

	_vkGraphicsPipelineInfo.stageCount = 0;
	_vkGraphicsPipelineInfo.pStages = nullptr;
	if (graphicsPipelineInfo._stageCount > 0)
	{
		// allocte memory
		size_t shaderStageArraySize = graphicsPipelineInfo._stageCount * sizeof(VkPipelineShaderStageCreateInfo);
		VkPipelineShaderStageCreateInfo *stages = static_cast<VkPipelineShaderStageCreateInfo*>(device->GetEngineAllocator()->Allocate(shaderStageArraySize, 4));
		if (stages)
		{
			for (size_t i = 0; i < graphicsPipelineInfo._stageCount; ++i)
			{
				VulkanShader* shader = static_cast<VulkanShader*>(graphicsPipelineInfo._pStages[i]);
				shader->GetShaderStageInfo(stages[i]);
			}
			_vkGraphicsPipelineInfo.stageCount = graphicsPipelineInfo._stageCount;
			_vkGraphicsPipelineInfo.pStages = stages;
		}
	}

	if (graphicsPipelineInfo._vertexInput)
	{
		const VulkanVertexInput* vertexInput = static_cast<VulkanVertexInput*>(graphicsPipelineInfo._vertexInput);
		_vkGraphicsPipelineInfo.pVertexInputState = &vertexInput->GetVertexInputStateInfo();
	}
	else
		_vkGraphicsPipelineInfo.pVertexInputState = nullptr;

	if (graphicsPipelineInfo._inputAssembly)
	{
		const VulkanInputAssembly* inputAssembly = static_cast<VulkanInputAssembly*>(graphicsPipelineInfo._inputAssembly);
		_vkGraphicsPipelineInfo.pInputAssemblyState = &inputAssembly->GetVertexInputStateInfo();
	}
	else
		_vkGraphicsPipelineInfo.pInputAssemblyState = nullptr;

	if (graphicsPipelineInfo._viewport)
	{
		const VulkanViewportAndScissor* viewport = static_cast<VulkanViewportAndScissor*>(graphicsPipelineInfo._viewport);
		_vkGraphicsPipelineInfo.pViewportState = &viewport->GetViewportStateInfo();
	}
	else
		_vkGraphicsPipelineInfo.pViewportState = nullptr;

	if (graphicsPipelineInfo._raterizer)
	{
		const VulkanRasterizerState* raterizer = static_cast<VulkanRasterizerState*>(graphicsPipelineInfo._raterizer);
		_vkGraphicsPipelineInfo.pRasterizationState = &raterizer->GetRasterizerStateInfo();
	}
	else
		_vkGraphicsPipelineInfo.pRasterizationState = nullptr;

	if (graphicsPipelineInfo._multisample)
	{
		const VulkanMultisample* multisample = static_cast<VulkanMultisample*>(graphicsPipelineInfo._multisample);
		_vkGraphicsPipelineInfo.pMultisampleState = &multisample->GetMultisampleStateInfo();
	}
	else
		_vkGraphicsPipelineInfo.pMultisampleState = nullptr;

	if (graphicsPipelineInfo._depthStencil)
	{
		const VulkanDepthStencil* depthStencil = static_cast<VulkanDepthStencil*>(graphicsPipelineInfo._depthStencil);
		_vkGraphicsPipelineInfo.pDepthStencilState = &depthStencil->GetDepthStencilStateInfo();
	}
	else
		_vkGraphicsPipelineInfo.pDepthStencilState = nullptr;

	if (graphicsPipelineInfo._colorBlend)
	{
		const VulkanColorBlend* colorBlend = static_cast<VulkanColorBlend*>(graphicsPipelineInfo._colorBlend);
		_vkGraphicsPipelineInfo.pColorBlendState = &colorBlend->GetColorBlendStateInfo();
	}
	else
		_vkGraphicsPipelineInfo.pColorBlendState = nullptr;

	if (graphicsPipelineInfo._dynamicState)
	{
		const VulkanDynamicState* dynamicState = static_cast<VulkanDynamicState*>(graphicsPipelineInfo._dynamicState);
		_vkGraphicsPipelineInfo.pDynamicState = &dynamicState->GetDynamicStateInfo();
	}
	else
		_vkGraphicsPipelineInfo.pDynamicState = nullptr;

	if (graphicsPipelineInfo._layout)
	{
		VulkanPipelineLayout* layout = static_cast<VulkanPipelineLayout*>(graphicsPipelineInfo._layout);
		_vkGraphicsPipelineInfo.layout = layout->GetPipelineLayout();
	}
	else
		_vkGraphicsPipelineInfo.layout = nullptr;

	if (graphicsPipelineInfo._renderPass)
	{
		VulkanRenderPass* renderPass = static_cast<VulkanRenderPass*>(graphicsPipelineInfo._renderPass);
		_vkGraphicsPipelineInfo.renderPass = renderPass->GetRenderPass();
	}
	else
		_vkGraphicsPipelineInfo.renderPass = nullptr;
}

VulkanGraphicsPipeline::~VulkanGraphicsPipeline()
{
	if (_vkGraphicsPipelineInfo.pStages)
		_pDevice->GetEngineAllocator()->Deallocate((void *)_vkGraphicsPipelineInfo.pStages);

	if (_vkPipeline != VK_NULL_HANDLE)
		VulkanApi::GetApi()->vkDestroyPipeline(_pDevice->GetDeviceHandle(), _vkPipeline, nullptr);
}

VkPipeline VulkanGraphicsPipeline::GetGraphicsPipeline()
{
	if (_vkPipeline == VK_NULL_HANDLE)
	{
		CompilePipeline();
	}

	return _vkPipeline;
}

bool VulkanGraphicsPipeline::CompilePipeline()
{
	if (_vkPipeline != VK_NULL_HANDLE)
		return true;

	// create
	VkResult result = VulkanApi::GetApi()->vkCreateGraphicsPipelines(_pDevice->GetDeviceHandle(), VK_NULL_HANDLE, 1, &_vkGraphicsPipelineInfo, nullptr, &_vkPipeline);
	assert(_vkPipeline != VK_NULL_HANDLE);

	return (result == VK_SUCCESS);
}

}
