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

/// @file halBuffer.h
///       Hardware data buffer abstraction

#include "engineDefines.h"
#include "Memory/allocatorBase.h"
#include "halTypes.h"

#include <iostream>		// includes exception handling
#include <memory>

/** \addtogroup backend
*  @{
*
*/

namespace cave
{

///< forwards
class HalRenderDevice;

/**
* @brief Abstraction of device buffers
*/
class HalBuffer
{
public:
	/**
	* @brief Constructor
	*
	* @param[in] renderDevice	Pointer to render device object
	* @param[in] bufferInfo		Buffer create info
	*/
	HalBuffer(HalRenderDevice* renderDevice, HalBufferInfo& bufferInfo);

	/** @brief Destructor */
	virtual ~HalBuffer();

	/**
	* @brief Bind Binds a buffer and allocates memory
	*
	*/
	virtual void Bind() = 0;

	/**
	* @brief Copy data to buffer
	*
	* @param[in] offset		Start offset from where to copy data
	* @param[in] size		The size of the memory range to copy
	* @param[in] pData		Pointer to data
	*
	*/
	virtual void Update(uint64_t offset, uint64_t size, const void* pData) = 0;

	/**
	* @brief Map buffer to virtual memory address
	*
	* @param[in] offset		Start offset from memory start in bytes
	* @param[in] size		The size of the memory range to map from offset
	* @param[out] ppData	Points to a pointer in which is returned a host-accessible pointer to the beginning of the mapped range
	*
	*/
	virtual void Map(uint64_t offset, uint64_t size, void** ppData) = 0;

	/**
	* @brief Unmap buffer
	*
	*/
	virtual void Unmap() = 0;

	/**
	* @brief Query byte size of HalIndexType
	*
	* @param[in] type	HalIndexType type
	*
	* @return size in byte of type
	*/
	const uint32_t GetHalIndexTypeSize(HalIndexType type);

private:
	HalRenderDevice* _pDevice;	///< Pointer to device object
	HalBufferInfo _bufferInfo;	///< Buffer create info
};

}

/** @}*/



