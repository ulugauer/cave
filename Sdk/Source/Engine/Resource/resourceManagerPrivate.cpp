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

/// @file resourceManagerPrivate.cpp
///       Handle all kind of resources (images, shaders,...)

#include "resourceManagerPrivate.h"
#include "materialResource.h"
#include "engineInstancePrivate.h"
#include "engineError.h"

#include <fstream>

namespace cave
{

// our default relative locations
static const char* g_contentLocation = "/content/";

//-----------------------------------------------------------------------------
// ResourceObjectFinder class
//-----------------------------------------------------------------------------
ResourceObjectFinder::ResourceObjectFinder(ResourceManagerPrivate& rm)
{
	std::string tmp = rm.GetProjectPath();
	if (!tmp.empty())
	{
		std::string projPath(tmp);
		projPath.append(g_contentLocation);
		_projectContentPath = projPath;
	}

	tmp = rm.GetApplicationPath();
	if (!tmp.empty())
	{
		std::string appPath(tmp);
		appPath.append(g_contentLocation);
		_appContentPath = appPath;
	}
}

bool ResourceObjectFinder::OpenFileAscii(const char* file, std::ifstream& inStream)
{
	// first search in project dir if available
	if (!_projectContentPath.empty())
	{
		for (size_t i = 0; i < _localSearchPath.size(); i++)
		{
			std::string projPath(_projectContentPath);
			projPath.append(_localSearchPath[i]);
			projPath.append(file);
			inStream.open(projPath.c_str(), std::ifstream::in);
			if (inStream.is_open())
			{
				return true;
			}
		}
	}

	if (!_appContentPath.empty())
	{
		for (size_t i = 0; i < _localSearchPath.size(); i++)
		{
			std::string appPath(_appContentPath);
			appPath.append(_localSearchPath[i]);
			appPath.append(file);
			inStream.open(appPath.c_str(), std::ifstream::in);
			if (inStream.is_open())
			{
				return true;
			}
		}
	}

	std::string errorPath(_projectContentPath);
	errorPath.append(file);
	std::cerr << "FILE: " << errorPath << " " << errorPath.length() << std::endl;
	std::cerr << "FILE Open Error: " << strerror(errno) << std::endl;

	return false;
}

std::string ResourceObjectFinder::GetFileName(const char* file)
{
	std::string input(file);
	size_t pos = input.find_last_of('/');

	if (pos != std::string::npos)
	{
		return input.substr(pos + 1, input.length());
	}

	return input;
}

std::string ResourceObjectFinder::GetDirectory(const char* file)
{
	std::string input(file);
	size_t pos = input.find_last_of('/');

	if (pos != std::string::npos)
	{
		return input.substr(0, pos + 1);
	}

	return std::string();
}

std::string ResourceObjectFinder::GetFileExt(const char* file)
{
	std::string fileString(file);
	size_t start = fileString.find_last_of(".");
	if (start != std::string::npos)
	{
		return fileString.substr(start + 1, fileString.length());
	}

	return std::string("");
}

bool ResourceObjectFinder::CaseInsensitiveStringCompare(const std::string& str1, const std::string& str2)
{
	// compare size first
	if (str1.size() != str2.size())
		return false;

	// compare char by char
	std::string::const_iterator s1 = str1.begin();
	std::string::const_iterator s2 = str2.begin();
	for (; s1 != str1.end(); ++s1, ++s2)
	{
		if (tolower(*s1) != tolower(*s2))
			return false;
	}

	return true;
}


//-----------------------------------------------------------------------------
// ResourceManagerPrivate class
//-----------------------------------------------------------------------------
ResourceManagerPrivate::ResourceManagerPrivate(EngineInstancePrivate* engine
	, const char* applicationPath, const char* projectPath)
	: _pEngineInstance(engine)
	, _appPath(applicationPath)
	, _projectPath(projectPath)
{

}

ResourceManagerPrivate::~ResourceManagerPrivate()
{
}

std::shared_ptr<AllocatorGlobal>
ResourceManagerPrivate::GetEngineAllocator()
{
	return _pEngineInstance->GetEngineAllocator();
}

bool ResourceManagerPrivate::LoadMaterialAsset(const char* file)
{
	ResourceObjectFinder objectFinder(*this);
	MaterialResource mr(this);

	return mr.LoadMaterialAsset(objectFinder, file);

}


}
