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
#pragma once

/// @file caveSanityTestPushConstants.h
///       Draw simple indexed quad transformed by a data stored in push constants

#include "../caveSanityTestBase.h"

/**
* @brief Test simple quad with index data.
*		 The quad is transformed using push constants
*/
class CaveSanityTestPushConstansts : public CaveSanityTestBase
{
public:
	/** constructor */
	CaveSanityTestPushConstansts();
	/** destructor */
	~CaveSanityTestPushConstansts();

	bool IsSupported(cave::RenderDevice *device);
	bool Run(cave::RenderDevice *device, cave::RenderCommandPool* commandPool, userContextData* pUserData);
	void Cleanup(cave::RenderDevice *device, userContextData* pUserData);
	bool RunPerformance(cave::RenderDevice *device, userContextData* pContextData);

private:
	void LoadResource(cave::RenderDevice *device);
	void CreateRenderSection(cave::RenderDevice *device, userContextData* pUserData);
	void CreateVertexSetup(cave::RenderDevice *device);
	void CreateRasterizerState(cave::RenderDevice *device);
	void CreateMultisampleState(cave::RenderDevice *device);
	void CreateDepthStencilState(cave::RenderDevice *device);
	void CreateColorBlendState(cave::RenderDevice *device);
	void CreateDynamicState(cave::RenderDevice *device);
	void CreateDescriptorSetLayout(cave::RenderDevice *device);
	void CreatePipelineLayout(cave::RenderDevice *device);
	void CreateRenderPass(cave::RenderDevice *device);
	void CreateVertexBuffer(cave::RenderDevice *device);
	void CreateIndexBuffer(cave::RenderDevice *device);
	void CreateUniformBuffer(cave::RenderDevice *device);
	void CreateDescriptorPool(cave::RenderDevice *device);
	void CreateGraphicsPipeline(cave::RenderDevice *device, cave::RenderPass* renderPass);
	void AllocateCommandBuffers(cave::RenderDevice *device, cave::RenderCommandPool* commandPool);

private:
	cave::RenderMaterial* _material;
	cave::RenderLayerSection* _layerSection;
	cave::RenderInputAssembly* _inputAssembly;
	cave::RenderVertexInput* _vertexInput;
	cave::RenderRasterizerState* _rasterizerState;
	cave::RenderMultisample* _multisampleState;
	cave::RenderDepthStencil* _depthStencilState;
	cave::RenderColorBlend* _colorBlendState;
	cave::RenderDynamicState* _dynamicState;
	cave::RenderPipelineLayout* _pipelineLayout;
	cave::RenderPass* _renderPass;
	cave::RenderVertexBuffer* _vertexBuffer;
	cave::RenderIndexBuffer* _indexBuffer;
	cave::RenderUniformBuffer* _uniformBuffer;
	cave::RenderDescriptorPool* _descriptorPool;
	cave::RenderDescriptorSet* _descriptorSet;
	cave::RenderGraphicsPipeline* _graphicsPipeline;
	cave::RenderCommandBuffer** _commandBuffers;
};





