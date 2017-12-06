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

/// @file halTypes.h
///       Backend types

/** \addtogroup backend
*  @{
*
*/

#include "engineDefines.h"

#include <memory>

namespace cave
{

/**
*  @brief A strongly typed enum class representing image formts
*/
enum class HalImageFormat
{
	Undefined = 0,	///< Undefined format
	R8G8B8A8UNorm = 37,
	R8G8B8A8SNorm = 38,
};

/**
*  @brief A strongly typed enum class representing the sample count
*/
enum class HalSampleCount
{
	SampleCount1 = 1,
	SampleCount2 = 2,
	SampleCount4 = 4,
	SampleCount8 = 8,
	SampleCount16 = 16,
	SampleCount32 = 32,
	SampleCount64 = 64
};
typedef uint32_t HalSampleCountFlags;	///< Combined sample count flags

/**
*  @brief A strongly typed enum class representing attachment load operations
*/
enum class HalLoadOperation
{
	Load = 0,
	Clear = 1,
	DontCare = 2
};

/**
*  @brief A strongly typed enum class representing attachment store operations
*/
enum class HalStoreOperation
{
	Store = 0,
	DontCare = 1
};

/**
*  @brief A strongly typed enum class representing image layout format
*/
enum class HalImageLayout
{
	Undefined = 0,
	General = 1,
	ColorAttachment = 2,
	DepthStencilAttachment = 3,
	DepthStencilReadOnly = 4,
	ShaderReadOnly = 5,
	TransferSrc = 6,
	TransferDst = 7,
	Preinitialized = 8
};

/**
*  @brief A strongly typed enum class representing subpass desc bits
*/
enum class HalSubpassDescriptionBits
{
};

/**
*  @brief A strongly typed enum class representing pipeline bind points
*/
enum class HalPipelineBindPoints
{
	Graphics = 0,
	Compute = 1
};

/**
*  @brief A strongly typed enum class representing pipeline staging points
*/
enum class HalPipelineStageBits 
{
	TopOfPipe = 0x1,
	DrawIndirect = 0x2,
	VertexInput = 0x4,
	VertexShader = 0x8,
	TessellationControlShader = 0x10,
	TessellationEvaluationShader = 0x20,
	GeometryShader = 0x40,
	FragmentShader = 0x80,
	EarlyFragmentTests = 0x100,
	LateFragmentTests = 0x200,
	ColorAttachmentOutput = 0x400,
	ComputeShader = 0x800,
	Transfer = 0x1000,
	BottomOfPipe = 0x2000,
	Host = 0x4000,
};
typedef uint32_t HalPipelineStageFlags;		///< Combined pipeline stage flags

/**
*  @brief A strongly typed enum class representing access types
*/
enum HalAccessBits 
{
	IndirectCommandRead = 0x1,
	IndexRead = 0x2,
	VertexAttributeRead = 0x4,
	UniformRead = 0x8,
	InputAttachmentRead = 0x10,
	ShaderRead = 0x20,
	ShaderWrite = 0x40,
	ColorAttachmentRead = 0x80,
	ColorAttachmentWrite = 0x100,
	DepthStencilAttachmentRead = 0x200,
	DepthStencilAttachmentWrite = 0x400,
	TransferRead = 0x800,
	TransferWrite = 0x1000,
	HostRead = 0x2000,
	HostWrite = 0x4000,
	MemoryRead = 0x8000,
	MemoryWrite = 0x10000,
};
typedef uint32_t HalAccessFlags;		///< Combined access flags

/**
*  @brief A strongly typed enum class representing subpass dependencies
*/
enum HalDependencyBits 
{
	DependencyByRegion = 0x1
};
typedef uint32_t HalDependencyFlags;		///< Combined dependency flags


/**
*  @brief A strongly typed enum class representing polygon cull mode
*/
enum class HalCullMode
{
	NoCull = 0,
	Front = 1,
	Back = 2,
	FrontAndBack = 3
};

/**
*  @brief A strongly typed enum class representing polygons front face
*/
enum class HalFrontFace
{
	CounterClockwise = 0,
	Clockwise = 1
};

/**
*  @brief A strongly typed enum class representing polygon render mode
*/
enum class HalPolygonMode
{
	Fill = 0,
	Line = 1,
	Point = 2
};

/**
*  @brief A strongly typed enum class representing stencil operations
*/
enum class HalStencilOp
{
	Keep = 0,
	Zero = 1,
	Replace = 2,
	IncrementClamp = 3,
	DecrementClamp = 4,
	Invert = 5,
	IncrementWarp = 6,
	DecrementWarp = 7
};

/**
*  @brief A strongly typed enum class representing depth compare operations
*/
enum class HalCompareOp
{
	Never = 0,
	Less = 1,
	Equal = 2,
	LessEqual = 3,
	Greater = 4,
	NotEqual = 5,
	GreaterEqual = 6,
	AlwaysPass = 7
};

/**
* @brief Rasterizer state setup
*/
struct CAVE_INTERFACE HalRasterizerSetup
{
	HalCullMode _cullMode;			///< Polygon culling mode
	HalFrontFace _frontFace;		///< Front face mode
	bool _depthClampEnable;			///< Clamp depth values to depth range
	bool _depthBiasEnable;			///< Add constant value to depth
	float _depthBiasConstantFactor;	///< Constant bias value
	float _depthBiasClamp;			///< Depth bias clamp value
	float _depthBiasSlopeFactor;	///< Depth bias slope
	float _lineWidth;				///< Line thickness
	HalPolygonMode _polygonMode;	///< Polygon render mode
	bool _rasterizerDiscardEnable;	///< Disable rasterizer state

