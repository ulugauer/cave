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

/// @file halDescriptorSet.h
///       Hardware descriptor set layout abstraction

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
class HalDescriptorPool;
class HalDescriptorSet;
class HalBuffer;

/**
* @brief Structure specifying descriptor buffer info
*/
struct HalDescriptorBufferInfo
{
	HalBuffer* _buffer;	///< The buffer resource.
	uint64_t _offset;	///< The offset in bytes from the start of buffer.
	uint64_t _range;	///< The size in bytes that is used for this descriptor update.
};

/**
* @brief Structure specifying the parameters of a descriptor set write operation
*/
struct HalWriteDescriptorSet
{
	HalDescriptorSet* _dstSet;						///< The destination descriptor set to update.
	uint32_t _dstBinding;							///< The descriptor binding within that set.
	uint32_t _dstArrayElement;						///< The starting element in that array.
	uint32_t _descriptorCount;						///< The number of descriptors to update.
	HalDescriptorType _descriptorType;				///< Specifying the type of each descriptor in _pBufferInfo
	//const HalDescriptorImageInfo* _pImageInfo;
	HalDescriptorBufferInfo* _pBufferInfo;	///< Info points to an array of HalDescriptorBufferInfo structures or is ignored.
	//const HalBufferView* _pTexelBufferView;

	HalWriteDescriptorSet()
		: _dstSet(nullptr)
		, _dstArrayElement(0)
		, _descriptorCount(0)
		, _pBufferInfo(nullptr)
	{}
};

/**
* @brief Describes the descriptor set layout
*/
class HalDescriptorSet
{
public:

	/**
	* @brief Constructor
	*
	* @param[in] renderDevice			Pointer to render device object
	*/
	HalDescriptorSet(HalRenderDevice* renderDevice);

	/** @brief Destructor */
	virtual ~HalDescriptorSet();

	/**
	* @brief Allocate a descriptor set based on the set layouts
	* Note that descriptorSetLayouts must be alread set before calling this
	*
	* @return true on success
	*/
	virtual bool AllocateDescriptorSet(HalDescriptorPool *descriptorPool) = 0;

private:
	HalRenderDevice* _pDevice;	///< Pointer to device object
};

}

/** @}*/

