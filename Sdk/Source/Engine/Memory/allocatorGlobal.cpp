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

/// @file allocatorGlobal.cpp
///       Handle engine allocations

#include "allocatorGlobal.h"

namespace cave
{

AllocatorGlobal::AllocatorGlobal(size_t size)
    : AllocatorBase(size, nullptr)
{

}

AllocatorGlobal::~AllocatorGlobal()
{

}

void* AllocatorGlobal::Allocate(size_t size, uint8_t)
{
	if (size == 0)
		return nullptr;

    void *aligned_address = malloc(size);

    _numAllocations++;

    return (void*)aligned_address;
}

void AllocatorGlobal::Deallocate(void* p)
{
    _numAllocations--;
    free(p);
}

void AllocatorGlobal::Reset()
{
    _numAllocations = 0;
    _usedMemory = 0;
}

}