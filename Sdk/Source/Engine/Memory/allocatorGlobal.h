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

/// @file allocatorGlobal.h
///       handle engine allocations


/** @addtogroup engine 
*  @{
*       
*/

#include "allocatorBase.h"

namespace cave
{

/**
* Engine internal global allocation Handling
*/
class AllocatorGlobal : public AllocatorBase
{
public:
	/**
	* @brief Constructor
	*
	* @param[in] size	Maximum size allowed for this heap (0 means unlimited)
	*
	*/
    AllocatorGlobal(size_t size);

	/** destrucctor */
    virtual ~AllocatorGlobal();

	/**
	* @brief Allocate. Needs to be overwritten
	*
	* @param[in] size	Allocation size
	* @param[in] alignment	Allocation alignment
	*
	* @return Aligned pointer to allocation
	*/
    void* Allocate(size_t size, uint8_t alignment) override;

	/**
	* @brief Deallocate. Needs to be overwritten
	*
	* @param[in] p	Pointer to allocated memory
	*
	*/
    void Deallocate(void* p) override;

	/**
	* @brief Reset all allocations
	*
	*/
    void Reset();

private:
    AllocatorGlobal(const AllocatorGlobal&);                //no copy constructor
    AllocatorGlobal& operator=(const AllocatorGlobal&);
}; 

}

/** @}*/
