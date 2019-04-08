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

/// @file renderTexture.cpp
///       Render data buffer interface

#include "renderTexture.h"
#include "renderDevice.h"
#include "engineError.h"
#include "halImage.h"

#include <cassert>

namespace cave
{
RenderTexture::RenderTexture(RenderDevice& renderDevice, HalImageInfo& imageInfo, const char* filename)
	: CaveRefCount(renderDevice.GetEngineAllocator())
	, _renderDevice(renderDevice)
	, _halImage(nullptr)
	, _filename(renderDevice.GetEngineAllocator(), filename)
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

RenderTexture::~RenderTexture()
{
	// Free handle
	if (_halImage)
		DeallocateDelete(*_renderDevice.GetEngineAllocator(), *_halImage);
}

void RenderTexture::Bind()
{
	if (!_halImage)
		throw EngineError("Invalid image");

	// Allocate low level object
	try
	{
		_halImage->Bind();
	}
	catch (std::exception& e)
	{
		std::string msg(e.what());
		throw EngineError(msg);
	}
}

void RenderTexture::Update(const void* pData)
{
	if (_halImage)
		_halImage->Update(pData);
}

HalImageFormat RenderTexture::GetImageFormat()
{
    HalImageFormat format = HalImageFormat::Undefined;
    if (_halImage)
        format = _halImage->GetImageFormat();

    return format;
}

uint32_t RenderTexture::GetLevelCount()
{
    uint32_t levels = 0;
    if (_halImage)
        levels = _halImage->GetLevelCount();

    return levels;
}

}
