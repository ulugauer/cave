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

/// @file halGraphicsPipeline.h
///       Hardware graphic pipeline abstraction

#include "engineDefines.h"
#include "halInstance.h"
#include "halTypes.h"
#include "Memory/allocatorBase.h"
#include "Common/caveVector.h"

#include <iostream>		// includes exception handling
#include <memory>

/** \addtogroup backend
*  @{
*
*/

namespace cave
{

// forwards
class HalShader;
class HalVertexInput;
class HalInputAssembly;
class HalViewportAndScissor;
class HalRasterizerState;
class HalMultisample;
class HalDepthStencil;
class HalColorBlend;
class HalDynamicState;
class HalPipelineLayout;
class HalRenderPass;
class HalGraphicsPipeline;

/**
* @brief Graphics pipeline create info
*/
struct CAVE_INTERFACE HalGraphicsPipelineInfo
{
	uint32_t _stageCount;						///< Shader stages count
	HalShader** _pStages;						///< Shader array
	HalVertexInput* _vertexInput;				///< Pointer to an instance of HalVertexInput structure
	HalInputAssembly* _inputAssembly;			///< Pointer to an instance of HalVertexInput structure
	HalViewportAndScissor* _viewport;			///< Pointer to an instance of HalVertexInput structure
	HalRasterizerState* _raterizer;				///< Pointer to an instance of HalVertexInput structure
	HalMultisample* _multisample;				///< Pointer to an instance of HalVertexInput structure
	HalDepthStencil* _depthStencil;				///< Pointer to an instance of HalVertexInput structure
	HalColorBlend* _colorBlend;					///< Pointer to an instance of HalVertexInput structure
	HalDynamicState* _dynamicState;				///< Pointer to an instance of HalVertexInput structure
	HalPipelineLayout* _layout;					///< Pointer to an instance of HalVertexInput structure
	HalRenderPass* _renderPass;					///< Pointer to an instance of HalVertexInput structure
	uint32_t _subpass;							///< The index of the subpass in the render pass where this pipeline will be used
	HalGraphicsPipeline* _basePipelineHandle;	///< A pipeline to derive from
	int32_t _basePipelineIndex;					///< An index into the pCreateInfos parameter to use as a pipeline to derive from

	HalGraphicsPipelineInfo()
		: _stageCount(0)
		, _pStages(nullptr)
		, _vertexInput(nullptr)
		, _inputAssembly(nullptr)
		, _viewport(nullptr)
		, _raterizer(nullptr)
		, _multisample(nullptr)
		, _depthStencil(nullptr)
		, _colorBlend(nullptr)
		, _dynamicState(nullptr)
		, _layout(nullptr)
		, _renderPass(nullptr)
		, _subpass(0)
		, _basePipelineHandle(nullptr)
		, _basePipelineIndex(-1)
	{}
};

/**
* @brief Describes the graphics pipeline
*/
class HalGraphicsPipeline
{
public:

	/**
	* @brief Constructor
	*
	* @param[in] renderDevice			Pointer to render device object
	* @param[in] graphicsPipelineInfo	Graphics pipeline setup struct
	*/
	HalGraphicsPipeline(HalRenderDevice* renderDevice, HalGraphicsPipelineInfo& graphicsPipelineInfo);

	/** @brief Destructor */
	virtual ~HalGraphicsPipeline();

	/**
	* @brief Generate low level graphics pipeline object
	*
	* @return true if  successful
	*/
	virtual bool CompilePipeline() = 0;

private:
	HalRenderDevice* _pDevice;	///< Pointer to device object
};

}

/** @}*/

#pragma once

