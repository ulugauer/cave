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

/// @file renderDescriptorPool.cpp
///       Render descriptor pool interface

#include "renderDescriptorPool.h"
#include "renderDevice.h"
#include "engineError.h"
#include "halDescriptorPool.h"

#include <cassert>

namespace cave
{
RenderDescriptorPool::RenderDescriptorPool(RenderDevice& renderDevice, HalDescriptorPoolInfo& descriptorPoolInfo)
	: CaveRefCount(renderDevice.GetEngineAllocator())
	, _renderDevice(renderDevice)
{
	// Allocate low level object
	_halDescriptorPool = renderDevice.GetHalRenderDevice()->CreateDescriptorPool(descriptorPoolInfo);
	assert(_halDescriptorPool);
}

RenderDescriptorPool::~RenderDescriptorPool()
{
	if (_halDescriptorPool)
		DeallocateDelete(*_renderDevice.GetEngineAllocator(), *_halDescriptorPool);
}

}
