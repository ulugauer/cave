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

/// @file renderRenderPass.cpp
///       Render pass interface

#include "renderRenderPass.h"
#include "renderDevice.h"
#include "engineError.h"
#include "halRenderPass.h"

#include <cassert>

namespace cave
{
RenderPass::RenderPass(RenderDevice& renderDevice, HalRenderPassInfo& renderPassInfo)
	: CaveRefCount(renderDevice.GetEngineAllocator())
	, _renderDevice(renderDevice)
{
	// Allocate low level object
    try
    {
        _halRenderPass = renderDevice.GetHalRenderDevice()->CreateRenderPass(renderPassInfo);
    }
    catch (std::exception& e)
    {
        std::string msg(e.what());
        throw EngineError(msg);
    }
}

RenderPass::~RenderPass()
{
	if (_halRenderPass)
		DeallocateDelete(*_renderDevice.GetEngineAllocator(), *_halRenderPass);
}

}
