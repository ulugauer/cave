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
#include "shaderResource.h"
#include "engineError.h"

#include "json.hpp"

#include <fstream>
#include <iostream>

using json = nlohmann::json;

namespace cave
{

// our default relative locations for materials
static const char* g_materialLocation = "Materials/";


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

	bool success = LoadMaterialJson(objectFinder, fileStream);
	fileStream.close();

	return success;
}

bool MaterialResource::LoadMaterialJson(ResourceObjectFinder& objectFinder, std::ifstream& fileStream)
{
	json asset;
	fileStream >> asset;
	std::string materialName("");
	float opacity = 0.0f;
	std::vector<float> diffuse;

	// find material entry
	if (asset.find("material") != asset.end())
	{
		// there is an entry with key "material"
		json::iterator itM = asset.find("material");
		if (itM != asset.end() && itM.value().is_object())
		{
			json material = itM.value();
			if (material.count("name") > 0)
				materialName = material["name"].get<std::string>();

			// read material value object
			json::iterator itV = material.find("values");
			if (itV != material.end() && itV.value().is_object())
			{
				json materialValues = itV.value();
				if (materialValues.count("opacity") > 0)
					opacity = materialValues["opacity"].get<float>();
				if (materialValues.count("diffuse") > 0)
					diffuse = materialValues["diffuse"].get< std::vector<float> >();
			}
		}
	}
	else
		return false;

	// find program entry
	std::string programName("");
	std::string language("");
	std::string vertexShaderName("");
	std::string vertexShaderType("");
	std::string fragmentShaderName("");
	std::string fragmentShaderType("");
	if (asset.find("program") != asset.end())
	{
		// there is an entry with key "program"
		json::iterator itP = asset.find("program");
		if (itP != asset.end() && itP.value().is_object())
		{
			json program = itP.value();
			if (program.count("name") > 0)
				programName = program["name"].get<std::string>();
			if (program.count("language") > 0)
				language = program["language"].get<std::string>();

			// read vertex shader
			json::iterator itVertex = program.find("vertex");
			if (itVertex != program.end() && itVertex.value().is_object())
			{
				json vertexValues = itVertex.value();
				if (vertexValues.count("source") > 0)
					vertexShaderName = vertexValues["source"].get<std::string>();
				if (vertexValues.count("type") > 0)
					vertexShaderType = vertexValues["type"].get<std::string>();
			}
			// read fragment shader
			json::iterator itFragment = program.find("fragment");
			if (itFragment != program.end() && itFragment.value().is_object())
			{
				json fragmentValues = itFragment.value();
				if (fragmentValues.count("source") > 0)
					fragmentShaderName = fragmentValues["source"].get<std::string>();
				if (fragmentValues.count("type") > 0)
					fragmentShaderType = fragmentValues["type"].get<std::string>();
			}
		}

		ShaderResource sr(_pResourceManagerPrivate);
		sr.LoadShaderAsset(objectFinder, fragmentShaderName.c_str(), language.c_str(), vertexShaderType.c_str());
		sr.LoadShaderAsset(objectFinder, vertexShaderName.c_str(), language.c_str(), fragmentShaderType.c_str());
	}

	return true;
}

}
