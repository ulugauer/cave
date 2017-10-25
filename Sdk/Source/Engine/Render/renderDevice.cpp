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

/// @file renderDevice.cpp
///       Render instance abstraction

#include "renderDevice.h"
#include "renderInstance.h"
#include "halRenderDevice.h"
#include "engineError.h"

namespace cave
{

RenderDevice::RenderDevice(RenderInstance* renderInstance, HalInstance* halInstance)
	: _pRenderInstance(renderInstance)
	, _pHalInstance(halInstance)
	, _pHalRenderDevice(nullptr)
{
	try
	{
		_pHalRenderDevice = halInstance->CreateRenderDevice(renderInstance->GetEngineAllocator());
	}
	catch (std::exception& e)
	{
		std::string msg(e.what());
		throw EngineError(msg);
	}
}

RenderDevice::~RenderDevice()
{
	if (_pHalRenderDevice)
		DeallocateDelete(*_pRenderInstance->GetEngineAllocator(), *_pHalRenderDevice);
}

}