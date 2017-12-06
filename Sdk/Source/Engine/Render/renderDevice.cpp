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

/// @file renderDevice.cpp
///       Render instance abstraction

#include "renderDevice.h"
#include "renderInstance.h"
#include "renderVertexInput.h"
#include "renderInputAssembly.h"
#include "renderLayerSection.h"
#include "renderRasterizerState.h"
#include "renderMultisample.h"
#include "renderDepthStencil.h"
#include "renderColorBlend.h"
#include "renderDynamicState.h"
#include "renderPipelineLayout.h"
#include "halRenderDevice.h"
#include "engineError.h"

namespace cave
{

RenderDevice::RenderDevice(RenderInstance* renderInstance, HalInstance* halInstance, FrontendWindowInfo& windowInfo)
	: _pRenderInstance(renderInstance)
	, _pHalInstance(halInstance)
	, _pHalRenderDevice(nullptr)
	, _pResourceManager(nullptr)
{
	// first copy data
	_swapChainInfo.colorBits = windowInfo.colorBits;
	_swapChainInfo.depthBits = windowInfo.depthBits;
	_swapChainInfo.fullscreen = windowInfo.fullscreen;
	_swapChainInfo.offscreen = windowInfo.offscreen;
	_swapChainInfo.surfaceHeight = windowInfo.windowHeight;
	_swapChainInfo.surfaceWidth = windowInfo.windowWidth;
#ifdef _WIN32
	_swapChainInfo.hInstance = windowInfo.hInstance;
	_swapChainInfo.hWindow = windowInfo.hWindow;
#else
	_swapChainInfo.connection = windowInfo.connection;
	_swapChainInfo.visualId = windowInfo.visualId;
	_swapChainInfo.window = windowInfo.window;
#endif

	try
	{
		_pHalRenderDevice = halInstance->CreateRenderDevice(renderInstance->GetEngineAllocator(), _swapChainInfo);
		_pResourceManager = AllocateObject<ResourceManager>(*renderInstance->GetEngineAllocator()
				, this
				, renderInstance->GetApplicationPath()
				, renderInstance->GetProjectPath() );
	}
	catch (std::exception& e)
	{
		std::string msg(e.what());
		throw EngineError(msg);
	}
}

RenderDevice::~RenderDevice()
{
	if (_pResourceManager)
		DeallocateDelete(*_pRenderInstance->GetEngineAllocator(), *_pResourceManager);

	if (_pHalRenderDevice)
		DeallocateDelete(*_pRenderInstance->GetEngineAllocator(), *_pHalRenderDevice);
}

std::shared_ptr<AllocatorGlobal>
RenderDevice::GetEngineAllocator()
{
	return _pRenderInstance->GetEngineAllocator();
}

EngineLog* RenderDevice::GetEngineLog() const
{
	return _pRenderInstance->GetEngineLog();
}

void RenderDevice::CreateSwapChain()
{
	if (!_pHalRenderDevice || !_pRenderInstance || !_pHalInstance)
		throw EngineError("Render device not properly setup");

	try
	{
		_pHalRenderDevice->CreateSwapChain(_swapChainInfo);
	}
	catch (std::exception& e)
	{
		std::string msg(e.what());
		throw EngineError(msg);
	}
}

RenderVertexInput* RenderDevice::CreateVertexInput()
{
	RenderVertexInput* vertexInput = AllocateObject<RenderVertexInput>(*_pRenderInstance->GetEngineAllocator(), *this);
	if (vertexInput)
		vertexInput->IncrementUsageCount();

	return vertexInput;
}

void RenderDevice::ReleaseVertexInput(RenderVertexInput* vertexInput)
{
	if (vertexInput)
	{
		int32_t refCount = vertexInput->DecrementUsageCount();
		if (refCount == 0)
			DeallocateDelete(*_pRenderInstance->GetEngineAllocator(), *vertexInput);
	}
}

RenderInputAssembly* RenderDevice::CreateInputAssembly()
{
	RenderInputAssembly* inputAssembly = AllocateObject<RenderInputAssembly>(*_pRenderInstance->GetEngineAllocator(), *this);
	if (inputAssembly)
		inputAssembly->IncrementUsageCount();

	return inputAssembly;
}

void RenderDevice::ReleaseInputAssembly(RenderInputAssembly* inputAssembly)
{
	if (inputAssembly)
	{
		int32_t refCount = inputAssembly->DecrementUsageCount();
		if (refCount == 0)
			DeallocateDelete(*_pRenderInstance->GetEngineAllocator(), *inputAssembly);
	}
}

RenderLayerSection* RenderDevice::CreateLayerSection(RenderLayerSectionInfo& sectionInfo)
{
	RenderLayerSection* layerSection = AllocateObject<RenderLayerSection>(*_pRenderInstance->GetEngineAllocator(), *this, sectionInfo);

	return layerSection;
}

void RenderDevice::ReleaseLayerSection(RenderLayerSection* layerSection)
{
	if (layerSection)
		DeallocateDelete(*_pRenderInstance->GetEngineAllocator(), *layerSection);
}

RenderRasterizerState* RenderDevice::CreateRasterizerState(HalRasterizerSetup& rasterizerInfo)
{
	RenderRasterizerState* rasterizerState = AllocateObject<RenderRasterizerState>(*_pRenderInstance->GetEngineAllocator(), *this, rasterizerInfo);
	if (rasterizerState)
		rasterizerState->IncrementUsageCount();

	return rasterizerState;
}

void RenderDevice::ReleaseRasterizerState(RenderRasterizerState* rasterizerState)
{
	if (rasterizerState)
	{
		int32_t refCount = rasterizerState->DecrementUsageCount();
		if (refCount == 0)
			DeallocateDelete(*_pRenderInstance->GetEngineAllocator(), *rasterizerState);
	}
}

RenderMultisample* RenderDevice::CreateMultisampleState(HalMultisampleState& multisampleInfo)
{
	RenderMultisample* multisampleState = AllocateObject<RenderMultisample>(*_pRenderInstance->GetEngineAllocator(), *this, multisampleInfo);
	if (multisampleState)
		multisampleState->IncrementUsageCount();

	return multisampleState;
}

void RenderDevice::ReleaseMultisampleState(RenderMultisample* multisampleState)
{
	if (multisampleState)
	{
		int32_t refCount = multisampleState->DecrementUsageCount();
		if (refCount == 0)
			DeallocateDelete(*_pRenderInstance->GetEngineAllocator(), *multisampleState);
	}
}

RenderDepthStencil* RenderDevice::CreateDepthStencilState(HalDepthStencilSetup& depthStencilInfo)
{
	RenderDepthStencil* depthStencilState = AllocateObject<RenderDepthStencil>(*_pRenderInstance->GetEngineAllocator(), *this, depthStencilInfo);
	if (depthStencilState)
		depthStencilState->IncrementUsageCount();

	return depthStencilState;
}

void RenderDevice::ReleaseDepthStencilState(RenderDepthStencil* depthStencilState)
{
	if (depthStencilState)
	{
		int32_t refCount = depthStencilState->DecrementUsageCount();
		if (refCount == 0)
			DeallocateDelete(*_pRenderInstance->GetEngineAllocator(), *depthStencilState);
	}
}

RenderColorBlend* RenderDevice::CreateColorBlendState(HalColorBlendState& colorBlendInfo
	, caveVector<HalColorBlendAttachment>& blendAttachments)
{
	RenderColorBlend* colorBlendState = AllocateObject<RenderColorBlend>(*_pRenderInstance->GetEngineAllocator(), *this, colorBlendInfo, blendAttachments);
	if (colorBlendState)
		colorBlendState->IncrementUsageCount();

	return colorBlendState;
}

void RenderDevice::ReleaseColorBlendState(RenderColorBlend* colorBlendState)
{
	if (colorBlendState)
	{
		int32_t refCount = colorBlendState->DecrementUsageCount();
		if (refCount == 0)
			DeallocateDelete(*_pRenderInstance->GetEngineAllocator(), *colorBlendState);
	}
}

RenderDynamicState* RenderDevice::CreateDynamicState(caveVector<HalDynamicStates>& dynamicStates)
{
	RenderDynamicState* dynamicState = AllocateObject<RenderDynamicState>(*_pRenderInstance->GetEngineAllocator(), *this, dynamicStates);
	if (dynamicState)
		dynamicState->IncrementUsageCount();

	return dynamicState;
}

void RenderDevice::ReleaseDynamicState(RenderDynamicState* dynamicState)
{
	if (dynamicState)
	{
		int32_t refCount = dynamicState->DecrementUsageCount();
		if (refCount == 0)
			DeallocateDelete(*_pRenderInstance->GetEngineAllocator(), *dynamicState);
	}
}

RenderPipelineLayout* RenderDevice::CreatePipelineLayout(caveVector<HalDescriptorSetLayout>& descriptorSetLayouts
	, caveVector<HalPushConstantRange>& pushConstants)
{
	RenderPipelineLayout* pipelineLayout = AllocateObject<RenderPipelineLayout>(*_pRenderInstance->GetEngineAllocator(), *this, descriptorSetLayouts, pushConstants);
	if (pipelineLayout)
		pipelineLayout->IncrementUsageCount();

	return pipelineLayout;
}

void RenderDevice::ReleasePipelineLayout(RenderPipelineLayout* pipelineLayout)
{
	if (pipelineLayout)
	{
		int32_t refCount = pipelineLayout->DecrementUsageCount();
		if (refCount == 0)
			DeallocateDelete(*_pRenderInstance->GetEngineAllocator(), *pipelineLayout);
	}
}

}