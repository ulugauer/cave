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

/// @file renderBuffer.h
///       Render data buffer interface

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
class HalBuffer;


/**
* @brief Represents a data buffer base interface
*/
class CAVE_INTERFACE RenderBuffer : public CaveRefCount
{
public:

	/**
	* @brief Constructor
	*
	* @param[in] renderDevice		Pointer to render device
	* @param[in] bufferInfo			HAL buffer create info
	*
	*/
	RenderBuffer(RenderDevice& renderDevice, HalBufferInfo& bufferInfo);
	/** @brief destructor */
	virtual ~RenderBuffer();

	/**
	* @brief Backs the buffer with memory
	*
	*/
	virtual void Bind();

	/**
	* @brief Copy data to buffer
	*		 Throws an exception at failure
	*
	* @param[in] offset		Start offset from where to copy data
	* @param[in] size		The size of the memory range to copy
	* @param[in] pData		Pointer to data
	*
	*/
	virtual void Update(uint64_t offset, uint64_t size, const void* pData);

	/**
	* @brief Map buffer to virtual memory address
	*		 Throws an exception at failure
	*
	* @param[in] offset		Start offset from memory start
	* @param[in] size		The size of the memory range to map from offset
	* @param[out] ppData	Points to a pointer in which is returned a host-accessible pointer to the beginning of the mapped range
	*
	*/
	virtual void Map(uint64_t offset, uint64_t size, void** ppData);

	/**
	* @brief Unmap buffer
	*
	*/
	virtual void Unmap();

	/**
	* @brief Get low level HAL handle
	*
	* @return HalBuffer handle
	*/
	HalBuffer* GetHalHandle() { return _halBuffer; }

protected:
	RenderDevice& _renderDevice;	///< Render device object
	HalBuffer* _halBuffer;			///< Pointer to low level data buffer object
	uint64_t _size;					///< Buffer size
	bool _mapped;					///< Buffer is currently mapped
};

}

/** @}*/




