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

/// @file renderGraphicsPipeline.h
///       Render graphics pipeline interface

#include "Common/caveRefCount.h"
#include "Memory/allocatorGlobal.h"
#include "Common/caveVector.h"
#include "halTypes.h"

#include <memory>

/** \addtogroup engine
*  @{
*		This module contains all code related to the engine
*/

namespace cave
{

/// forward declaration
class RenderDevice;
class RenderShader;
class RenderMaterial;
class RenderVertexInput;
class RenderInputAssembly;
class RenderViewportScissor;
class RenderRasterizerState;
class RenderMultisample;
class RenderDepthStencil;
class RenderColorBlend;
class RenderDynamicState;
class RenderPipelineLayout;
class RenderPass;
class RenderGraphicsPipeline;
class HalGraphicsPipeline;

/**
* @brief Graphics pipeline create info
*/
struct CAVE_INTERFACE RenderGraphicsPipelineInfo
{
	RenderMaterial* _material;						///< Material object
	RenderVertexInput* _vertexInput;				///< Pointer to an instance of HalVertexInput structure
	RenderInputAssembly* _inputAssembly;			///< Pointer to an instance of HalVertexInput structure
	RenderViewportScissor* _viewport;				///< Pointer to an instance of HalVertexInput structure
	RenderRasterizerState* _raterizer;				///< Pointer to an instance of HalVertexInput structure
	RenderMultisample* _multisample;				///< Pointer to an instance of HalVertexInput structure
	RenderDepthStencil* _depthStencil;				///< Pointer to an instance of HalVertexInput structure
	RenderColorBlend* _colorBlend;					///< Pointer to an instance of HalVertexInput structure
	RenderDynamicState* _dynamicState;				///< Pointer to an instance of HalVertexInput structure
	RenderPipelineLayout* _layout;					///< Pointer to an instance of HalVertexInput structure
	RenderPass* _renderPass;						///< Pointer to an instance of HalVertexInput structure
	uint32_t _subpass;								///< The index of the subpass in the render pass where this pipeline will be used
	RenderGraphicsPipeline* _basePipelineHandle;	///< A pipeline to derive from
	int32_t _basePipelineIndex;						///< An index into the pCreateInfos parameter to use as a pipeline to derive from

	RenderGraphicsPipelineInfo()
		: _material(nullptr)
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
	{
	}
};

/**
* @brief Interface for graphics pipeline setup
*/
class CAVE_INTERFACE RenderGraphicsPipeline : public CaveRefCount
{
public:

	/**
	* @brief Constructor
	*
	* @param[in] renderDevice			Pointer to render device
	* @param[in] graphicsPipelineInfo	Graphics pipeline setup struct
	*
	*/
	RenderGraphicsPipeline(RenderDevice& renderDevice, RenderGraphicsPipelineInfo& graphicsPipelineInfo);

	/** @brief destructor */
	virtual ~RenderGraphicsPipeline();

	/**
	* @brief Update pipeline
	*		 If not yet created this creates the low level pipeline object
	*
	* @return none
	*/
	void Update();

private:
	RenderDevice& _renderDevice;	///< Render device object
	HalGraphicsPipeline* _halGraphicsPipeline;	///< Pointer to low level graphics pipeline object
};

}
/** @}*/



