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

/// @file caveSanityTestUniformBuffer.cpp
///       Draw simple index quad with device index buffer and vertex buffer

#include "caveSanityTestUniformBuffer.h"

#include "engineInstance.h"
#include "engineError.h"
#include "Render/renderCommandPool.h"
#include "Math/vector4.h"
#include "Math/matrix4.h"

#include <cstring>

using namespace cave;

CaveSanityTestUniformBuffer::CaveSanityTestUniformBuffer()
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
	, _uniformBuffer(nullptr)
	, _graphicsPipeline(nullptr)
	, _commandBuffers(nullptr)
{

}

CaveSanityTestUniformBuffer::~CaveSanityTestUniformBuffer()
{

}

bool CaveSanityTestUniformBuffer::IsSupported(RenderDevice*)
{
	return true;
}

bool CaveSanityTestUniformBuffer::Run(RenderDevice *device, RenderCommandPool* commandPool, cave::RenderPass* renderPass, userContextData* pUserData)
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
		//CreateRenderPass(device);
		CreateVertexBuffer(device);
		CreateIndexBuffer(device);
		CreateUniformBuffer(device, pUserData);
		CreateGraphicsPipeline(device, renderPass);
		AllocateCommandBuffers(device, commandPool);
	}
	catch (CaveSanityTestException err)
	{
		std::cerr << err.what();
		return false;
	}

	// start command buffer recording
	HalCommandBufferBeginInfo beginInfo; // always the same here
	beginInfo._flags = static_cast<uint32_t>(HalCommandBufferUsage::SimultaneousUse);
	HalClearValue clearValues = { 0.0f, 0.0f, 0.0f, 1.0f };
	HalRect2D renderArea;
	renderArea._x = 0; renderArea._y = 0;
	renderArea._height = pUserData->winHeight;
	renderArea._width = pUserData->winWidth;
	for (size_t i = 0; i < device->GetSwapChainImageCount(); i++)
	{
		device->BeginCommandBuffer(_commandBuffers[i], beginInfo);
		// render pass begin
		RenderCmdRenderPassInfo renderPassBeginInfo;
		renderPassBeginInfo._renderPass = renderPass;
		renderPassBeginInfo._swapChainIndex = static_cast<int32_t>(i); ///< fetch framebuffer from swap chain
		renderPassBeginInfo._clearValueCount = 1;
		renderPassBeginInfo._clearValues = &clearValues;
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
		device->EndCommandBuffer(_commandBuffers[i]);
	}

	uint32_t nextImage;
	nextImage = device->AcquireNextSwapChainImage((std::numeric_limits<uint64_t>::max)());

	device->PresentQueueSubmit(_commandBuffers[nextImage]);
	device->PresentQueue(nextImage);

	return true;
}

void CaveSanityTestUniformBuffer::Cleanup(RenderDevice *device, userContextData*)
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
	if (_renderPass)
		device->ReleaseRenderPass(_renderPass);
	if (_vertexBuffer)
		device->ReleaseVertexBuffer(_vertexBuffer);
	if (_indexBuffer)
		device->ReleaseIndexBuffer(_indexBuffer);
	if (_uniformBuffer)
		device->ReleaseUniformBuffer(_uniformBuffer);
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

bool CaveSanityTestUniformBuffer::RunPerformance(RenderDevice*, userContextData*)
{
	return true;
}

//-----------------------------------------------------------------------------
// Setup functions
//-----------------------------------------------------------------------------

void CaveSanityTestUniformBuffer::LoadResource(cave::RenderDevice *device)
{
	_material = AllocateObject<RenderMaterial>(*device->GetEngineAllocator(), *device);
	ResourceManager& rm = device->GetResourceManager();
	*_material = rm.LoadMaterialAsset("CaveSanityTestIndexBuffer.asset");

	if (!_material || _material->GetStageCount() == 0)
		throw CaveSanityTestException("CaveSanityTestIndexBuffer: Failed to load material");
}

