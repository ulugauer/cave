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

/// @file shaderResource.cpp
///        Handles shader assets

#include "shaderResource.h"
#include "engineError.h"

#include "json.hpp"

#include <fstream>
#include <iostream>

using json = nlohmann::json;

namespace cave
{

// our default relative locations for materials
static const char* g_shaderLocation = "Shader/";
static const char* g_shaderLocationSpirv = "Shader/Spirv";


ShaderResource::ShaderResource(ResourceManagerPrivate* rm)
	: _pResourceManagerPrivate(rm)
{

}

ShaderResource::~ShaderResource()
{
}

bool ShaderResource::LoadShaderAsset(ResourceObjectFinder& objectFinder, const char* shaderFile, const char* language, const char* type)
{
	if (!IsLanguageSupported(language) || !IsProgramTypeSupported(type))
		return false;

	// get file and path
	std::string fileString = objectFinder.GetFileName(shaderFile);

	std::string shaderDir = objectFinder.GetDirectory(shaderFile);
	// add local search path
	if (!shaderDir.empty())
		objectFinder._localSearchPath.push_back(shaderDir);

	// add default local serach path
	objectFinder._localSearchPath.push_back(g_shaderLocation);
	objectFinder._localSearchPath.push_back(g_shaderLocationSpirv);

	std::ifstream stream;
	if (!objectFinder.OpenFileBinary(fileString.c_str(), stream))
	{
		return false;
	}

	bool success = ParseShader(stream);

	stream.close();
	return success;
}

bool ShaderResource::ParseShader(std::ifstream& fileStream)
{
	// get content size
	std::streampos begin, end;
	begin = fileStream.tellg();
	fileStream.seekg(0, std::ios::end);
	end = fileStream.tellg();

	std::vector<char> result(static_cast<size_t>(end - begin));
	fileStream.seekg(0, std::ios::beg);
	fileStream.read(&result[0], end - begin);

	return true;
}

bool ShaderResource::IsLanguageSupported(const char* language)
{
	std::string lang(language);
	if (lang.compare("spirv") == 0)
		return true;

	return false;
}

bool ShaderResource::IsProgramTypeSupported(const char* type)
{
	std::string program(type);
	if (program.compare("vertex") == 0)
		return true;
	if (program.compare("fragment") == 0)
		return true;

	return false;
}


}
