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

void RenderDevice::CreateSwapChain(FrontendWindowInfo& windowInfo)
{
	if (!_pHalRenderDevice || !_pRenderInstance || !_pHalInstance)
		throw EngineError("Render device not properly setup");

	// first copy data
	SwapChainInfo swapChainInfo;
	swapChainInfo.colorBits = windowInfo.colorBits;
	swapChainInfo.depthBits = windowInfo.depthBits;
	swapChainInfo.fullscreen = windowInfo.fullscreen;
	swapChainInfo.surfaceHeight = windowInfo.windowHeight;
	swapChainInfo.surfaceWidth = windowInfo.windowWidth;
#ifdef _WIN32
	swapChainInfo.hInstance = windowInfo.hInstance;
	swapChainInfo.hWindow = windowInfo.hWindow;
#else
	swapChainInfo.connection = windowInfo.connection;
	swapChainInfo.visualId = windowInfo.visualId;
	swapChainInfo.window = windowInfo.window;
#endif

	try
	{
		_pHalRenderDevice->CreateSwapChain(swapChainInfo);
	}
	catch (std::exception& e)
	{
		std::string msg(e.what());
		throw EngineError(msg);
	}
}

}