void CaveSanityTestUniformBuffer::CreateRenderSection(RenderDevice *device, userContextData* pUserData)
{
	// Render section
	RenderLayerSectionInfo sectionInfo;
	sectionInfo.x = sectionInfo.y = 0;
	sectionInfo.width = pUserData->winWidth;
	sectionInfo.height = pUserData->winHeight;
	_layerSection = device->CreateLayerSection(sectionInfo);

	if (!_layerSection)
		throw CaveSanityTestException("CaveSanityTestIndexBuffer: Failed to create layer section");
}

void CaveSanityTestUniformBuffer::CreateVertexSetup(cave::RenderDevice *device)
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
		throw CaveSanityTestException("CaveSanityTestIndexBuffer: Failed to create vertex setup");
}

void CaveSanityTestUniformBuffer::CreateRasterizerState(cave::RenderDevice *device)
{
	// rasterizer state
	HalRasterizerSetup rasterizerInfo;
	rasterizerInfo._frontFace = HalFrontFace::Clockwise;
	_rasterizerState = device->CreateRasterizerState(rasterizerInfo);

	if (!_rasterizerState)
		throw CaveSanityTestException("CaveSanityTestIndexBuffer: Failed to create rasterizer state");
}

void CaveSanityTestUniformBuffer::CreateMultisampleState(cave::RenderDevice *device)
{
	// multisample state
	HalMultisampleState multisampleInfo;
	_multisampleState = device->CreateMultisampleState(multisampleInfo);

	if (!_multisampleState)
		throw CaveSanityTestException("CaveSanityTestIndexBuffer: Failed to create multisample state");
}

void CaveSanityTestUniformBuffer::CreateDepthStencilState(cave::RenderDevice *device)
{
	// depth stencil state
	HalDepthStencilSetup depthStencilInfo;
	_depthStencilState = device->CreateDepthStencilState(depthStencilInfo);

	if (!_depthStencilState)
		throw CaveSanityTestException("CaveSanityTestIndexBuffer: Failed to create depth stencil state");
}

void CaveSanityTestUniformBuffer::CreateColorBlendState(cave::RenderDevice *device)
{
	HalColorBlendState colorBlendInfo;
	HalColorBlendAttachment blendAttachment;
	caveVector<HalColorBlendAttachment> blendAttachmentArray(device->GetEngineAllocator());
	blendAttachmentArray.Push(blendAttachment);
	_colorBlendState = device->CreateColorBlendState(colorBlendInfo, blendAttachmentArray);

	if (!_colorBlendState)
		throw CaveSanityTestException("CaveSanityTestIndexBuffer: Failed to create color blend state");
}

void CaveSanityTestUniformBuffer::CreateDynamicState(cave::RenderDevice *device)
{
	caveVector<HalDynamicStates> dynamicStates(device->GetEngineAllocator());
	dynamicStates.Push(HalDynamicStates::Viewport);
	dynamicStates.Push(HalDynamicStates::Scissor);
	_dynamicState = device->CreateDynamicState(dynamicStates);

	if (!_dynamicState)
		throw CaveSanityTestException("CaveSanityTestIndexBuffer: Failed to create dynamic state");
}

void CaveSanityTestUniformBuffer::CreatePipelineLayout(cave::RenderDevice *device)
{
	caveVector<HalDescriptorSetLayout> descriptorSetLayouts(device->GetEngineAllocator());
	caveVector<HalPushConstantRange> pushConstants(device->GetEngineAllocator());
	_pipelineLayout = device->CreatePipelineLayout(descriptorSetLayouts, pushConstants);

	if (!_pipelineLayout)
		throw CaveSanityTestException("CaveSanityTestIndexBuffer: Failed to create pipeline layout");
}

