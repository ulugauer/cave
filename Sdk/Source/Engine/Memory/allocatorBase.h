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


/** \addtogroup engine 
*  @{
*       
*/

#include "engineError.h"

namespace cave
{

///< see http://www.gamedev.net/page/resources/_/technical/general-programming/c-custom-memory-allocation-r3010 

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

}
