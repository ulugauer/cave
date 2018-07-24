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

/// @file resourceManager.cpp
///       Handle all kind of resources (images, shaders,...)

#include "resourceManager.h"
#include "engineInstancePrivate.h"
#include "engineError.h"
#include "renderDevice.h"

#include "json.hpp"

namespace cave
{

ResourceManager::ResourceManager(RenderDevice* device, const char* applicationPath, const char* projectPath)
	: _pResourceManagerPrivate(nullptr)
{
	// Create our logger. By default no logging
	_pResourceManagerPrivate = AllocateObject<ResourceManagerPrivate>(*device->GetEngineAllocator(), device, applicationPath, projectPath);

	if (!_pResourceManagerPrivate && device->GetEngineLog())
	{
		device->GetEngineLog()->Error("Failed to create resource manager");
		throw EngineError("Failed to resource manager");
	}
}

ResourceManager::~ResourceManager()
{
	if (_pResourceManagerPrivate)
		DeallocateDelete(*_pResourceManagerPrivate->GetEngineAllocator(), *_pResourceManagerPrivate);
}

std::shared_ptr<AllocatorGlobal>
ResourceManager::GetEngineAllocator()
{
	return _pResourceManagerPrivate->GetEngineAllocator();
}

RenderMaterial ResourceManager::LoadMaterialAsset(const char* file)
{
	if (!file)
	{
		_pResourceManagerPrivate->_pRenderDevice->GetEngineLog()->Error("Could not find file %s", file);
		return RenderMaterial(*_pResourceManagerPrivate->GetRenderDevice());
	}

	RenderMaterial* material = _pResourceManagerPrivate->LoadMaterialAsset(file);

	return RenderMaterial(*material);
}

void ResourceManager::LoadImageAssest(const char* file)
{
	if (!file)
	{
		_pResourceManagerPrivate->_pRenderDevice->GetEngineLog()->Error("Could not find file %s", file);
		return;
	}

	_pResourceManagerPrivate->LoadImageAssest(file);
}

}