void CaveSanityTestUniformBuffer::CreateRenderPass(cave::RenderDevice *device)
{
	HalRenderPassAttachment renderAttachment;
	renderAttachment._format = device->GetSwapChainImageFormat();
	renderAttachment._samples = HalSampleCount::SampleCount1;
	renderAttachment._loadOp = HalAttachmentLoadOperation::Clear;
	renderAttachment._storeOp = HalAttachmentStoreOperation::Store;
	renderAttachment._loadStencilOp = HalAttachmentLoadOperation::DontCare;
	renderAttachment._storeStencilOp = HalAttachmentStoreOperation::DontCare;
	renderAttachment._initialLayout = HalImageLayout::Undefined;
	renderAttachment._finalLayout = HalImageLayout::PresentSrcKHR;
	HalAttachmentReference attachRef;
	attachRef._attachment = 0;
	attachRef._layout = HalImageLayout::ColorAttachment;
	HalSubpassDescription subpassDesc;
	subpassDesc._pipelineBindPoint = HalPipelineBindPoints::Graphics;
	subpassDesc._colorAttachmentCount = 1;
	subpassDesc._pColorAttachments = &attachRef;
	HalSubpassDependency dependency;
	dependency._srcSubpass = HAL_SUBPASS_EXTERNAL;	// special subpass
	dependency._dstSubpass = 0;	// our subpass
	dependency._srcStageMask = static_cast<HalPipelineStageFlags>(HalPipelineStageBits::ColorAttachmentOutput);
	dependency._srcAccessMask = HalAccessBits::AccessNone;
	dependency._dstStageMask = static_cast<HalPipelineStageFlags>(HalPipelineStageBits::ColorAttachmentOutput);
	dependency._dstAccessMask = HalAccessBits::ColorAttachmentRead | HalAccessBits::ColorAttachmentWrite;
	dependency._dependencyFlags = static_cast<HalDependencyFlags>(HalDependencyBits::DependencyNone);
	HalRenderPassInfo renderPassInfo;
	renderPassInfo._attachmentCount = 1;
	renderPassInfo._pAttachments = &renderAttachment;
	renderPassInfo._subpassCount = 1;
	renderPassInfo._pSubpasses = &subpassDesc;
	renderPassInfo._dependencyCount = 1;
	renderPassInfo._pDependencies = &dependency;
	_renderPass = device->CreateRenderPass(renderPassInfo);

	if (!_renderPass)
		throw CaveSanityTestException("CaveSanityTestIndexBuffer: Failed to create render pass");
}

void CaveSanityTestUniformBuffer::CreateVertexBuffer(cave::RenderDevice *device)
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
		throw CaveSanityTestException("CaveSanityTestIndexBuffer: Failed to create vertex buffer");

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

