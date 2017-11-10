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

/// @file allocatorBase.h
///       handle engine allocations


/** @addtogroup engine 
*  @{
*       
*/

#include "engineTypes.h"
#include "engineError.h"

#include <cassert>

namespace cave
{

// see http://www.gamedev.net/page/resources/_/technical/general-programming/c-custom-memory-allocation-r3010 

/**
* Engine internal allocation Handling
*/
class AllocatorBase
{
public:
	/**
	* @brief Constructor
	*
	* @param[in] size	Maximum size allowed for this heap (0 means unlimited)
	* @param[in] start	Heap start adress
	*
	*/
    AllocatorBase(size_t size, void* start)
        : _usedMemory(0)
        , _numAllocations(0)
    {
        _start = start;
        _size = size;
    }

	/** destrucctor */
    virtual ~AllocatorBase()
    {
		assert(_numAllocations == 0 && _usedMemory == 0);
        _start = nullptr;
        _size = 0;
    }

	/**
	* @brief Allocate. Needs to be overwritten
	*
	* @param[in] size	Allocation size
	* @param[in] alignment	Allocation alignment
	*
	* @return Aligned pointer to allocation
	*/
    virtual void* Allocate(size_t size, uint8_t alignment = 4) = 0;

	/**
	* @brief Deallocate. Needs to be overwritten
	*
	* @param[in] p	Pointer to allocated memory
	*
	*/
    virtual void Deallocate(void* p) = 0;

	/**
	* @brief Get start of memory heap
	*
	* @return Pointer to heap start
	*/
    void* GetStart() const
    {
        return _start;
    }

	/**
	* @brief Get heap size
	*
	* @return Max heap size
	*/
    size_t GetSize() const
    {
        return _size;
    }

	/**
	* @brief Get used memory
	*
	* @return Current used heap size
	*/
    size_t GetUsedMemory() const
    {
        return _usedMemory;
    }

	/**
	* @brief Get number of allocations
	*
	* @return Current issued allocations
	*/
    size_t GetNumAllocations() const
    {
        return _numAllocations;
    }

protected:
    size_t        _usedMemory;		///< Current size of used memory
    size_t        _numAllocations;	///< Current allocation count

    void*         _start;	///< Heap start adress
    size_t        _size;	///< Maximum Heap size
};

/**
* @brief AllocateNew like a call to new
*
* @param allocator Used allocator
*
* @return Pointer to adress
*/
template <class T> 
T* AllocateNew(AllocatorBase& allocator)
{
    return new (allocator.Allocate(sizeof(T), __alignof(T))) T;
}

/**
* @brief AllocateObject like a call to new for objects
*
* @param allocator Used allocator
* @param args Variadic object arguments
*
* @return Pointer to adress
*/
template <typename T, typename ...Args>
T * AllocateObject(AllocatorBase& allocator, Args &&... args)
{
	return new (allocator.Allocate(sizeof(T), __alignof(T))) T(std::forward<Args>(args)...);
}

/**
* @brief DeallocateDelete like a call to delete
*
* @param allocator Used allocator
* @param object Pointer to object
*
*/
template<class T> 
void DeallocateDelete(AllocatorBase& allocator, T& object)
{
    object.~T();
    allocator.Deallocate(&object);
} 

/**
* @brief Allocate a array of objects
*
* @param allocator Used allocator
* @param length Array length
*
*/
template<class T> 
T* AllocateArray(AllocatorBase& allocator, size_t length)
{
	assert(length != 0);

	uint8_t headerSize = sizeof(size_t) / sizeof(T);
	uint8_t headerModuloSize = sizeof(size_t) % sizeof(T);

	if (headerModuloSize > 0)
		headerSize += 1;

	//Allocate extra space to store array length in the bytes before the array
	T* p = ((T*)allocator.Allocate(sizeof(T)*(length + headerSize), __alignof(T))) + headerSize;

	*(((size_t*)p) - 1) = length;

	for (size_t i = 0; i < length; i++)
		new (&p[i]) T;

	return p;
}

/**
* @brief Deallocate an array of objects
*
* @param allocator Used allocator
* @param array Pointer to array
*
*/
template<class T> 
void DeallocateArray(AllocatorBase& allocator, T* array)
{
	assert(array != nullptr);

	size_t length = *(((size_t*)array) - 1);

	for (size_t i = 0; i < length; i++)
		array[i].~T();

	//Calculate how much extra memory was allocated to store the length before the array
	uint8_t headerSize = sizeof(size_t) / sizeof(T);
	uint8_t headerModuloSize = sizeof(size_t) % sizeof(T);

	if (headerModuloSize > 0)
		headerSize += 1;

	allocator.Deallocate(array - headerSize);
}

}

/** @}*/
