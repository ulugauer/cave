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

/// @file caveSanityTestFrameBuffer.cpp
///       Draw geometry into a offscreen buffer copy final result to swapchain

#include "caveSanityTestFrameBuffer.h"

#include "engineInstance.h"
#include "engineError.h"
#include "Render/renderCommandPool.h"

#include <cstring>

using namespace cave;

CaveSanityTestFrameBuffer::CaveSanityTestFrameBuffer()
    : _material(nullptr)
    , _layerSection(nullptr)
    , _inputAssembly(nullptr)
    , _vertexInput(nullptr)
    , _rasterizerState(nullptr)
    , _multisampleState(nullptr)
    , _depthStencilState(nullptr)
    , _colorBlendState(nullptr)
    , _dynamicState(nullptr)
    , _pipelineLayout(nullptr)
    , _renderPass(nullptr)
    , _vertexBuffer(nullptr)
    , _indexBuffer(nullptr)
    , _graphicsPipeline(nullptr)
    , _colorRenderTarget(nullptr)
    , _depthRenderTarget(nullptr)
    , _framebuffer(nullptr)
    , _commandBuffers(nullptr)
{

}

CaveSanityTestFrameBuffer::~CaveSanityTestFrameBuffer()
{

}

bool CaveSanityTestFrameBuffer::IsSupported(RenderDevice*)
{
    return true;
}

bool CaveSanityTestFrameBuffer::Run(RenderDevice *device, RenderCommandPool* commandPool, userContextData* pUserData)
{
    try
    {
        // setup
        LoadResource(device);
        CreateRenderSection(device, pUserData);
        CreateVertexSetup(device);
        CreateRasterizerState(device);
        CreateMultisampleState(device);
        CreateDepthStencilState(device);
        CreateColorBlendState(device);
        CreateDynamicState(device);
        CreatePipelineLayout(device);
        CreateRenderPass(device);
        CreateVertexBuffer(device);
        CreateIndexBuffer(device);
        CreateGraphicsPipeline(device, _renderPass);
        CreateFrameBuffer(device, pUserData, _renderPass);
        AllocateCommandBuffers(device, commandPool);
    }
    catch (CaveSanityTestException err)
    {
        std::cerr << err.what();
        return false;
    }

    // setup copy struct
    HalImageCopy halImageCopy;
    halImageCopy._srcOffset = HalOffset3D{ 0, 0, 0 };
    halImageCopy._srcLayers._aspectMask = static_cast<HalImageAspectFlags>(HalImageAspectFlagBits::Color);
    halImageCopy._srcLayers._baseArrayLayer = 0;
    halImageCopy._srcLayers._layerCount = 1;
    halImageCopy._srcLayers._mipLevel = 0;
    halImageCopy._dstOffset = HalOffset3D{ 0, 0, 0 };
    halImageCopy._dstLayers = {};   // no need to setup dest. we copy to swapchain
    halImageCopy._extent = HalExtent3D{ pUserData->winWidth, pUserData->winHeight , 1};

    // start command buffer recording
    HalCommandBufferBeginInfo beginInfo; // always the same here
    beginInfo._flags = static_cast<uint32_t>(HalCommandBufferUsage::SimultaneousUse);
    HalClearValue clearValues[2];
    clearValues[0]._color = { 0.0f, 0.0f, 0.0f, 1.0f };
    clearValues[1]._depthStencil = { 1.0f, 0 };
    HalRect2D renderArea;
    renderArea._x = 0; renderArea._y = 0;
    renderArea._height = pUserData->winHeight;
    renderArea._width = pUserData->winWidth;
    for (size_t i = 0; i < device->GetSwapChainImageCount(); i++)
    {
        device->BeginCommandBuffer(_commandBuffers[i], beginInfo);

        // render pass begin
        RenderCmdRenderPassInfo renderPassBeginInfo;
        renderPassBeginInfo._renderPass = _renderPass;
        renderPassBeginInfo._framebuffer = _framebuffer;
        renderPassBeginInfo._clearValueCount = 2;
        renderPassBeginInfo._clearValues = clearValues;
        renderPassBeginInfo._renderRect = renderArea;
        device->CmdBeginRenderPass(_commandBuffers[i], renderPassBeginInfo, HalSubpassContents::Inline);
        device->CmdBindGraphicsPipeline(_commandBuffers[i], _graphicsPipeline);

        // bind vertex buffer
        RenderVertexBuffer* vertexBuffers[] = { _vertexBuffer };
        uint64_t offsets[] = { 0 };
        device->CmdBindVertexBuffers(_commandBuffers[i], _vertexInput->GetBaseBinding(), _vertexInput->GetBindingCount(), vertexBuffers, offsets);
        device->CmdBindIndexBuffer(_commandBuffers[i], _indexBuffer, 0, _indexBuffer->GetIndexType());

        device->CmdDrawIndexed(_commandBuffers[i], _indexBuffer->GetIndexCount(), 1, 0, 0, 0);

        device->CmdEndRenderPass(_commandBuffers[i]);
        // copy to swapchain
        device->CmdCopyImage(_commandBuffers[i], _colorRenderTarget->GetImageHandle(), HalImageLayout::ColorAttachment, i, 1, &halImageCopy);

        device->EndCommandBuffer(_commandBuffers[i]);
    }

    uint32_t nextImage;
    nextImage = device->AcquireNextSwapChainImage((std::numeric_limits<uint64_t>::max)());

    device->PresentQueueSubmit(_commandBuffers[nextImage]);
    device->PresentQueue(nextImage);

    return true;
}