void CaveSanityTestUniformBuffer::CreateIndexBuffer(cave::RenderDevice *device)
{
	// Index data
	const std::vector<uint16_t> indices = {
		0, 1, 2, 2, 3, 0
	};

	// allocate index buffer
	HalBufferInfo bufferInfo;
	bufferInfo._size = sizeof(uint16_t) * indices.size();
	bufferInfo._usage = static_cast<HalBufferUsageFlags>(HalBufferUsage::TransferDest)
		| static_cast<HalBufferUsageFlags>(HalBufferUsage::IndexBuffer);
	bufferInfo._shareMode = HalBufferShareMode::Exclusive;
	bufferInfo._properties = static_cast<HalMemoryPropertyFlags>(HalMemoryPropertyBits::DeviceLocal);
	_indexBuffer = device->CreateIndexBuffer(bufferInfo, HalIndexType::UInt16);
	if (!_indexBuffer)
		throw CaveSanityTestException("CaveSanityTestIndexBuffer: Failed to create vertex buffer");

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

void CaveSanityTestUniformBuffer::CreateUniformBuffer(cave::RenderDevice *device, userContextData* pUserData)
{
	struct UniformBufferObject 
	{
		Matrix4f model;
		Matrix4f view;
		Matrix4f proj;
	}XForm;

	XForm.model.SetIdentity();
	XForm.view.SetIdentity();
	XForm.proj = Ortho<float>(0, static_cast<float>(pUserData->winWidth), static_cast<float>(pUserData->winHeight), 0, 1, 10);

	Matrix4f res = XForm.model * XForm.view;

	// allocate uniform buffer
	HalBufferInfo bufferInfo;
	bufferInfo._size = sizeof(UniformBufferObject);
	bufferInfo._usage = static_cast<HalBufferUsageFlags>(HalBufferUsage::TransferDest)
		| static_cast<HalBufferUsageFlags>(HalBufferUsage::UniformBuffer);
	bufferInfo._shareMode = HalBufferShareMode::Exclusive;
	bufferInfo._properties = static_cast<HalMemoryPropertyFlags>(HalMemoryPropertyBits::DeviceLocal);
	_uniformBuffer = device->CreateUniformBuffer(bufferInfo);
	if (!_uniformBuffer)
		throw CaveSanityTestException("CaveSanityTestIndexBuffer: Failed to create vertex buffer");

	// fill with data
	try
	{
		// back it with memory
		_uniformBuffer->Bind();
		_uniformBuffer->Update(0, bufferInfo._size, &XForm);
	}
	catch (cave::EngineError err)
	{
		throw CaveSanityTestException(err.what());
		return;
	}
}

void CaveSanityTestUniformBuffer::CreateGraphicsPipeline(cave::RenderDevice *device, cave::RenderPass* renderPass)
{
	RenderGraphicsPipelineInfo grpahicsPipelineInfo;
	grpahicsPipelineInfo._material = _material;
	grpahicsPipelineInfo._vertexInput = _vertexInput;
	grpahicsPipelineInfo._inputAssembly = _inputAssembly;
	grpahicsPipelineInfo._viewport = _layerSection->GetViewport();
	grpahicsPipelineInfo._raterizer = _rasterizerState;
	grpahicsPipelineInfo._multisample = _multisampleState;
	grpahicsPipelineInfo._depthStencil = nullptr;
	grpahicsPipelineInfo._colorBlend = _colorBlendState;
	grpahicsPipelineInfo._dynamicState = nullptr;
	grpahicsPipelineInfo._layout = _pipelineLayout;
	grpahicsPipelineInfo._renderPass = renderPass;
	grpahicsPipelineInfo._subpass = 0;
	grpahicsPipelineInfo._basePipelineIndex = -1;
	grpahicsPipelineInfo._basePipelineHandle = nullptr;
	_graphicsPipeline = device->CreateGraphicsPipeline(grpahicsPipelineInfo);

	if (!_graphicsPipeline)
		throw CaveSanityTestException("CaveSanityTestIndexBuffer: Failed to create graphics pipiline");

	_graphicsPipeline->Update();
}

void CaveSanityTestUniformBuffer::AllocateCommandBuffers(cave::RenderDevice *device, cave::RenderCommandPool* commandPool)
{
	// allocate command buffers
	HalCommandBufferInfo allocInfo;
	allocInfo._bufferCount = device->GetSwapChainImageCount();
	allocInfo._level = HalCommandBufferLevel::PrimaryLevel;
	caveVector<RenderCommandBuffer*> commandBuffers(device->GetEngineAllocator());
	commandBuffers.Resize(allocInfo._bufferCount);
	if (!device->AllocateCommandBuffers(commandPool, allocInfo, commandBuffers))
		throw CaveSanityTestException("CaveSanityTestIndexBuffer: Failed to allocate command buffers");

	// copy buffer pointer
	_commandBuffers = AllocateArray<RenderCommandBuffer*>(*device->GetEngineAllocator(), allocInfo._bufferCount);
	if (!_commandBuffers)
		throw CaveSanityTestException("CaveSanityTestIndexBuffer: Failed to allocate command buffers");

	for (uint32_t i = 0; i < allocInfo._bufferCount; ++i)
	{
		_commandBuffers[i] = commandBuffers[i];
	}

}