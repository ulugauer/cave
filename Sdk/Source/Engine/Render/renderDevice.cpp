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
#include "renderCommandPool.h"
#include "renderDescriptorPool.h"
#include "renderDescriptorSet.h"
#include "renderVertexInput.h"
#include "renderInputAssembly.h"
#include "renderLayerSection.h"
#include "renderRasterizerState.h"
#include "renderMultisample.h"
#include "renderDepthStencil.h"
#include "renderColorBlend.h"
#include "renderDynamicState.h"
#include "renderPipelineLayout.h"
#include "renderRenderPass.h"
#include "renderGraphicsPipeline.h"
#include "renderVertexBuffer.h"
#include "renderIndexBuffer.h"
#include "renderUniformBuffer.h"
#include "renderTexture.h"
#include "renderTextureView.h"
#include "renderTextureSampler.h"
#include "renderRenderTarget.h"
#include "renderFrameBuffer.h"
#include "renderCommandBuffer.h"
#include "renderSemaphore.h"
#include "renderFence.h"
#include "halRenderDevice.h"
#include "engineError.h"
#include "engineLog.h"

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
	_swapChainInfo.stencilBits = windowInfo.stencilBits;
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

void RenderDevice::GetApiVersion(uint32_t& major, uint32_t& minor, uint32_t& patch)
{
	if (!_pHalRenderDevice)
		throw EngineError("Render device not properly setup");

	_pHalRenderDevice->GetApiVersion(major, minor, patch);
}

void RenderDevice::GetDriverVersion(uint32_t& major, uint32_t& minor, uint32_t& patch)
{
	if (!_pHalRenderDevice)
		throw EngineError("Render device not properly setup");

	_pHalRenderDevice->GetDriverVersion(major, minor, patch);
}

const char* RenderDevice::GetDeviceName()
{
	if (!_pHalRenderDevice)
		throw EngineError("Render device not properly setup");

	return _pHalRenderDevice->GetDeviceName();
}

RenderInstanceTypes RenderDevice::GetInstanceType() const
{
    RenderInstanceTypes type = RenderInstanceTypes::InstanceVulkan;
    if (_pHalInstance)
    {
        BackendInstanceTypes beType = _pHalInstance->GetInstanceType();
        if(beType == BackendInstanceTypes::InstanceDX12)
            type = RenderInstanceTypes::InstanceDX12;
    }

    return type;
}

