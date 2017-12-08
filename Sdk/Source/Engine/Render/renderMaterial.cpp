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

/// @file renderMaterial.cpp
///       Render material interface

#include "renderMaterial.h"
#include "renderShader.h"
#include "renderDevice.h"
#include "engineError.h"

namespace cave
{
RenderMaterial::RenderMaterial(RenderDevice& renderDevice)
	: _vertexShader(nullptr)
	, _fragmentShader(nullptr)
	, _renderDevice(renderDevice)
{

}

RenderMaterial::~RenderMaterial()
{

}

RenderMaterial::RenderMaterial(const RenderMaterial& rhs)
	: _renderDevice(rhs._renderDevice)
	, _vertexShader(rhs._vertexShader)
	, _fragmentShader(rhs._fragmentShader)
{
	_materialData = rhs._materialData;
}

RenderMaterial& RenderMaterial::operator=(const RenderMaterial& rhs)
{
	_renderDevice = rhs._renderDevice;

	// copy data
	_materialData = rhs._materialData;

	return *this;
}

void RenderMaterial::Update()
{
	if (_vertexShader)
		_vertexShader->CompileShader();
	if (_fragmentShader)
		_fragmentShader->CompileShader();
}

}
