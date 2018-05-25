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

/// @file caveRefCount.h
///       ref counting objects interface

/** \addtogroup engine
*  @{
*
*/

#include "engineDefines.h"
#include "Memory/allocatorBase.h"

#include <memory>
#include <mutex>

namespace cave
{

/**
* @brief Interface for ref counting objects
*/

/**
* Interface of renderer materials
*/
class CAVE_INTERFACE CaveRefCount
{
public:
	/** @brief Constructor */
	CaveRefCount(std::shared_ptr<AllocatorBase> pAllocator)
		: _refCount(0)
		, _pAllocator(pAllocator)
	{}

	/** @brief Destructor */
	virtual ~CaveRefCount() {}

	/**
	* @brief Increment usage count of object
	*
	* @return new ref count value
	*/
	int32_t AddRef()
	{
		std::lock_guard<std::mutex> lock(_refCountMutex);
		_refCount++;
		return _refCount;
	}

	/**
	* @brief Decrement usage count and release object at zero
	*
	*/
	void Relase()
	{
		_refCountMutex.lock();
		_refCount--;
		_refCountMutex.unlock();
		if (_refCount == 0 && _pAllocator)
		{ 
			DeallocateDelete(*_pAllocator, *(CaveRefCount *)this);
		}
	}

protected:
	int32_t _refCount;	///< Our reference count
	class CAVE_INTERFACE std::mutex _refCountMutex; ///< mutex object for ref counter
	class CAVE_INTERFACE std::shared_ptr<AllocatorBase> _pAllocator; ///< pointer to base allocator
};

}

/** @}*/
