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

/// @file renderFrameBuffer.cpp
///       Render framebuffer interface

#include "renderFrameBuffer.h"
#include "renderDevice.h"
#include "renderRenderPass.h"
#include "renderRenderTarget.h"
#include "engineError.h"
#include "halImage.h"
#include "halImageView.h"

#include <cassert>

namespace cave
{
RenderFrameBuffer::RenderFrameBuffer(RenderDevice& renderDevice, RenderPass& renderPass,
    uint32_t width, uint32_t height, caveVector<RenderTarget*>& renderAttachments)
    : CaveRefCount(renderDevice.GetEngineAllocator())
    , _renderDevice(renderDevice)
    , _halFrameBuffer(nullptr)
{
    try
    {
        // add image views based on attachments
        HalImageViewInfo imageView;
        caveVector<HalImageView*> halImageViews(renderDevice.GetEngineAllocator());
        for (size_t i = 0; i < renderAttachments.Size(); i++)
        {

            halImageViews.Push(renderAttachments[i]->GetImageViewHandle());
        }

        // Allocate low level object
        _halFrameBuffer = renderDevice.GetHalRenderDevice()->CreateFrameBuffer(renderPass.GetHalHandle(), width, height, 1,
            halImageViews);
    }
    catch (std::exception& e)
    {
        std::string msg(e.what());
        throw EngineError(msg);
    }
}

RenderFrameBuffer::~RenderFrameBuffer()
{
    // Free handle
    if (_halFrameBuffer)
        DeallocateDelete(*_renderDevice.GetEngineAllocator(), *_halFrameBuffer);
}

}
