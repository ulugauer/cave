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

/// @file renderVertexInput.cpp
///       Render vertex inout interface

#include "renderVertexInput.h"
#include "renderDevice.h"
#include "engineError.h"
#include "halVertexInput.h"

#include <cassert>

namespace cave
{
RenderVertexInput::RenderVertexInput(RenderDevice& renderDevice, HalVertexInputStateInfo& vertexInputState)
	: _renderDevice(renderDevice)
{
	// Allocate low level object
	_halVertexInput = renderDevice.GetHalRenderDevice()->CreateVertexInput(vertexInputState);
	assert(_halVertexInput);
}

RenderVertexInput::~RenderVertexInput()
{
	if (_halVertexInput)
		DeallocateDelete(*_renderDevice.GetEngineAllocator(), *_halVertexInput);
}

}