void CaveSanityTestFrameBuffer::Cleanup(RenderDevice *device, userContextData*)
{
    // before we cleanup make sure everything is processed
    device->WaitIdle();

    if (_commandBuffers)
    {
        for (size_t i = 0; i < device->GetSwapChainImageCount(); ++i)
        {
            if (_commandBuffers[i] != nullptr)
            {
                DeallocateDelete(*device->GetEngineAllocator(), *_commandBuffers[i]);
                _commandBuffers[i] = nullptr;
            }
        }

        DeallocateArray<RenderCommandBuffer*>(*device->GetEngineAllocator(), _commandBuffers);
    }
    if (_graphicsPipeline)
        device->ReleaseGraphicsPipeline(_graphicsPipeline);
    if (_colorRenderTarget)
        device->ReleaseRenderTarget(_colorRenderTarget);
    if (_depthRenderTarget)
        device->ReleaseRenderTarget(_depthRenderTarget);
    if (_framebuffer)
        device->ReleaseFrameBuffer(_framebuffer);
    if (_renderPass)
        device->ReleaseRenderPass(_renderPass);
    if (_vertexBuffer)
        device->ReleaseVertexBuffer(_vertexBuffer);
    if (_indexBuffer)
        device->ReleaseIndexBuffer(_indexBuffer);
    if (_pipelineLayout)
        device->ReleasePipelineLayout(_pipelineLayout);
    if (_dynamicState)
        device->ReleaseDynamicState(_dynamicState);
    if (_colorBlendState)
        device->ReleaseColorBlendState(_colorBlendState);
    if (_depthStencilState)
        device->ReleaseDepthStencilState(_depthStencilState);
    if (_multisampleState)
        device->ReleaseMultisampleState(_multisampleState);
    if (_rasterizerState)
        device->ReleaseRasterizerState(_rasterizerState);
    if (_inputAssembly)
        device->ReleaseInputAssembly(_inputAssembly);
    if (_vertexInput)
        device->ReleaseVertexInput(_vertexInput);
    if (_layerSection)
        device->ReleaseLayerSection(_layerSection);
    if (_material)
        DeallocateDelete(*device->GetEngineAllocator(), *_material);
}

bool CaveSanityTestFrameBuffer::RunPerformance(RenderDevice*, userContextData*)
{
    return true;
}

//-----------------------------------------------------------------------------
// Setup functions
//-----------------------------------------------------------------------------

void CaveSanityTestFrameBuffer::LoadResource(cave::RenderDevice *device)
{
    _material = AllocateObject<RenderMaterial>(*device->GetEngineAllocator(), *device);
    ResourceManager& rm = device->GetResourceManager();
    *_material = rm.LoadMaterialAsset("CaveSanityTestFrameBuffer.asset");

    if (!_material || _material->GetStageCount() == 0)
        throw CaveSanityTestException("CaveSanityTestFrameBuffer: Failed to load material");
}

