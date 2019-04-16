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

/// @file renderRenderTarget.cpp
///       Render data buffer interface

#include "renderRenderTarget.h"
#include "renderDevice.h"
#include "engineError.h"
#include "halImage.h"

#include <cassert>

namespace cave
{
RenderTarget::RenderTarget(RenderDevice& renderDevice, HalImageInfo& imageInfo)
    : CaveRefCount(renderDevice.GetEngineAllocator())
    , _renderDevice(renderDevice)
    , _halImage(nullptr)
    , _halImageView(nullptr)
{
    if ((imageInfo._width * imageInfo._height * imageInfo._depth) == 0)
        throw EngineError("Invalid texture size");

    // Allocate low level object
    try
    {
        _halImage = renderDevice.GetHalRenderDevice()->CreateImage(imageInfo);
    }
    catch (std::exception& e)
    {
        std::string msg(e.what());
        throw EngineError(msg);
    }
}

RenderTarget::~RenderTarget()
{
    // Free handle
    if (_halImageView)
        DeallocateDelete(*_renderDevice.GetEngineAllocator(), *_halImageView);
    if (_halImage)
        DeallocateDelete(*_renderDevice.GetEngineAllocator(), *_halImage);
}

void RenderTarget::Bind()
{
    if (!_halImage)
        throw EngineError("Invalid image");

    // Allocate low level object
    try
    {
        _halImage->Bind();

        // we need memory before we can creata the view
        if (_halImageView == nullptr)
        {
            HalImageViewInfo imageView;
            imageView._format = _halImage->GetImageFormat();
            imageView._type = HalImageViewType::Image2D;
            imageView._subresourceRange._aspectMask = (_halImage->IsDepthFormat()) ? static_cast<HalImageAspectFlags>(HalImageAspectFlagBits::Depth) :
                static_cast<HalImageAspectFlags>(HalImageAspectFlagBits::Color);
            imageView._subresourceRange._layerCount = 1;
            imageView._subresourceRange._levelCount = 1;

            _halImageView = _renderDevice.GetHalRenderDevice()->CreateImageView(_halImage, imageView);
        }
    }
    catch (std::exception& e)
    {
        std::string msg(e.what());
        throw EngineError(msg);
    }
}

HalImageFormat RenderTarget::GetImageFormat()
{
    HalImageFormat format = HalImageFormat::Undefined;
    if (_halImage)
        format = _halImage->GetImageFormat();

    return format;
}

}
