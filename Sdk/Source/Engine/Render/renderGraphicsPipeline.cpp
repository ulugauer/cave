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

/// @file renderGraphicsPipeline.cpp
///       Render graphics pipeline interface

#include "renderGraphicsPipeline.h"
#include "renderDevice.h"
#include "renderShader.h"
#include "renderMaterial.h"
#include "renderVertexInput.h"
#include "renderInputAssembly.h"
#include "renderViewportScissor.h"
#include "renderRasterizerState.h"
#include "renderMultisample.h"
#include "renderDepthStencil.h"
#include "renderColorBlend.h"
#include "renderDynamicState.h"
#include "renderPipelineLayout.h"
#include "renderRenderPass.h"
#include "engineError.h"
#include "halGraphicsPipeline.h"
#include "halShader.h"

#include "Common/caveVector.h"

#include <cassert>

namespace cave
{
RenderGraphicsPipeline::RenderGraphicsPipeline(RenderDevice& renderDevice
	, RenderGraphicsPipelineInfo& graphicsPipelineInfo)
	: CaveRefCount(renderDevice.GetEngineAllocator())
	, _renderDevice(renderDevice)
{
	HalGraphicsPipelineInfo graphicsPipeline;

	// temp allocation
	caveVector<HalShader*> stages(renderDevice.GetEngineAllocator());
	if (graphicsPipelineInfo._material)
	{
		graphicsPipelineInfo._material->Update(); // compile if needed
		stages.Reserve(graphicsPipelineInfo._material->GetStageCount());
		if (graphicsPipelineInfo._material->_vertexShader)
			stages.Push(graphicsPipelineInfo._material->_vertexShader->GetHalHandle());
		if (graphicsPipelineInfo._material->_fragmentShader)
			stages.Push(graphicsPipelineInfo._material->_fragmentShader->GetHalHandle());
	}

	graphicsPipeline._stageCount = static_cast<uint32_t>(stages.Size());
	graphicsPipeline._pStages = stages.Data();

	// we need at least a vertex program
	assert(graphicsPipeline._stageCount);

	if (graphicsPipelineInfo._vertexInput)
		graphicsPipeline._vertexInput = graphicsPipelineInfo._vertexInput->GetHalHandle();
	if (graphicsPipelineInfo._inputAssembly)
		graphicsPipeline._inputAssembly = graphicsPipelineInfo._inputAssembly->GetHalHandle();
	if (graphicsPipelineInfo._viewport)
		graphicsPipeline._viewport = graphicsPipelineInfo._viewport->GetHalHandle();
	if (graphicsPipelineInfo._raterizer)
		graphicsPipeline._raterizer = graphicsPipelineInfo._raterizer->GetHalHandle();
	if (graphicsPipelineInfo._multisample)
		graphicsPipeline._multisample = graphicsPipelineInfo._multisample->GetHalHandle();
	if (graphicsPipelineInfo._depthStencil)
		graphicsPipeline._depthStencil = graphicsPipelineInfo._depthStencil->GetHalHandle();
	if (graphicsPipelineInfo._colorBlend)
		graphicsPipeline._colorBlend = graphicsPipelineInfo._colorBlend->GetHalHandle();
	if (graphicsPipelineInfo._dynamicState)
		graphicsPipeline._dynamicState = graphicsPipelineInfo._dynamicState->GetHalHandle();
	if (graphicsPipelineInfo._layout)
		graphicsPipeline._layout = graphicsPipelineInfo._layout->GetHalHandle();
	if (graphicsPipelineInfo._renderPass)
		graphicsPipeline._renderPass = graphicsPipelineInfo._renderPass->GetHalHandle();
	
	// Allocate low level object
	_halGraphicsPipeline = renderDevice.GetHalRenderDevice()->CreateGraphicsPipeline(graphicsPipeline);
	assert(_halGraphicsPipeline);
}

RenderGraphicsPipeline::~RenderGraphicsPipeline()
{
	if (_halGraphicsPipeline)
		DeallocateDelete(*_renderDevice.GetEngineAllocator(), *_halGraphicsPipeline);
}

void RenderGraphicsPipeline::Update()
{
	if (_halGraphicsPipeline)
		_halGraphicsPipeline->CompilePipeline();
}

}
