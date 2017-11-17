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
#include "Render/renderMaterial.h"
#include "Render/renderShader.h"
#include "engineError.h"
#include "Math/vector4.h"

#include "json.hpp"

#include <fstream>
#include <iostream>

using json = nlohmann::json;	///< convenience shortcut

namespace cave
{

// our default relative locations for materials and shaders
static const char* g_materialLocation = "Materials/";
static const char* g_shaderLocation = "Shader/";
static const char* g_shaderLocationSpirv = "Shader/Spirv";

MaterialResource::MaterialResource(ResourceManagerPrivate* rm)
	: _pResourceManagerPrivate(rm)
{

}

MaterialResource::~MaterialResource()
{
}

bool MaterialResource::LoadShader(ResourceObjectFinder& objectFinder, std::string& filename, RenderShader* shader)
{
	// get file and path
	std::string fileString = objectFinder.GetFileName(filename.c_str());

	std::string shaderDir = objectFinder.GetDirectory(filename.c_str());
	// add local search path
	if (!shaderDir.empty())
		objectFinder._localSearchPath.push_back(shaderDir);

	std::ifstream fileStream;
	if (!objectFinder.OpenFileBinary(fileString.c_str(), fileStream))
	{
		return false;
	}

	// get content size
	std::streampos begin, end;
	begin = fileStream.tellg();
	fileStream.seekg(0, std::ios::end);
	end = fileStream.tellg();

	std::vector<char> code(static_cast<size_t>(end - begin));
	fileStream.seekg(0, std::ios::beg);
	fileStream.read(&code[0], end - begin);
	fileStream.close();

	shader->SetShaderSource(code);

	return true;
}

RenderMaterial* MaterialResource::LoadMaterialAsset(ResourceObjectFinder& objectFinder, const char* file)
{
	std::string fileString = objectFinder.GetFileName(file);
	std::string directory = objectFinder.GetDirectory(file);

	// add local search path
	if (!directory.empty())
		objectFinder._localSearchPath.push_back(directory);

	// add default local serach path
	objectFinder._localSearchPath.push_back(g_materialLocation);
	objectFinder._localSearchPath.push_back(g_shaderLocation);
	objectFinder._localSearchPath.push_back(g_shaderLocationSpirv);

	// create a new material
	RenderMaterial* newMaterial = AllocateObject<RenderMaterial>(*_pResourceManagerPrivate->GetEngineAllocator(), *_pResourceManagerPrivate->GetRenderDevice());

	std::ifstream fileStream;
	if (!objectFinder.OpenFileAscii(fileString.c_str(), fileStream))
	{
		return newMaterial;
	}

	if (newMaterial)
	{
		LoadMaterialJson(objectFinder, fileStream, newMaterial);
		fileStream.close();
	}

	return newMaterial;
}

bool MaterialResource::LoadMaterialJson(ResourceObjectFinder& objectFinder, std::ifstream& fileStream, RenderMaterial* material)
{
	json asset;
	fileStream >> asset;
	std::string materialName("");
	float opacity = 0.0f;
	Vector4f ambientColor(0, 0, 0, 1);
	Vector4f diffuseColor(1,1,1,1);
	Vector4f emissiveColor(0, 0, 0, 0);
	std::vector<float> diffuseValue;

	// find material entry
	if (asset.find("material") != asset.end())
	{
		// there is an entry with key "material"
		json::iterator itM = asset.find("material");
		if (itM != asset.end() && itM.value().is_object())
		{
			json materialJson = itM.value();
			if (materialJson.count("name") > 0)
				materialName = materialJson["name"].get<std::string>();

			// read material value object
			json::iterator itV = materialJson.find("values");
			if (itV != materialJson.end() && itV.value().is_object())
			{
				json materialValues = itV.value();
				if (materialValues.count("opacity") > 0)
					opacity = materialValues["opacity"].get<float>();
				if (materialValues.count("diffuse") > 0)
				{
					diffuseValue = materialValues["diffuse"].get< std::vector<float> >();
					if (diffuseValue.size() == 3)
					{
						diffuseColor._x = diffuseValue[0];
						diffuseColor._y = diffuseValue[1];
						diffuseColor._z = diffuseValue[2];
					}
				}
			}
		}
	}

	material->SetAmbientColor(ambientColor);
	material->SetDiffuseColor(diffuseColor);
	material->SetEmissiveColor(emissiveColor);
	material->SetOpacity(opacity);

	// find program entry
	std::string programName("");
	std::string language("");
	std::string vertexShaderName("");
	std::string vertexShaderEntry("");
	std::string fragmentShaderName("");
	std::string fragmentShaderEntry("");
	if (asset.find("program") != asset.end())
	{
		// there is an entry with key "program"
		json::iterator itP = asset.find("program");
		if (itP != asset.end() && itP.value().is_object())
		{
			json programJson = itP.value();
			if (programJson.count("name") > 0)
				programName = programJson["name"].get<std::string>();
			if (programJson.count("language") > 0)
				language = programJson["language"].get<std::string>();

			// read vertex shader
			json::iterator itVertex = programJson.find("vertex");
			if (itVertex != programJson.end() && itVertex.value().is_object())
			{
				json vertexValues = itVertex.value();
				if (vertexValues.count("source") > 0)
					vertexShaderName = vertexValues["source"].get<std::string>();
				if (vertexValues.count("entry") > 0)
					vertexShaderEntry = vertexValues["entry"].get<std::string>();
			}
			// read fragment shader
			json::iterator itFragment = programJson.find("fragment");
			if (itFragment != programJson.end() && itFragment.value().is_object())
			{
				json fragmentValues = itFragment.value();
				if (fragmentValues.count("source") > 0)
					fragmentShaderName = fragmentValues["source"].get<std::string>();
				if (fragmentValues.count("entry") > 0)
					fragmentShaderEntry = fragmentValues["entry"].get<std::string>();
			}
		}

		if (!vertexShaderName.empty())
		{
			// check if material already exists
			RenderShader* vertexShader = _pResourceManagerPrivate->FindRenderShaderResource(vertexShaderName.c_str());
			if (!vertexShader)
			{
				// create new shader object
				vertexShader = AllocateObject<RenderShader>(*_pResourceManagerPrivate->GetEngineAllocator()
								, *_pResourceManagerPrivate->GetRenderDevice(), "vertex", language.c_str());
				if (vertexShader)
				{
					LoadShader(objectFinder, vertexShaderName, vertexShader);
					// set shader entry function if available
					if (!vertexShaderEntry.empty())
						vertexShader->SetShaderEntryFunc(vertexShaderEntry.c_str());

					// Insert new shader into our map
					_pResourceManagerPrivate->InsertRenderShaderResource(vertexShaderName.c_str(), vertexShader);
				}
			}
			material->_vertexShader = vertexShader;
		}
		if (!fragmentShaderName.empty())
		{
			// check if material already exists
			RenderShader* fragmentShader = _pResourceManagerPrivate->FindRenderShaderResource(fragmentShaderName.c_str());
			if (!fragmentShader)
			{
				// create new shader object
				fragmentShader = AllocateObject<RenderShader>(*_pResourceManagerPrivate->GetEngineAllocator()
								, *_pResourceManagerPrivate->GetRenderDevice(), "fragment", language.c_str());
				if (fragmentShader)
				{
					LoadShader(objectFinder, fragmentShaderName, fragmentShader);
					// set shader entry function if available
					if (!fragmentShaderEntry.empty())
						fragmentShader->SetShaderEntryFunc(fragmentShaderEntry.c_str());

					// Insert new shader into our map
					_pResourceManagerPrivate->InsertRenderShaderResource(fragmentShaderName.c_str(), fragmentShader);
				}
			}
			material->_fragmentShader = fragmentShader;
		}
	}

	return true;
}

}
