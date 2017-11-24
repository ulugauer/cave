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

/// @file halDepthStencil.h
///       Hardware depth stencil abstraction

#include "engineDefines.h"
#include "halInstance.h"
#include "Memory/allocatorBase.h"

#include <iostream>		// includes exception handling
#include <memory>

/** \addtogroup backend
*  @{
*
*/

namespace cave
{

/**
*  @brief A strongly typed enum class representing stencil operations
*/
enum class StencilOp
{
	Keep = 0,
	Zero,
	Replace,
	IncrementClamp,
	DecrementClamp,
	Invert,
	IncrementWarp,
	DecrementWarp,
};

/**
*  @brief A strongly typed enum class representing depth compare operations
*/
enum class CompareOp
{
	Never = 0,
	Less,
	Equal,
	LessEqual,
	Greater,
	NotEqual,
	GreaterEqual,
	Always
};

/**
* @brief Stencil operation state setup
*/
struct HalStencilOpSetup
{
	StencilOp _failOp;	///< Operation on failing stencil test samples
	StencilOp _passOp;	///< Operation on passing stencil test samples
	StencilOp _depthFailOp;	///< Operation on failing depth test samples
	StencilOp _depthPassOp;	///< Operation on passing depth test samples
	CompareOp _compareOp;	///< Stencil compare operation
	uint32_t _compareMask;	///< Bitmaks of tested stencil values
	uint32_t _writeMask;	///< Bitmask of stencil values written to framebuffer
	uint32_t _reference;	///< Reference value used for comparisons
};

/**
* @brief Depth stencil state setup
*/
struct HalDepthStencilSetup
{
	bool _depthTestEnable;	///< Enable depth testing
	bool _depthWriteEnable;	///< Enable depth writes
	CompareOp _depthCompareOp;	///< Depth compare operation
	bool _depthBoundsTestEnable;	///< Enable depth bounds testing
	bool _stencilTestEnable;	///< Enable stencil testing
	HalStencilOpSetup _front;	///< Front face stencil parameter setup
	HalStencilOpSetup _back;	///< Back face stencil parameter setup
	float _minDepthBounds;	///< Min depth value used in depth bound test
	float _maxDepthBounds;	///< Min depth value used in depth bound test

};

/**
* @brief Describes the depth stencil setup
*/
class HalDepthStencil
{
public:
	/**
	* @brief Constructor
	*
	* @param[in] depthStencilSetup	Depth Stencil setup struct
	*/
	HalDepthStencil(HalDepthStencilSetup& depthStencilSetup);

	/** @brief Destructor */
	virtual ~HalDepthStencil();

private:
	HalDepthStencilSetup _depthStencilState;	///< Depth stencil state info
};

}

/** @}*/

