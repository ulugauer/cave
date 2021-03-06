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

/// @file renderPipelineLayout.cpp
///       Render pipeline layout interface

#include "renderPipelineLayout.h"
#include "renderDescriptorSet.h"
#include "renderDevice.h"
#include "engineError.h"
#include "halPipelineLayout.h"

#include <cassert>

namespace cave
{
RenderPipelineLayout::RenderPipelineLayout(RenderDevice& renderDevice
		, RenderDescriptorSet* descriptorSet
		, caveVector<HalPushConstantRange>& pushConstants)
	: CaveRefCount(renderDevice.GetEngineAllocator())
	, _renderDevice(renderDevice)
{
	HalDescriptorSet* pDescriptorSet = (descriptorSet) ? descriptorSet->GetHalHandle() : nullptr;
	// Allocate low level object
	_halPipelineLayout = renderDevice.GetHalRenderDevice()->CreatePipelineLayout(pDescriptorSet, pushConstants);
	assert(_halPipelineLayout);
}

RenderPipelineLayout::~RenderPipelineLayout()
{
	if (_halPipelineLayout)
		DeallocateDelete(*_renderDevice.GetEngineAllocator(), *_halPipelineLayout);
}

}
