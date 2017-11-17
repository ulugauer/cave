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

/// @file renderShader.cpp
///       Render material interface

#include "renderShader.h"
#include "renderDevice.h"
#include "engineError.h"
#include "engineLog.h"
#include "halShader.h"

#include <cstring>
#include <cassert>

namespace cave
{


RenderShader::RenderShader(RenderDevice& renderDevice, const char* shaderType, const char* shaderLanguage)
	: _renderDevice(renderDevice)
	, _halShader(nullptr)
	, _sourceSize(0)
	, _source(nullptr)
	, _refCount(0)
{
	// convert to enums
	ShaderType type = HalShader::ConvertToShaderType(shaderType);
	ShaderLanguage language = HalShader::ConvertToShaderLanguage(shaderLanguage);
	assert(type != ShaderType::Unknown && language != ShaderLanguage::Unknown);

	// Allocate low level object
	_halShader = renderDevice.GetHalRenderDevice()->CreateShader(type, language);
	assert(_halShader);
}

RenderShader::~RenderShader()
{
	if (_source)
		_renderDevice.GetEngineAllocator()->Deallocate(_source);
	if (_halShader)
		DeallocateDelete(*_renderDevice.GetEngineAllocator(), *_halShader);
}

void RenderShader::IncrementUsageCount()
{
	std::lock_guard<std::mutex> lock(_refCountMutex);
	_refCount++;
}

void RenderShader::DecrementUsageCount()
{
	std::lock_guard<std::mutex> lock(_refCountMutex);
	_refCount--;
}

void RenderShader::SetShaderSource(const std::vector<char>& code)
{
	if (_sourceSize > 0 || _source)
	{
		// we already have the source
		_renderDevice.GetEngineLog()->Error("Warning: Source code alread set");
		return;
	}

	if (code.empty())
	{
		// no code
		_renderDevice.GetEngineLog()->Error("Warning: No source code provided");
		return;
	}

	_source = (char *)_renderDevice.GetEngineAllocator()->Allocate(code.size(), 4);
	if (_source)
	{
		_sourceSize = code.size();
		memcpy(_source, code.data(), _sourceSize);

		CompileShader();
	}
}

void RenderShader::SetShaderEntryFunc(const char* funcName)
{
	if (!_halShader)
		return;

	_halShader->SetShaderEntryFunc(funcName);
}

bool RenderShader::CompileShader()
{
	if (!_halShader || !_source || !_sourceSize)
		return false;

	return _halShader->CompileShader(_source, _sourceSize);
}

}