void CaveSanityTestFrameBuffer::CreateRenderSection(RenderDevice *device, userContextData* pUserData)
{
    // Render section
    RenderLayerSectionInfo sectionInfo;
    sectionInfo.x = sectionInfo.y = 0;
    sectionInfo.width = pUserData->winWidth;
    sectionInfo.height = pUserData->winHeight;
    _layerSection = device->CreateLayerSection(sectionInfo);

    if (!_layerSection)
        throw CaveSanityTestException("CaveSanityTestFrameBuffer: Failed to create layer section");
}

void CaveSanityTestFrameBuffer::CreateVertexSetup(cave::RenderDevice *device)
{
    // Vertex setup
    HalVertexInputBindingDescription bindingDesc;
    bindingDesc._binding = 0;
    bindingDesc._stride = sizeof(float) * (2 + 3); // 2 vertex pos, three colors
    bindingDesc._inputRate = HalVertexInputRate::Vertex;

    caveVector<HalVertexInputAttributeDescription> attribDescArray(device->GetEngineAllocator());
    attribDescArray.Resize(2);
    // position
    attribDescArray[0]._binding = bindingDesc._binding;
    attribDescArray[0]._location = 0;
    attribDescArray[0]._offset = 0;
    attribDescArray[0]._format = HalImageFormat::R32G32SFloat;
    // color
    attribDescArray[1]._binding = bindingDesc._binding;
    attribDescArray[1]._location = 1;
    attribDescArray[1]._offset = sizeof(float) * 2;
    attribDescArray[1]._format = HalImageFormat::R32G32B32SFloat;

    HalVertexInputStateInfo vertexInputInfo;
    vertexInputInfo._vertexBindingDescriptionCount = 1;
    vertexInputInfo._pVertexBindingDescriptions = &bindingDesc;
    vertexInputInfo._vertexAttributeDescriptionCount = static_cast<uint32_t>(attribDescArray.Size());
    vertexInputInfo._pVertexAttributeDescriptions = attribDescArray.Data();
    _vertexInput = device->CreateVertexInput(vertexInputInfo);

    HalInputAssemblyInfo inputAssemblyInfo;
    inputAssemblyInfo._primitiveRestartEnable = false;
    inputAssemblyInfo._topology = HalPrimitiveTopology::TriangleList;
    _inputAssembly = device->CreateInputAssembly(inputAssemblyInfo);

    if (!_vertexInput || !_inputAssembly)
        throw CaveSanityTestException("CaveSanityTestFrameBuffer: Failed to create vertex setup");
}

void CaveSanityTestFrameBuffer::CreateRasterizerState(cave::RenderDevice *device)
{
    // rasterizer state
    HalRasterizerSetup rasterizerInfo;
    rasterizerInfo._frontFace = HalFrontFace::Clockwise;
    _rasterizerState = device->CreateRasterizerState(rasterizerInfo);

    if (!_rasterizerState)
        throw CaveSanityTestException("CaveSanityTestFrameBuffer: Failed to create rasterizer state");
}

void CaveSanityTestFrameBuffer::CreateMultisampleState(cave::RenderDevice *device)
{
    // multisample state
    HalMultisampleState multisampleInfo;
    _multisampleState = device->CreateMultisampleState(multisampleInfo);

    if (!_multisampleState)
        throw CaveSanityTestException("CaveSanityTestFrameBuffer: Failed to create multisample state");
}

void CaveSanityTestFrameBuffer::CreateDepthStencilState(cave::RenderDevice *device)
{
    // depth stencil state
    HalDepthStencilSetup depthStencilInfo;
    _depthStencilState = device->CreateDepthStencilState(depthStencilInfo);

    if (!_depthStencilState)
        throw CaveSanityTestException("CaveSanityTestFrameBuffer: Failed to create depth stencil state");
}

void CaveSanityTestFrameBuffer::CreateColorBlendState(cave::RenderDevice *device)
{
    HalColorBlendState colorBlendInfo;
    HalColorBlendAttachment blendAttachment;
    caveVector<HalColorBlendAttachment> blendAttachmentArray(device->GetEngineAllocator());
    blendAttachmentArray.Push(blendAttachment);
    _colorBlendState = device->CreateColorBlendState(colorBlendInfo, blendAttachmentArray);

    if (!_colorBlendState)
        throw CaveSanityTestException("CaveSanityTestFrameBuffer: Failed to create color blend state");
}