	HalRasterizerSetup()
	{
		// defaults
		_cullMode = HalCullMode::Back;
		_frontFace = HalFrontFace::CounterClockwise;
		_depthClampEnable = false;
		_depthBiasEnable = false;
		_depthBiasConstantFactor = 0;
		_depthBiasClamp = 0;
		_depthBiasSlopeFactor = 0;
		_lineWidth = 1;
		_polygonMode = HalPolygonMode::Fill;
		_rasterizerDiscardEnable = false;
	}
};

/**
* @brief Multisample state 
*/
struct CAVE_INTERFACE HalMultisampleState
{
	bool _alphaToCoverageEnable;			///< Enable temporary coverage value based on the alpha value
	bool _alphaToOneEnable;					///< Enable replacing alpah output to one
	float _minSampleShading;				///< Minimum fraction value
	uint32_t* _pSampleMask;					///< Coverage bitmask
	HalSampleCount _rasterizationSamples;	///< Sample count
	bool _sampleShadingEnable;				///< Enable per sample fragment execution

	HalMultisampleState()
	{
		_alphaToCoverageEnable = false;
		_alphaToOneEnable = false;
		_minSampleShading = 1;
		_pSampleMask = nullptr;
		_rasterizationSamples = HalSampleCount::SampleCount1;
		_sampleShadingEnable = false;
	}
};

/**
* @brief Stencil operation state setup
*/
struct CAVE_INTERFACE HalStencilOpSetup
{
	HalStencilOp _failOp;		///< Operation on failing stencil test samples
	HalStencilOp _passOp;		///< Operation on passing stencil test samples
	HalStencilOp _depthFailOp;	///< Operation on failing depth test samples
	HalStencilOp _depthPassOp;	///< Operation on passing depth test samples
	HalCompareOp _compareOp;	///< Stencil compare operation
	uint32_t _compareMask;		///< Bitmaks of tested stencil values
	uint32_t _writeMask;		///< Bitmask of stencil values written to framebuffer
	uint32_t _reference;		///< Reference value used for comparisons

	HalStencilOpSetup()
	{
		_failOp = HalStencilOp::Keep;
		_passOp = HalStencilOp::Keep;
		_depthFailOp = HalStencilOp::Keep;
		_depthPassOp = HalStencilOp::Keep;
		_compareMask = 0;
		_writeMask = 0;
		_reference = 0;
	}
};

/**
* @brief Depth stencil state setup
*/
struct CAVE_INTERFACE HalDepthStencilSetup
{
	bool _depthTestEnable;			///< Enable depth testing
	bool _depthWriteEnable;			///< Enable depth writes
	HalCompareOp _depthCompareOp;	///< Depth compare operation
	bool _depthBoundsTestEnable;	///< Enable depth bounds testing
	bool _stencilTestEnable;		///< Enable stencil testing
	HalStencilOpSetup _front;		///< Front face stencil parameter setup
	HalStencilOpSetup _back;		///< Back face stencil parameter setup
	float _minDepthBounds;			///< Min depth value used in depth bound test
	float _maxDepthBounds;			///< Min depth value used in depth bound test

	HalDepthStencilSetup()
	{
		_depthTestEnable = false;
		_depthWriteEnable = false;
		_depthCompareOp = HalCompareOp::Never;
		_depthBoundsTestEnable = false;
		_stencilTestEnable = false;
		_minDepthBounds = 0;
		_maxDepthBounds = 1;
	}
};

}

/** @}*/

