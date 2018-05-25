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

/// @file renderBuffer.cpp
///       Render data buffer interface

#include "renderBuffer.h"
#include "renderDevice.h"
#include "engineError.h"
#include "halBuffer.h"

#include <cassert>

namespace cave
{
RenderBuffer::RenderBuffer(RenderDevice& renderDevice, HalBufferInfo& bufferInfo)
	: CaveRefCount(renderDevice.GetEngineAllocator())
	, _renderDevice(renderDevice)
	, _halBuffer(nullptr)
	, _size(0)
	, _mapped(false)
{
	if (bufferInfo._size == 0)
		throw EngineError("Invalid buffer size");

	// Allocate low level object
	try
	{
		_halBuffer = renderDevice.GetHalRenderDevice()->CreateBuffer(bufferInfo);
	}
	catch (std::exception& e)
	{
		std::string msg(e.what());
		throw EngineError(msg);
	}

	_size = bufferInfo._size;
}

RenderBuffer::~RenderBuffer()
{
	// Free handle
	if (_halBuffer)
		DeallocateDelete(*_renderDevice.GetEngineAllocator(), *_halBuffer);
}

void RenderBuffer::Bind()
{
	if (!_halBuffer)
		throw EngineError("Invalid buffer");

	// Allocate low level object
	try
	{
		_halBuffer->Bind();
	}
	catch (std::exception& e)
	{
		std::string msg(e.what());
		throw EngineError(msg);
	}
}

void RenderBuffer::Update(uint64_t offset, uint64_t size, const void* pData)
{
	if ((size == 0) || (size > _size) || (offset + size > _size))
		throw EngineError("Error RenderBuffer::Map: Size missmatch");

	_halBuffer->Update(offset, size, pData);
}

void RenderBuffer::Map(uint64_t offset, uint64_t size, void** ppData)
{
	if (_mapped)
		throw EngineError("Error RenderBuffer::Map: Already mapped");

	if ((size == 0) || (size > _size) || (offset + size > _size))
		throw EngineError("Error RenderBuffer::Map: Size missmatch");

	try
	{
		_halBuffer->Map(offset, size, ppData);
	}
	catch (std::exception& e)
	{
		std::string msg(e.what());
		throw EngineError(msg);
	}

	_mapped = true;
}

void RenderBuffer:: Unmap()
{
	if (_mapped)
	{
		_halBuffer->Unmap();
		_mapped = false;
	}
}

}