void CaveSanityTestFrameBuffer::CreateDynamicState(cave::RenderDevice *device)
{
    caveVector<HalDynamicStates> dynamicStates(device->GetEngineAllocator());
    dynamicStates.Push(HalDynamicStates::Viewport);
    dynamicStates.Push(HalDynamicStates::Scissor);
    _dynamicState = device->CreateDynamicState(dynamicStates);

    if (!_dynamicState)
        throw CaveSanityTestException("CaveSanityTestFrameBuffer: Failed to create dynamic state");
}

void CaveSanityTestFrameBuffer::CreatePipelineLayout(cave::RenderDevice *device)
{
    caveVector<HalDescriptorSetLayout> descriptorSetLayouts(device->GetEngineAllocator());
    caveVector<HalPushConstantRange> pushConstants(device->GetEngineAllocator());
    _pipelineLayout = device->CreatePipelineLayout(nullptr, pushConstants);

    if (!_pipelineLayout)
        throw CaveSanityTestException("CaveSanityTestFrameBuffer: Failed to create pipeline layout");
}

void CaveSanityTestFrameBuffer::CreateRenderPass(cave::RenderDevice *device)
{
    HalRenderPassAttachment renderAttachments[2];
    renderAttachments[0]._format = device->GetSwapChainImageFormat();
    renderAttachments[0]._samples = HalSampleCount::SampleCount1;
    renderAttachments[0]._loadOp = HalAttachmentLoadOperation::Clear;
    renderAttachments[0]._storeOp = HalAttachmentStoreOperation::Store;
    renderAttachments[0]._loadStencilOp = HalAttachmentLoadOperation::DontCare;
    renderAttachments[0]._storeStencilOp = HalAttachmentStoreOperation::DontCare;
    renderAttachments[0]._initialLayout = HalImageLayout::Undefined;
    renderAttachments[0]._finalLayout = HalImageLayout::ColorAttachment;

    HalAttachmentReference colorAttachRef;
    colorAttachRef._attachment = 0;
    colorAttachRef._layout = HalImageLayout::ColorAttachment;

    // depth attachment
    renderAttachments[1]._format = device->GetSwapChainDepthImageFormat();
    renderAttachments[1]._samples = HalSampleCount::SampleCount1;
    renderAttachments[1]._loadOp = HalAttachmentLoadOperation::Clear;
    renderAttachments[1]._storeOp = HalAttachmentStoreOperation::DontCare;
    renderAttachments[1]._loadStencilOp = HalAttachmentLoadOperation::DontCare;
    renderAttachments[1]._storeStencilOp = HalAttachmentStoreOperation::DontCare;
    renderAttachments[1]._initialLayout = HalImageLayout::Undefined;
    renderAttachments[1]._finalLayout = HalImageLayout::DepthStencilAttachment;

    HalAttachmentReference depthAttachRef;
    depthAttachRef._attachment = 1;
    depthAttachRef._layout = HalImageLayout::DepthStencilAttachment;

    HalSubpassDescription subpassDesc;
    subpassDesc._pipelineBindPoint = HalPipelineBindPoints::Graphics;
    subpassDesc._colorAttachmentCount = 1;
    subpassDesc._pColorAttachments = &colorAttachRef;
    subpassDesc._pDepthStencilAttachment = &depthAttachRef;

    HalSubpassDependency dependency;
    dependency._srcSubpass = 0;	// our subpass
    dependency._dstSubpass = 0;	// our subpass
    dependency._srcStageMask = static_cast<HalPipelineStageFlags>(HalPipelineStageBits::ColorAttachmentOutput);
    dependency._srcAccessMask = HalAccessBits::AccessNone;
    dependency._dstStageMask = static_cast<HalPipelineStageFlags>(HalPipelineStageBits::ColorAttachmentOutput);
    dependency._dstAccessMask = HalAccessBits::ColorAttachmentRead | HalAccessBits::ColorAttachmentWrite;
    dependency._dependencyFlags = static_cast<HalDependencyFlags>(HalDependencyBits::DependencyNone);

    HalRenderPassInfo renderPassInfo;
    renderPassInfo._attachmentCount = 2;
    renderPassInfo._pAttachments = renderAttachments;
    renderPassInfo._subpassCount = 1;
    renderPassInfo._pSubpasses = &subpassDesc;
    renderPassInfo._dependencyCount = 1;
    renderPassInfo._pDependencies = &dependency;
    _renderPass = device->CreateRenderPass(renderPassInfo);

    if (!_renderPass)
        throw CaveSanityTestException("CaveSanityTestFrameBuffer: Failed to create render pass");
}

