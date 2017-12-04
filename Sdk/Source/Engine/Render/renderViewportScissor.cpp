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

/// @file renderViewportScissor.cpp
///       Render viewport and scissor interface

#include "renderViewportScissor.h"
#include "renderLayerSection.h"
#include "renderDevice.h"
#include "engineError.h"

#include <cassert>

namespace cave
{
RenderViewportScissor::RenderViewportScissor(RenderDevice& renderDevice, RenderLayerSectionInfo& sectionInfo)
	: _renderDevice(renderDevice)
{
	// Allocate low level object
	HalViewport viewport;
	viewport._offset._x = static_cast<float>(sectionInfo.x); 
	viewport._offset._y = static_cast<float>(sectionInfo.y);
	viewport._extend._x = static_cast<float>(sectionInfo.width);
	viewport._extend._y = static_cast<float>(sectionInfo.height);
	viewport._depthRange._x = 0; viewport._depthRange._y = 1;
	HalScissor scissor;
	scissor._offset._x = sectionInfo.x;
	scissor._offset._y = sectionInfo.y;
	scissor._extend._x = sectionInfo.width;
	scissor._extend._y = sectionInfo.height;
	_halViewportAndScissor = renderDevice.GetHalRenderDevice()->CreateViewportAndScissor(viewport, scissor);
	assert(_halViewportAndScissor);
}

RenderViewportScissor::~RenderViewportScissor()
{
	if (_halViewportAndScissor)
		DeallocateDelete(*_renderDevice.GetEngineAllocator(), *_halViewportAndScissor);
}

}
