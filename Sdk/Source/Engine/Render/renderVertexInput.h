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

/// @file renderVertexInput.h
///       Render vertex inout interface

#include "Common/caveRefCount.h"
#include "Memory/allocatorGlobal.h"
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
class HalVertexInput;

/**
* @brief Interface for vertex input setup
*/
class CAVE_INTERFACE RenderVertexInput : public CaveRefCount
{
public:

	/**
	* @brief Constructor
	*
	* @param[in] renderDevice	Pointer to render device
	* @param[in] vertexInputState	Vertex input setup struct
	*
	*/
	RenderVertexInput(RenderDevice& renderDevice, HalVertexInputStateInfo& vertexInputState);
	/** @brief copy constructor */
	virtual ~RenderVertexInput();

	/**
	* @brief Get low level HAL handle
	*
	* @return HalVertexInput handle
	*/
	HalVertexInput* GetHalHandle() { return _halVertexInput; }

	/**
	* @brief Get start of binding index
	*		 This basically tells you where your vertex buffers should be bound
	*
	* @return first binding point
	*/
	const uint32_t GetBaseBinding() { return _bindingBase; }

	/**
	* @brief Get number of binding descriptors
	*		 This is usually 1 for interleaved buffers
	*
	* @return amount of vertex bindings
	*/
	const uint32_t GetBindingCount() { return _bindingCount; }

private:
	RenderDevice& _renderDevice;	///< Render device object
	HalVertexInput* _halVertexInput;	///< Pointer to low level shader object
	uint32_t _bindingBase;	///< Base binding of first descriptor
	uint32_t _bindingCount;	///< Number of bindings
};

}
/** @}*/

