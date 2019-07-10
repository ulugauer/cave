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

/// @file vulkanRenderDevice.cpp
///       Vulkan render device

#include "d3dInstance.h"
#include "dx12RenderDevice.h"

namespace cave
{

Dx12RenderDevice::Dx12RenderDevice(D3dInstance* instance)
    : HalRenderDevice(instance)
    , _d3dInstance(instance)
{

}

Dx12RenderDevice::~Dx12RenderDevice()
{

}

void Dx12RenderDevice::GetApiVersion(uint32_t& major, uint32_t& minor, uint32_t& patch)
{

}

void Dx12RenderDevice::GetDriverVersion(uint32_t& major, uint32_t& minor, uint32_t& patch)
{

}

const char* Dx12RenderDevice::GetDeviceName()
{
    return nullptr;
}

void Dx12RenderDevice::WaitIdle()
{

}

void Dx12RenderDevice::CreateSwapChain(SwapChainInfo& swapChainInfo)
{

}

HalCommandPool* Dx12RenderDevice::CreateCommandPool(HalCommandPoolInfo& commandPoolInfo)
{
    return nullptr;
}

HalDescriptorPool* Dx12RenderDevice::CreateDescriptorPool(HalDescriptorPoolInfo& descriptorPoolInfo)
{
    return nullptr;
}

HalShader* Dx12RenderDevice::CreateShader(ShaderType type, ShaderLanguage language)
{
    return nullptr;
}

HalVertexInput* Dx12RenderDevice::CreateVertexInput(HalVertexInputStateInfo& vertexInputState)
{
    return nullptr;
}

HalInputAssembly* Dx12RenderDevice::CreateInputAssembly(HalInputAssemblyInfo& inputAssemblyState)
{
    return nullptr;
}

HalViewportAndScissor* Dx12RenderDevice::CreateViewportAndScissor(HalViewport& viewport, HalScissor& scissor)
{
    return nullptr;
}

HalRasterizerState* Dx12RenderDevice::CreateRasterizerState(HalRasterizerSetup& rasterizerStateInfo)
{
    return nullptr;
}

HalMultisample* Dx12RenderDevice::CreateMultisampleState(HalMultisampleState& multisampleStateInfo)
{
    return nullptr;
}

HalDepthStencil* Dx12RenderDevice::CreateDepthStencilState(HalDepthStencilSetup& depthStencilInfo)
{
    return nullptr;
}

HalColorBlend* Dx12RenderDevice::CreateColorBlendState(HalColorBlendState& colorBlendInfo
    , caveVector<HalColorBlendAttachment>& blendAttachments)
{
    return nullptr;
}

HalDynamicState* Dx12RenderDevice::CreateDynamicState(caveVector<HalDynamicStates>& dynamicStates)
{
    return nullptr;
}

HalPipelineLayout* Dx12RenderDevice::CreatePipelineLayout(HalDescriptorSet* descriptorSet, caveVector<HalPushConstantRange>& pushConstants)
{
    return nullptr;
}

HalDescriptorSet* Dx12RenderDevice::CreateDescriptorSetLayouts(caveVector<HalDescriptorSetLayout>& descriptorSetLayouts)
{
    return nullptr;
}

void Dx12RenderDevice::UpdateDescriptorSets(caveVector<HalWriteDescriptorSet>& descriptorWrites)
{

}

HalRenderPass* Dx12RenderDevice::CreateRenderPass(HalRenderPassInfo& renderPassInfo)
{
    return nullptr;
}

HalGraphicsPipeline* Dx12RenderDevice::CreateGraphicsPipeline(HalGraphicsPipelineInfo& graphicsPipelineInfo)
{
    return nullptr;
}

HalFrameBuffer* Dx12RenderDevice::CreateFrameBuffer(HalRenderPass* renderPass,
    uint32_t width, uint32_t height, uint32_t layers, caveVector<HalImageView*>& renderAttachments)
{
    return nullptr;
}

HalSemaphore* Dx12RenderDevice::CreateSemaphore(HalSemaphoreDesc& semaphoreDesc)
{
    return nullptr;
}

HalFence* Dx12RenderDevice::CreateFence(HalFenceDesc& fenceDesc)
{
    return nullptr;
}

HalBuffer* Dx12RenderDevice::CreateBuffer(HalBufferInfo& bufferInfo)
{
    return nullptr;
}

HalImage* Dx12RenderDevice::CreateImage(HalImageInfo& imageInfo)
{
    return nullptr;
}

HalImageView* Dx12RenderDevice::CreateImageView(HalImage* image, HalImageViewInfo& viewInfo)
{
    return nullptr;
}

HalSampler* Dx12RenderDevice::CreateSampler(HalSamplerCreateInfo& samplerInfo)
{
    return nullptr;
}

bool Dx12RenderDevice::AllocateCommandBuffers(HalCommandPool* commandPool
    , HalCommandBufferInfo& commandBufferInfo
    , caveVector<HalCommandBuffer*>& commandBuffers)
{
    return false;
}

void Dx12RenderDevice::BeginCommandBuffer(HalCommandBuffer* commandBuffer, HalCommandBufferBeginInfo& commandBufferBeginInfo)
{

}

void Dx12RenderDevice::EndCommandBuffer(HalCommandBuffer* commandBuffer)
{

}

void Dx12RenderDevice::CmdBeginRenderPass(HalCommandBuffer* commandBuffer, HalCmdRenderPassInfo& RenderPassBeginInfo, HalSubpassContents subpass)
{

}

void Dx12RenderDevice::CmdEndRenderPass(HalCommandBuffer* commandBuffer)
{

}

void Dx12RenderDevice::CmdTransitionResource(HalCommandBuffer* commandBuffer,
    HalPipelineStageFlags srcStageMask, HalPipelineStageFlags dstStageMask,
    HalTransitionBarrierDesc& TransitionBarrierDes)
{

}

void Dx12RenderDevice::CmdBindGraphicsPipeline(HalCommandBuffer* commandBuffer, HalGraphicsPipeline* graphicsPipelineInfo)
{

}

void Dx12RenderDevice::CmdBindVertexBuffers(HalCommandBuffer* commandBuffer, uint32_t firstBinding, uint32_t bindingCount
    , HalBuffer** vertexBuffers, const uint64_t* offsetArray)
{

}

void Dx12RenderDevice::CmdBindIndexBuffer(HalCommandBuffer* commandBuffer
    , HalBuffer* indexBuffer, const uint64_t offset, HalIndexType indexType)
{

}

void Dx12RenderDevice::CmdBindDescriptorSets(HalCommandBuffer* commandBuffer, HalPipelineBindPoints pipelineBindPoint
    , HalPipelineLayout* layout, uint32_t firstSet, uint32_t descriptorSetCount
    , HalDescriptorSet** descriptorSets, uint32_t dynamicOffsetCount, const uint32_t* dynamicOffsets)
{

}

void Dx12RenderDevice::CmdPushConstants(HalCommandBuffer* commandBuffer, HalPipelineLayout* layout
    , HalShaderStagesFlags shaderStagesFlags, uint32_t offset, uint32_t size, const void* pData)
{

}

void Dx12RenderDevice::CmdDraw(HalCommandBuffer* commandBuffer, uint32_t vertexCount, uint32_t instanceCount
    , uint32_t firstVertex, uint32_t firstInstance)
{

}

void Dx12RenderDevice::CmdDrawIndexed(HalCommandBuffer* commandBuffer, uint32_t indexCount, uint32_t instanceCount
    , uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance)
{

}

void Dx12RenderDevice::CmdCopyImage(HalCommandBuffer* commandBuffer, HalImage* srcImage, HalImageLayout srcLayout
    , HalImage* dstImage, HalImageLayout dstLayout, uint32_t regionCount, HalImageCopy* regions)
{

}

void Dx12RenderDevice::CmdCopyImage(HalCommandBuffer* commandBuffer, HalImage* srcImage, HalImageLayout srcLayout, size_t swapChainIndex,
    uint32_t regionCount, HalImageCopy* regions)
{

}

bool Dx12RenderDevice::CmdSubmitGraphicsQueue(HalSubmitInfo& submitInfo, HalFence* fence)
{
    return false;
}

void Dx12RenderDevice::FlushCopies()
{

}

bool Dx12RenderDevice::PresentQueueSubmit(HalCommandBuffer* commandBuffer)
{
    return false;
}

bool Dx12RenderDevice::PresentQueue(uint32_t imageIndex)
{
    return false;
}

const HalImageFormat Dx12RenderDevice::GetSwapChainImageFormat()
{
    return HalImageFormat::Undefined;
}

const HalImageFormat Dx12RenderDevice::GetSwapChainDepthImageFormat()
{
    return HalImageFormat::Undefined;
}

const uint32_t Dx12RenderDevice::GetSwapChainImageCount()
{
    return 0;
}

const HalExtent2D Dx12RenderDevice::GetSwapChainExtend()
{
    return HalExtent2D();
}

const uint32_t Dx12RenderDevice::AcquireNextSwapChainImage(uint64_t timeout)
{
    return 0;
}

void Dx12RenderDevice::CreateSwapChainFramebuffers()
{

}

void Dx12RenderDevice::ReadPixels(void* data)
{

}

}
