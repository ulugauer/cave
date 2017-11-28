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

#include <string>

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

}

/** @}*/