void RenderDevice::WaitIdle()
{
	if (_pHalRenderDevice)
		_pHalRenderDevice->WaitIdle();
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

bool RenderDevice::CreateSwapChainFramebuffers()
{
	try
	{
		_pHalRenderDevice->CreateSwapChainFramebuffers();
	}
	catch (std::exception& )
	{
		return false;
	}

	return true;
}

const HalImageFormat RenderDevice::GetSwapChainImageFormat()
{
	HalImageFormat format = HalImageFormat::Undefined;
	if (_pHalRenderDevice)
		format = _pHalRenderDevice->GetSwapChainImageFormat();

	return format;
}

const HalImageFormat RenderDevice::GetSwapChainDepthImageFormat()
{
	HalImageFormat format = HalImageFormat::Undefined;
	if (_pHalRenderDevice)
		format = _pHalRenderDevice->GetSwapChainDepthImageFormat();

	return format;
}

const uint32_t RenderDevice::GetSwapChainImageCount()
{
	if (_pHalRenderDevice)
		return _pHalRenderDevice->GetSwapChainImageCount();

	return 0;
}

const uint32_t RenderDevice::AcquireNextSwapChainImage(uint64_t timeout)
{
	uint32_t imageIndex = (std::numeric_limits<uint32_t>::max)();
	if (_pHalRenderDevice)
		imageIndex = _pHalRenderDevice->AcquireNextSwapChainImage(timeout);

	return imageIndex;
}

RenderCommandPool* RenderDevice::CreateCommandPool(HalCommandPoolInfo& commandPoolInfo)
{
	RenderCommandPool* commandPool = AllocateObject<RenderCommandPool>(*_pRenderInstance->GetEngineAllocator(), *this, commandPoolInfo);
	if (commandPool)
		commandPool->AddRef();

	return  commandPool;
}

void RenderDevice::ReleaseCommandPool(RenderCommandPool* commandPool)
{
	if (commandPool)
	{
		commandPool->Relase();
	}
}

RenderDescriptorPool* RenderDevice::CreateDescriptorPool(HalDescriptorPoolInfo& deescriptorPoolInfo)
{
	RenderDescriptorPool* descriptorPool = AllocateObject<RenderDescriptorPool>(*_pRenderInstance->GetEngineAllocator(), *this, deescriptorPoolInfo);
	if (descriptorPool)
		descriptorPool->AddRef();

	return  descriptorPool;
}

void RenderDevice::ReleaseDescriptorPool(RenderDescriptorPool* descriptorPool)
{
	if (descriptorPool)
	{
		descriptorPool->Relase();
	}
}

RenderDescriptorSet* RenderDevice::CreateDescriptorSets(caveVector<HalDescriptorSetLayout>& descriptorSetLayouts)
{
	RenderDescriptorSet* descriptorSet = AllocateObject<RenderDescriptorSet>(*_pRenderInstance->GetEngineAllocator(), *this, descriptorSetLayouts);
	if (descriptorSet)
		descriptorSet->AddRef();

	return  descriptorSet;
}

void RenderDevice::ReleaseDescriptorSets(RenderDescriptorSet* descriptorSet)
{
	if (descriptorSet)
	{
		descriptorSet->Relase();
	}
}

static void UpdateDescriptorSetBufferInfo(const RenderDescriptorBufferInfo** inBufferInfos, HalDescriptorBufferInfo** outBufferInfos, uint32_t count)
{
	for (size_t i = 0; i < count; ++i)
	{
		outBufferInfos[i]->_buffer = inBufferInfos[i]->_buffer->GetHalHandle();
		outBufferInfos[i]->_offset = inBufferInfos[i]->_offset;
		outBufferInfos[i]->_range = inBufferInfos[i]->_range;
	}
}

static void UpdateDescriptorSetImageInfo(const RenderDescriptorImageInfo** inImageInfos, HalDescriptorImageInfo** outImageInfos, uint32_t count)
{
    for (size_t i = 0; i < count; ++i)
    {
        outImageInfos[i]->_imageSampler = inImageInfos[i]->_imageSampler->GetHalHandle();
        outImageInfos[i]->_imageView = inImageInfos[i]->_imageView->GetHalHandle();
        outImageInfos[i]->_imageLayout = inImageInfos[i]->_imageLayout;
    }
}

void RenderDevice::UpdateDescriptorSets(caveVector<RenderWriteDescriptorSet>& descriptorWrites)
{
	//Convert descriptor writes
	caveVector<HalWriteDescriptorSet> halDescriptorWrites(GetEngineAllocator());
	for (size_t i = 0; i < descriptorWrites.Size(); ++i)
	{
		// Set general values
		HalWriteDescriptorSet halDescriptorWriteSet;
		halDescriptorWriteSet._dstSet = descriptorWrites[i]._dstSet->GetHalHandle();
		halDescriptorWriteSet._dstBinding = descriptorWrites[i]._dstBinding;
		halDescriptorWriteSet._descriptorType = descriptorWrites[i]._descriptorType;
		halDescriptorWriteSet._dstArrayElement = descriptorWrites[i]._dstArrayElement;
		halDescriptorWriteSet._descriptorCount = descriptorWrites[i]._descriptorCount;

		// update depending on the type
		if (descriptorWrites[i]._pBufferInfo)
		{
			size_t descriptorInfoSize = descriptorWrites[i]._descriptorCount * sizeof(HalDescriptorBufferInfo);
			HalDescriptorBufferInfo *descriptorBufferInfos = static_cast<HalDescriptorBufferInfo*>(GetEngineAllocator()->Allocate(descriptorInfoSize, 4));
			if (descriptorBufferInfos)
				UpdateDescriptorSetBufferInfo(&descriptorWrites[i]._pBufferInfo, &descriptorBufferInfos, descriptorWrites[i]._descriptorCount);

			halDescriptorWriteSet._pBufferInfo = descriptorBufferInfos;
		}
        if (descriptorWrites[i]._pImageInfo)
        {
            size_t descriptorInfoSize = descriptorWrites[i]._descriptorCount * sizeof(HalDescriptorImageInfo);
            HalDescriptorImageInfo *descriptorImageInfos = static_cast<HalDescriptorImageInfo*>(GetEngineAllocator()->Allocate(descriptorInfoSize, 4));
            if (descriptorImageInfos)
                UpdateDescriptorSetImageInfo(&descriptorWrites[i]._pImageInfo, &descriptorImageInfos, descriptorWrites[i]._descriptorCount);

            halDescriptorWriteSet._pImageInfo = descriptorImageInfos;
        }

		halDescriptorWrites.Push(halDescriptorWriteSet);
	}

	// update to hardware
	_pHalRenderDevice->UpdateDescriptorSets(halDescriptorWrites);

	// Relase memory
	for (size_t i = 0; i < halDescriptorWrites.Size(); ++i)
	{
		if (halDescriptorWrites[i]._pBufferInfo)
		{
			GetEngineAllocator()->Deallocate((void *)halDescriptorWrites[i]._pBufferInfo);
		}
        if (halDescriptorWrites[i]._pImageInfo)
        {
            GetEngineAllocator()->Deallocate((void *)halDescriptorWrites[i]._pImageInfo);
        }
	}
}

RenderVertexInput* RenderDevice::CreateVertexInput(HalVertexInputStateInfo& vertexInputState)
{
	RenderVertexInput* vertexInput = AllocateObject<RenderVertexInput>(*_pRenderInstance->GetEngineAllocator(), *this, vertexInputState);
	if (vertexInput)
		vertexInput->AddRef();

	return vertexInput;
}

void RenderDevice::ReleaseVertexInput(RenderVertexInput* vertexInput)
{
	if (vertexInput)
	{
		vertexInput->Relase();
	}
}

RenderInputAssembly* RenderDevice::CreateInputAssembly(HalInputAssemblyInfo& inputAssemblyState)
{
	RenderInputAssembly* inputAssembly = AllocateObject<RenderInputAssembly>(*_pRenderInstance->GetEngineAllocator(), *this, inputAssemblyState);
	if (inputAssembly)
		inputAssembly->AddRef();

	return inputAssembly;
}

void RenderDevice::ReleaseInputAssembly(RenderInputAssembly* inputAssembly)
{
	if (inputAssembly)
	{
		inputAssembly->Relase();
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
		rasterizerState->AddRef();

	return rasterizerState;
}

void RenderDevice::ReleaseRasterizerState(RenderRasterizerState* rasterizerState)
{
	if (rasterizerState)
	{
		rasterizerState->Relase();
	}
}

RenderMultisample* RenderDevice::CreateMultisampleState(HalMultisampleState& multisampleInfo)
{
	RenderMultisample* multisampleState = AllocateObject<RenderMultisample>(*_pRenderInstance->GetEngineAllocator(), *this, multisampleInfo);
	if (multisampleState)
		multisampleState->AddRef();

	return multisampleState;
}

void RenderDevice::ReleaseMultisampleState(RenderMultisample* multisampleState)
{
	if (multisampleState)
	{
		multisampleState->Relase();
	}
}

RenderDepthStencil* RenderDevice::CreateDepthStencilState(HalDepthStencilSetup& depthStencilInfo)
{
	RenderDepthStencil* depthStencilState = AllocateObject<RenderDepthStencil>(*_pRenderInstance->GetEngineAllocator(), *this, depthStencilInfo);
	if (depthStencilState)
		depthStencilState->AddRef();

	return depthStencilState;
}

void RenderDevice::ReleaseDepthStencilState(RenderDepthStencil* depthStencilState)
{
	if (depthStencilState)
	{
		depthStencilState->Relase();
	}
}

RenderColorBlend* RenderDevice::CreateColorBlendState(HalColorBlendState& colorBlendInfo
	, caveVector<HalColorBlendAttachment>& blendAttachments)
{
	RenderColorBlend* colorBlendState = AllocateObject<RenderColorBlend>(*_pRenderInstance->GetEngineAllocator(), *this, colorBlendInfo, blendAttachments);
	if (colorBlendState)
		colorBlendState->AddRef();

	return colorBlendState;
}

void RenderDevice::ReleaseColorBlendState(RenderColorBlend* colorBlendState)
{
	if (colorBlendState)
	{
		colorBlendState->Relase();
	}
}

RenderDynamicState* RenderDevice::CreateDynamicState(caveVector<HalDynamicStates>& dynamicStates)
{
	RenderDynamicState* dynamicState = AllocateObject<RenderDynamicState>(*_pRenderInstance->GetEngineAllocator(), *this, dynamicStates);
	if (dynamicState)
		dynamicState->AddRef();

	return dynamicState;
}

void RenderDevice::ReleaseDynamicState(RenderDynamicState* dynamicState)
{
	if (dynamicState)
	{
		dynamicState->Relase();
	}
}

RenderPipelineLayout* RenderDevice::CreatePipelineLayout(RenderDescriptorSet* descriptorSet, caveVector<HalPushConstantRange>& pushConstants)
{
	RenderPipelineLayout* pipelineLayout = AllocateObject<RenderPipelineLayout>(*_pRenderInstance->GetEngineAllocator(), *this, descriptorSet, pushConstants);
	if (pipelineLayout)
		pipelineLayout->AddRef();

	return pipelineLayout;
}

void RenderDevice::ReleasePipelineLayout(RenderPipelineLayout* pipelineLayout)
{
	if (pipelineLayout)
	{
		pipelineLayout->Relase();
	}
}

RenderPass* RenderDevice::CreateRenderPass(HalRenderPassInfo& renderPassInfo)
{
	RenderPass* renderPass = AllocateObject<RenderPass>(*_pRenderInstance->GetEngineAllocator(), *this, renderPassInfo);
	if (renderPass)
		renderPass->AddRef();

	return renderPass;
}

void RenderDevice::ReleaseRenderPass(RenderPass* renderPass)
{
	if (renderPass)
	{
		renderPass->Relase();
	}
}

RenderGraphicsPipeline* RenderDevice::CreateGraphicsPipeline(RenderGraphicsPipelineInfo& graphicsPipelineInfo)
{
	RenderGraphicsPipeline* graphicsPipeline = AllocateObject<RenderGraphicsPipeline>(*_pRenderInstance->GetEngineAllocator(), *this, graphicsPipelineInfo);
	if (graphicsPipeline)
		graphicsPipeline->AddRef();

	return graphicsPipeline;
}

void RenderDevice::ReleaseGraphicsPipeline(RenderGraphicsPipeline* graphicsPipeline)
{
	if (graphicsPipeline)
	{
		graphicsPipeline->Relase();
	}
}

RenderFrameBuffer* RenderDevice::CreateFrameBuffer(RenderPass& renderPass,
    uint32_t width, uint32_t height, caveVector<RenderTarget*>& renderAttachments)
{
    RenderFrameBuffer* framebuffer = AllocateObject<RenderFrameBuffer>(*_pRenderInstance->GetEngineAllocator(), *this, 
        renderPass,  width, height, renderAttachments);
    if (framebuffer)
        framebuffer->AddRef();

    return framebuffer;
}

void RenderDevice::ReleaseFrameBuffer(RenderFrameBuffer* framebuffer)
{
    if (framebuffer)
    {
        framebuffer->Relase();
    }
}

RenderVertexBuffer* RenderDevice::CreateVertexBuffer(HalBufferInfo& bufferInfo)
{
	RenderVertexBuffer* vertexBuffer = nullptr;
	try
	{
		vertexBuffer = AllocateObject<RenderVertexBuffer>(*_pRenderInstance->GetEngineAllocator(), *this, bufferInfo);
		if (vertexBuffer)
			vertexBuffer->AddRef();

		return vertexBuffer;
	}
	catch (std::exception& )
	{
		return vertexBuffer;
	}
}

void RenderDevice::ReleaseVertexBuffer(RenderVertexBuffer* vertexBuffer)
{
	if (vertexBuffer)
	{
		vertexBuffer->Relase();
	}
}

RenderIndexBuffer* RenderDevice::CreateIndexBuffer(HalBufferInfo& bufferInfo, HalIndexType indexType)
{
	RenderIndexBuffer* indexBuffer = nullptr;
	try
	{
		indexBuffer = AllocateObject<RenderIndexBuffer>(*_pRenderInstance->GetEngineAllocator(), *this, bufferInfo, indexType);
		if (indexBuffer)
			indexBuffer->AddRef();

		return indexBuffer;
	}
	catch (std::exception&)
	{
		return indexBuffer;
	}
}

void RenderDevice::ReleaseIndexBuffer(RenderIndexBuffer* indexBuffer)
{
	if (indexBuffer)
	{
		indexBuffer->Relase();
	}
}

RenderUniformBuffer* RenderDevice::CreateUniformBuffer(HalBufferInfo& bufferInfo)
{
	RenderUniformBuffer* uniformBuffer = nullptr;
	try
	{
		uniformBuffer = AllocateObject<RenderUniformBuffer>(*_pRenderInstance->GetEngineAllocator(), *this, bufferInfo);
		if (uniformBuffer)
			uniformBuffer->AddRef();

		return uniformBuffer;
	}
	catch (std::exception&)
	{
		return uniformBuffer;
	}
}

void RenderDevice::ReleaseUniformBuffer(RenderUniformBuffer* uniformBuffer)
{
	if (uniformBuffer)
	{
		uniformBuffer->Relase();
	}
}

RenderTexture* RenderDevice::CreateTexture(const char* file)
{
	RenderTexture* texture = nullptr;

	try
	{
		texture = _pResourceManager->GetTexture(file);

		return texture;
	}
	catch (std::exception&)
	{
		return texture;
	}
}

void RenderDevice::ReleaseTexture(RenderTexture* texture)
{
	if (texture)
	{
		_pResourceManager->ReleaseTexture(texture);
	}
}

RenderTextureView* RenderDevice::CreateTextureView(HalImage* image, HalImageViewInfo& imageView)
{
    RenderTextureView* textureView = nullptr;

    try
    {
        textureView = AllocateObject<RenderTextureView>(*_pRenderInstance->GetEngineAllocator(), *this, image, imageView);
        if (textureView)
            textureView->AddRef();

        return textureView;
    }
    catch (std::exception&)
    {
        return textureView;
    }
}

void RenderDevice::ReleaseTextureView(RenderTextureView* textureView)
{
    if (textureView)
    {
        textureView->Relase();
    }
}

RenderTextureSampler* RenderDevice::CreateTextureSampler(HalSamplerCreateInfo& samplerInfo)
{
    RenderTextureSampler* textureSampler = nullptr;

    try
    {
        textureSampler = AllocateObject<RenderTextureSampler>(*_pRenderInstance->GetEngineAllocator(), *this, samplerInfo);
        if (textureSampler)
            textureSampler->AddRef();

        return textureSampler;
    }
    catch (std::exception&)
    {
        return textureSampler;
    }
}

void RenderDevice::ReleaseTextureSampler(RenderTextureSampler* textureSampler)
{
    if (textureSampler)
    {
        textureSampler->Relase();
    }
}

RenderTarget* RenderDevice::CreateRenderTarget(HalImageInfo& imageInfo)
{
    RenderTarget* renderTarget = nullptr;

    try
    {
        renderTarget = AllocateObject<RenderTarget>(*_pRenderInstance->GetEngineAllocator(), *this, imageInfo);
        if (renderTarget)
            renderTarget->AddRef();

        return renderTarget;
    }
    catch (std::exception&)
    {
        return renderTarget;
    }
}

void RenderDevice::ReleaseRenderTarget(RenderTarget* renderTarget)
{
    if (renderTarget)
    {
        renderTarget->Relase();
    }
}

bool RenderDevice::AllocateCommandBuffers(RenderCommandPool* commandPool
		, HalCommandBufferInfo& commandBufferInfo
		, caveVector<RenderCommandBuffer*>& commandBuffers)
{
	if (!commandPool)
		return false;

	// tmp buffer
	caveVector<HalCommandBuffer*> halCommandBuffers(_pRenderInstance->GetEngineAllocator());
	halCommandBuffers.Resize(commandBufferInfo._bufferCount);

	bool success = _pHalRenderDevice->AllocateCommandBuffers(commandPool->GetHalHandle(), commandBufferInfo, halCommandBuffers);
	if (!success)
	{
		_pRenderInstance->GetEngineLog()->Error("Error: Failed to allocate commnad buffers");
		return false;
	}

	for (size_t i = 0; i < commandBuffers.Size(); ++i)
	{
		commandBuffers[i] = AllocateObject<RenderCommandBuffer>(*_pRenderInstance->GetEngineAllocator(), *this, halCommandBuffers[i]);
	}

	return true;
}

void RenderDevice::ReleaseCommandBuffers(caveVector<RenderCommandBuffer*>& commandBuffers)
{
	for (size_t i = 0; i < commandBuffers.Size(); ++i)
	{
		if (commandBuffers[i]) 
			DeallocateDelete(*_pRenderInstance->GetEngineAllocator(), *commandBuffers[i]);
	}
}

void RenderDevice::BeginCommandBuffer(RenderCommandBuffer* commandBuffer, HalCommandBufferBeginInfo& commandBufferBeginInfo)
{
	if (commandBuffer)
		_pHalRenderDevice->BeginCommandBuffer(commandBuffer->GetHalHandle(), commandBufferBeginInfo);
}

void RenderDevice::EndCommandBuffer(RenderCommandBuffer* commandBuffer)
{
	if (commandBuffer)
		_pHalRenderDevice->EndCommandBuffer(commandBuffer->GetHalHandle());
}

void RenderDevice::CmdBeginRenderPass(RenderCommandBuffer* commandBuffer, RenderCmdRenderPassInfo& renderPassBeginInfo, HalSubpassContents subpass)
{
	HalCmdRenderPassInfo halInfo;
	halInfo._renderPass = renderPassBeginInfo._renderPass->GetHalHandle();
    halInfo._framebuffer = (renderPassBeginInfo._framebuffer != nullptr) ? renderPassBeginInfo._framebuffer->GetHalHandle() : nullptr;
	halInfo._swapChainIndex = renderPassBeginInfo._swapChainIndex;
	halInfo._renderRect = renderPassBeginInfo._renderRect;
	halInfo._clearValueCount = renderPassBeginInfo._clearValueCount;
	halInfo._clearValues = renderPassBeginInfo._clearValues;

	if (commandBuffer)
		_pHalRenderDevice->CmdBeginRenderPass(commandBuffer->GetHalHandle(), halInfo, subpass);
}

void RenderDevice::CmdEndRenderPass(RenderCommandBuffer* commandBuffer)
{
	if (commandBuffer)
		_pHalRenderDevice->CmdEndRenderPass(commandBuffer->GetHalHandle());
}

void RenderDevice::CmdTransitionResource(RenderCommandBuffer* commandBuffer,
    HalPipelineStageFlags srcStageMask, HalPipelineStageFlags dstStageMask,
    HalTransitionBarrierDesc& TransitionBarrierDes)
{
    if (commandBuffer)
        _pHalRenderDevice->CmdTransitionResource(commandBuffer->GetHalHandle(), srcStageMask, dstStageMask, TransitionBarrierDes);
}

void RenderDevice::CmdBindGraphicsPipeline(RenderCommandBuffer* commandBuffer, RenderGraphicsPipeline* graphicsPipelineInfo)
{
	if (commandBuffer && graphicsPipelineInfo)
		_pHalRenderDevice->CmdBindGraphicsPipeline(commandBuffer->GetHalHandle(), graphicsPipelineInfo->GetHalHandle());
}

void RenderDevice::CmdBindVertexBuffers(RenderCommandBuffer* commandBuffer, uint32_t firstBinding, uint32_t bindingCount
	, RenderVertexBuffer** vertexBuffers, const uint64_t* offsetArray)
{
	if (commandBuffer)
	{
		// tmp buffer
		caveVector<HalBuffer*> halVertexBuffers(_pRenderInstance->GetEngineAllocator());
		halVertexBuffers.Resize(bindingCount);
		for (uint32_t i = 0; i < bindingCount; i++)
			halVertexBuffers[i] = vertexBuffers[i]->GetHalHandle();

		_pHalRenderDevice->CmdBindVertexBuffers(commandBuffer->GetHalHandle(), firstBinding, bindingCount, halVertexBuffers.Data(), offsetArray);
	}
}

void RenderDevice::CmdBindIndexBuffer(RenderCommandBuffer* commandBuffer
	, RenderIndexBuffer* indexBuffer, const uint64_t offset, HalIndexType indexType)
{
	if (commandBuffer)
		_pHalRenderDevice->CmdBindIndexBuffer(commandBuffer->GetHalHandle(), indexBuffer->GetHalHandle(), offset, indexType);
}

void RenderDevice::CmdBindDescriptorSets(RenderCommandBuffer* commandBuffer, HalPipelineBindPoints pipelineBindPoint
	, RenderPipelineLayout* layout, uint32_t firstSet, uint32_t descriptorSetCount
	, RenderDescriptorSet** descriptorSets, uint32_t dynamicOffsetCount, const uint32_t* dynamicOffsets)
{
	if (commandBuffer)
	{
		// tmp buffer
		caveVector<HalDescriptorSet*> halDescriptorSets(_pRenderInstance->GetEngineAllocator());
		halDescriptorSets.Resize(descriptorSetCount);
		for (uint32_t i = 0; i < descriptorSetCount; i++)
			halDescriptorSets[i] = descriptorSets[i]->GetHalHandle();

		_pHalRenderDevice->CmdBindDescriptorSets(commandBuffer->GetHalHandle(), pipelineBindPoint, layout->GetHalHandle()
			, firstSet, descriptorSetCount, halDescriptorSets.Data(), dynamicOffsetCount, dynamicOffsets);
	}
}

void RenderDevice::CmdPushConstants(RenderCommandBuffer* commandBuffer, RenderPipelineLayout* layout
	, HalShaderStagesFlags shaderStagesFlags, uint32_t offset, uint32_t size, const void* pData)
{
	if (commandBuffer)
		_pHalRenderDevice->CmdPushConstants(commandBuffer->GetHalHandle(), layout->GetHalHandle(), shaderStagesFlags, offset, size, pData);
	
}

void RenderDevice::CmdDraw(RenderCommandBuffer* commandBuffer, uint32_t vertexCount, uint32_t instanceCount
	, uint32_t firstVertex, uint32_t firstInstance)
{
	if (commandBuffer)
		_pHalRenderDevice->CmdDraw(commandBuffer->GetHalHandle(), vertexCount, instanceCount, firstVertex, firstInstance);
}

void RenderDevice::CmdDrawIndexed(RenderCommandBuffer* commandBuffer, uint32_t indexCount, uint32_t instanceCount
	, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance)
{
	if (commandBuffer)
		_pHalRenderDevice->CmdDrawIndexed(commandBuffer->GetHalHandle(), indexCount, instanceCount, firstIndex, vertexOffset, firstInstance);
}

void RenderDevice::CmdCopyImage(RenderCommandBuffer* commandBuffer, HalImage* srcImage, HalImageLayout srcLayout,
    HalImage* dstImage, HalImageLayout dstLayout, uint32_t regionCount, HalImageCopy* regions)
{
    if (commandBuffer && srcImage && dstImage && regions)
    {
        _pHalRenderDevice->CmdCopyImage(commandBuffer->GetHalHandle(), srcImage, srcLayout,
            dstImage, dstLayout, regionCount, regions);
    }
}

void RenderDevice::CmdCopyImage(RenderCommandBuffer* commandBuffer, HalImage* srcImage, HalImageLayout srcLayout, size_t swapChainIndex,
    uint32_t regionCount, HalImageCopy* regions)
{
    if (commandBuffer && srcImage && regions)
    {
        _pHalRenderDevice->CmdCopyImage(commandBuffer->GetHalHandle(), srcImage, srcLayout,
            swapChainIndex, regionCount, regions);
    }
}

bool RenderDevice::CmdSubmitGraphicsQueue(caveVector<RenderCommandBuffer*>& commandBuffers,
    caveVector<RenderSemaphore*>& waitSemaphores,
    caveVector<HalPipelineStageFlags*>& stageMasks,
    caveVector<RenderSemaphore*>& signalSemaphores,
    RenderFence* fence)
{
    // convert to hal
    HalSubmitInfo halSubmitInfo = {};

    caveVector<HalCommandBuffer*> halCommandBuffers(GetEngineAllocator());
    for (size_t i = 0; i < commandBuffers.Size(); i++)
    {
        halCommandBuffers.Push(commandBuffers[i]->GetHalHandle());
    }

    caveVector<HalSemaphore*> halWaitSemaphores(GetEngineAllocator());
    for (size_t i = 0; i < waitSemaphores.Size(); i++)
    {
        halWaitSemaphores.Push(waitSemaphores[i]->GetHalHandle());
    }

    caveVector<HalSemaphore*> halSignalSemaphores(GetEngineAllocator());
    for (size_t i = 0; i < signalSemaphores.Size(); i++)
    {
        halSignalSemaphores.Push(signalSemaphores[i]->GetHalHandle());
    }

    halSubmitInfo._commandBufferCount = halCommandBuffers.Size();
    halSubmitInfo._pCommandBuffers = halCommandBuffers.Data();
    halSubmitInfo._waitSemaphoreCount = halWaitSemaphores.Size();
    halSubmitInfo._pWaitSemaphores = halWaitSemaphores.Data();
    halSubmitInfo._waitStageMask = stageMasks.Data();
    halSubmitInfo._signalSemaphoreCount = halSignalSemaphores.Size();
    halSubmitInfo._pSignalSemaphores = halSignalSemaphores.Data();

    return _pHalRenderDevice->CmdSubmitGraphicsQueue(halSubmitInfo, (fence) ? fence->GetHalHandle() : nullptr);
}

void RenderDevice::FlushCopies()
{
	_pHalRenderDevice->FlushCopies();
}

bool RenderDevice::PresentQueueSubmit(RenderCommandBuffer* commandBuffer)
{
	if (commandBuffer)
		return _pHalRenderDevice->PresentQueueSubmit(commandBuffer->GetHalHandle());

	return false;
}

bool RenderDevice::PresentQueue(uint32_t imageIndex)
{
	return _pHalRenderDevice->PresentQueue(imageIndex);
}

void RenderDevice::ReadPixels(void* data)
{
	if (data)
		_pHalRenderDevice->ReadPixels(data);
}

}