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

/// @file materialResource.cpp
///        Handles material assets

#include "materialResource.h"
#include "engineError.h"

#include "json.hpp"

#include <fstream>
#include <iostream>

using json = nlohmann::json;

namespace cave
{

// our default relative locations for materials
static const char* g_materialLocation = "materials/";


MaterialResource::MaterialResource(ResourceManagerPrivate* rm)
	: _pResourceManagerPrivate(rm)
{

}

MaterialResource::~MaterialResource()
{
}

bool MaterialResource::LoadMaterialAsset(ResourceObjectFinder& objectFinder, const char* file)
{
	std::string fileString = objectFinder.GetFileName(file);
	std::string directory = objectFinder.GetDirectory(file);

	// add local search path
	if (!directory.empty())
		objectFinder._localSearchPath.push_back(directory);

	// add default local serach path
	objectFinder._localSearchPath.push_back(g_materialLocation);

	std::ifstream fileStream;
	if (!objectFinder.OpenFileAscii(fileString.c_str(), fileStream))
	{
		return false;
	}

	LoadMaterialJson(fileStream);

	return  false;
}

bool MaterialResource::LoadMaterialJson(std::ifstream& fileStream)
{
	json materialJson;
	fileStream >> materialJson;

	// find an entry
	if (materialJson.find("material") != materialJson.end())
	{
		// there is an entry with key "foo"
		json::iterator it = materialJson.find("material");
		std::cerr << it.value();
	}


	return true;
}

}