void CaveSanityTestFrameBuffer::CreateVertexBuffer(cave::RenderDevice *device)
{
    // Vertex Data
    const std::vector<float> vertices = {
        -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,	// pos, color
         0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
         0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
        -0.5f,  0.5f, 1.0f, 1.0f, 1.0f
    };

    // allocate vertex buffer
    HalBufferInfo bufferInfo;
    bufferInfo._size = sizeof(float) * vertices.size();
    bufferInfo._usage = static_cast<HalBufferUsageFlags>(HalBufferUsage::TransferDest)
        | static_cast<HalBufferUsageFlags>(HalBufferUsage::VertexBuffer);
    bufferInfo._shareMode = HalBufferShareMode::Exclusive;
    bufferInfo._properties = static_cast<HalMemoryPropertyFlags>(HalMemoryPropertyBits::DeviceLocal);
    _vertexBuffer = device->CreateVertexBuffer(bufferInfo);
    if (!_vertexBuffer)
        throw CaveSanityTestException("CaveSanityTestFrameBuffer: Failed to create vertex buffer");

    // fill with data
    try
    {
        // back it with memory
        _vertexBuffer->Bind();
        _vertexBuffer->Update(0, bufferInfo._size, vertices.data());
    }
    catch (cave::EngineError err)
    {
        throw CaveSanityTestException(err.what());
        return;
    }
}

void CaveSanityTestFrameBuffer::CreateIndexBuffer(cave::RenderDevice *device)
{
    // Index data
    const std::vector<uint16_t> indices = {
        0, 1, 2, 2, 3, 0
    };

    // allocate vertex buffer
    HalBufferInfo bufferInfo;
    bufferInfo._size = sizeof(uint16_t) * indices.size();
    bufferInfo._usage = static_cast<HalBufferUsageFlags>(HalBufferUsage::TransferDest)
        | static_cast<HalBufferUsageFlags>(HalBufferUsage::IndexBuffer);
    bufferInfo._shareMode = HalBufferShareMode::Exclusive;
    bufferInfo._properties = static_cast<HalMemoryPropertyFlags>(HalMemoryPropertyBits::DeviceLocal);
    _indexBuffer = device->CreateIndexBuffer(bufferInfo, HalIndexType::UInt16);
    if (!_indexBuffer)
        throw CaveSanityTestException("CaveSanityTestFrameBuffer: Failed to create vertex buffer");

    // fill with data
    try
    {
        // back it with memory
        _indexBuffer->Bind();
        _indexBuffer->Update(0, bufferInfo._size, indices.data());
    }
    catch (cave::EngineError err)
    {
        throw CaveSanityTestException(err.what());
        return;
    }
}

void CaveSanityTestFrameBuffer::CreateGraphicsPipeline(cave::RenderDevice *device, cave::RenderPass* renderPass)
{
    RenderGraphicsPipelineInfo grpahicsPipelineInfo;
    grpahicsPipelineInfo._material = _material;
    grpahicsPipelineInfo._vertexInput = _vertexInput;
    grpahicsPipelineInfo._inputAssembly = _inputAssembly;
    grpahicsPipelineInfo._viewport = _layerSection->GetViewport();
    grpahicsPipelineInfo._raterizer = _rasterizerState;
    grpahicsPipelineInfo._multisample = _multisampleState;
    grpahicsPipelineInfo._depthStencil = _depthStencilState;
    grpahicsPipelineInfo._colorBlend = _colorBlendState;
    grpahicsPipelineInfo._dynamicState = nullptr;
    grpahicsPipelineInfo._layout = _pipelineLayout;
    grpahicsPipelineInfo._renderPass = renderPass;
    grpahicsPipelineInfo._subpass = 0;
    grpahicsPipelineInfo._basePipelineIndex = -1;
    grpahicsPipelineInfo._basePipelineHandle = nullptr;
    _graphicsPipeline = device->CreateGraphicsPipeline(grpahicsPipelineInfo);

    if (!_graphicsPipeline)
        throw CaveSanityTestException("CaveSanityTestFrameBuffer: Failed to create graphics pipiline");

    _graphicsPipeline->Update();
}

