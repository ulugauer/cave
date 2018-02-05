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

/// @file renderIndexBuffer.h
///       Index data buffer interface

#include "Common/caveRefCount.h"
#include "Memory/allocatorGlobal.h"
#include "renderBuffer.h"
#include "halTypes.h"

#include <memory>

/** \addtogroup engine
*  @{
*		This module contains all code related to the engine
*/

namespace cave
{


/**
* @brief Represents a index buffer interface
*/
class CAVE_INTERFACE RenderIndexBuffer : public RenderBuffer
{
public:

	/**
	* @brief Constructor
	*
	* @param[in] renderDevice	Pointer to render device
	* @param[in] bufferInfo		HAL buffer create info
	* @param[in] indexType		Index tpye like [uint16 | uint32]
	*
	*/
	RenderIndexBuffer(RenderDevice& renderDevice, HalBufferInfo& bufferInfo, HalIndexType inxdexType);
	/** @brief destructor */
	virtual ~RenderIndexBuffer();

	/**
	* @brief Query index type
	*
	* @return index type used for this index buffer
	*
	*/
	const HalIndexType GetIndexType() const { return _inxdexType; }

	/**
	* @brief Query index count
	*
	* @return index count
	*
	*/
	const uint32_t GetIndexCount() const { return _indexCount; }

private:
	HalIndexType _inxdexType;	///< Component type of the indices
	uint32_t _indexCount;		///< Number of indices
};

}

/** @}*/






