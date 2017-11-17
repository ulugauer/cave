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

/// @file halShader.cpp
///       Hardware render device abstraction

#include "halShader.h"

#include <cstring>

namespace cave
{

HalShader::HalShader(ShaderType type, ShaderLanguage language)
	: _type(type)
	, _language(language)
	, _entryFunc("main")	// default name
{

}

HalShader::~HalShader()
{

}

ShaderType HalShader::ConvertToShaderType(const char* typeString)
{
	ShaderType type = ShaderType::Unknown;
	if (!typeString)
		return type;

	if (!strncmp(typeString, "vertex", 6))
		type = ShaderType::Vertex;
	if (!strncmp(typeString, "fragment", 8))
		type = ShaderType::Fragment;

	return type;
}

ShaderLanguage HalShader::ConvertToShaderLanguage(const char* languageString)
{
	ShaderLanguage language = ShaderLanguage::Unknown;

	if (!languageString)
		return language;

	if (!strncmp(languageString, "glsl", 4))
		language = ShaderLanguage::Glsl;
	if (!strncmp(languageString, "hlsl", 4))
		language = ShaderLanguage::Hlsl;
	if (!strncmp(languageString, "spirv", 4))
		language = ShaderLanguage::Spirv;

	return language;
}

void HalShader::SetShaderEntryFunc(const char* funcName)
{
	_entryFunc = funcName;
}

}
