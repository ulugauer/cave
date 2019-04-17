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

/// @file halBuffer.cpp
///       Hardware data buffer abstraction

#include "halImage.h"
#include "halRenderDevice.h"

namespace cave
{

HalImage::HalImage(HalRenderDevice* renderDevice, HalImageInfo& imageInfo)
	: _pDevice(renderDevice)
	, _imageInfo(imageInfo)
{

}

HalImage::~HalImage()
{

}

HalImageFormat HalImage::GetImageFormat()
{
    return _imageInfo._format;
}

uint32_t HalImage::GetLevelCount()
{
    return _imageInfo._level;
}

bool HalImage::IsDepthFormat()
{
    bool isDepth = false;
    if (_imageInfo._format == HalImageFormat::D32SFloat ||
        _imageInfo._format == HalImageFormat::D32SFloatS8Uint ||
        _imageInfo._format == HalImageFormat::D24UnormS8Uint)
    {
        isDepth = true;
    }

    return isDepth;
}

bool HalImage::IsStencilFormat()
{
    bool isStencil = false;
    if (_imageInfo._format == HalImageFormat::D32SFloatS8Uint ||
        _imageInfo._format == HalImageFormat::D24UnormS8Uint)
    {
        isStencil = true;
    }

    return isStencil;
}

}
