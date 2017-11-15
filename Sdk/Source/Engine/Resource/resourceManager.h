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
#pragma once

/// @file resourceManager.h
///       Handle all kind of resources (images, shaders,...)

#include "renderMaterial.h"
#include "resourceManagerPrivate.h"

/** \addtogroup engine
*  @{
*		This module contains all code related to resource handling
*/

namespace cave
{

// forwards
class RenderDevice;

/**
* Interface to Global Resource Manager
*/
class CAVE_INTERFACE ResourceManager
{
public:
	/**
	* @brief Constructor
	*
	* @param[in] device	Pointer to render device
	* @param[in] applicationPath	Path to application
	* @param[in] projectPath	Path to project
	*
	*/
	ResourceManager(RenderDevice* device, const char* applicationPath, const char* projectPath);
	/** @brief Destructor */
	~ResourceManager();
	/** @brief copy constructor */
	ResourceManager(const ResourceManager&) = delete; // no copy constructor
	ResourceManager& operator=(const ResourceManager&) = delete; // no assignment operator

	/**
	* @brief GetAllocator
	*
	* @return Pointer Engine allocator
	*/
	std::shared_ptr<AllocatorGlobal> GetEngineAllocator();

	/**
	* @brief Load a material asset
	*
	* @param[in] file					String to file
	*
	* @return true if successful
	*/
	virtual RenderMaterial LoadMaterialAsset(const char* file);


private:
	ResourceManagerPrivate* _pResourceManagerPrivate;	///< Pointer to private resource manger
};

}

/** @}*/