void CaveSanityTestFrameBuffer::CreateFrameBuffer(cave::RenderDevice *device, userContextData* pUserData, cave::RenderPass* renderPass)
{
    // We need a RenderPass object
    assert(renderPass != nullptr);

    // Create color render target
    HalImageInfo colorImageInfo;
    colorImageInfo._width = pUserData->winWidth;
    colorImageInfo._height = pUserData->winHeight;
    colorImageInfo._depth = 1;
    colorImageInfo._slices = 1;
    colorImageInfo._level = 1;
    colorImageInfo._format = device->GetSwapChainImageFormat();
    colorImageInfo._usage = (static_cast<HalImageUsageFlags>(HalImageUsageFlagBits::TransferSrc) |
        static_cast<HalImageUsageFlags>(HalImageUsageFlagBits::ColorAttachment));

    _colorRenderTarget = device->CreateRenderTarget(colorImageInfo);
    if (!_colorRenderTarget)
        throw CaveSanityTestException("CaveSanityTestFrameBuffer: Failed to create color render target");

    // back up with memory
    _colorRenderTarget->Bind();

    // Create depth render target
    HalImageInfo depthImageInfo;
    depthImageInfo._width = pUserData->winWidth;
    depthImageInfo._height = pUserData->winHeight;
    depthImageInfo._depth = 1;
    depthImageInfo._slices = 1;
    depthImageInfo._level = 1;
    depthImageInfo._format = device->GetSwapChainDepthImageFormat();
    depthImageInfo._usage = static_cast<HalImageUsageFlags>(HalImageUsageFlagBits::DepthAttachment);

    _depthRenderTarget = device->CreateRenderTarget(depthImageInfo);
    if (!_depthRenderTarget)
        throw CaveSanityTestException("CaveSanityTestFrameBuffer: Failed to create depth render target");

    // back up with memory
    _depthRenderTarget->Bind();

    // Note our renderpass does the neccessary transition to render target usage

    caveVector<RenderTarget*> renderAttachments(device->GetEngineAllocator());
    renderAttachments.Push(_colorRenderTarget);
    renderAttachments.Push(_depthRenderTarget);
    _framebuffer = device->CreateFrameBuffer(*renderPass, pUserData->winWidth, pUserData->winHeight, renderAttachments);

    if (!_framebuffer)
        throw CaveSanityTestException("CaveSanityTestFrameBuffer: Failed to create framebuffer");
}

void CaveSanityTestFrameBuffer::AllocateCommandBuffers(cave::RenderDevice *device, cave::RenderCommandPool* commandPool)
{
    // allocate command buffers
    HalCommandBufferInfo allocInfo;
    allocInfo._bufferCount = device->GetSwapChainImageCount();
    allocInfo._level = HalCommandBufferLevel::PrimaryLevel;
    caveVector<RenderCommandBuffer*> commandBuffers(device->GetEngineAllocator());
    commandBuffers.Resize(allocInfo._bufferCount);
    if (!device->AllocateCommandBuffers(commandPool, allocInfo, commandBuffers))
        throw CaveSanityTestException("CaveSanityTestFrameBuffer: Failed to allocate command buffers");

    // copy buffer pointer
    _commandBuffers = AllocateArray<RenderCommandBuffer*>(*device->GetEngineAllocator(), allocInfo._bufferCount);
    if (!_commandBuffers)
        throw CaveSanityTestException("CaveSanityTestFrameBuffer: Failed to allocate command buffers");

    for (uint32_t i = 0; i < allocInfo._bufferCount; ++i)
    {
        _commandBuffers[i] = commandBuffers[i];
    }

}