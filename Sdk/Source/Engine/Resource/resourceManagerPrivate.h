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

/// @file resourceManagerPrivate.h
///       Handle all kind of resources (images, shaders,...)

#include "engineTypes.h"
#include "Memory/allocatorGlobal.h"

#include <memory>
#include <string>
#include <vector>
#include <map>

/** \addtogroup engine
*  @{
*		This module contains all code related to resource handling
*/

namespace cave
{

/// forward declaration
class RenderDevice;
class RenderMaterial;

// forwards
class ResourceManagerPrivate;
class MaterialResource;

/**
* A helper class to find resources
*/
class ResourceObjectFinder
{
public:
	std::string _projectContentPath;			///< Project path
	std::string _appContentPath;				///< Application binary path
	std::vector<std::string> _localSearchPath;	///< A string array for local search path

	/**
	* @brief Constructor
	*
	* @param[in] rm ResourceManagerPrivate object
	*
	*/
	ResourceObjectFinder(ResourceManagerPrivate& rm);

	/**
	* @brief Open the file in ascii read mode
	*
	* @param[in] file File name string
	* @param[in] fileStream The stream we use for reading
	*
	* @return true if successful
	*/
	bool OpenFileAscii(const char* file, std::ifstream& fileStream);

	/**
	* @brief Open the file in binary read mode
	*
	* @param[in] file File name string
	* @param[in] fileStream The stream we use for reading
	*
	* @return true if successful
	*/
	bool OpenFileBinary(const char* file, std::ifstream& fileStream);

	/**
	* @brief Extract filename from an input string
	*
	* @param[in] file File name string may include path
	*
	* @return filename string
	*/
	std::string GetFileName(const char* file);

	/**
	* @brief Extract filename suffix from an input string
	*
	* @param[in] file File name string may include path
	*
	* @return filename suffix
	*/
	std::string GetFileExt(const char* file);

	/**
	* @brief Extract directory from an input string
	*
	* @param[in] file File name string may include path
	*
	* @return path string
	*/
	std::string GetDirectory(const char* file);

	/**
	* @brief Compare two strings case insensitive
	*
	* @param[in] str1 String 1 to compare
	* @param[in] str2 String 2 to compare
	*
	* @return true if equal
	*/
	bool CaseInsensitiveStringCompare(const std::string& str1, const std::string& str2);
};


typedef std::map<std::string, RenderMaterial*> TResourceMaterialMap;	///< Material objects map

/**
* Global Resource Manager
*/
class ResourceManagerPrivate
{
	friend class ResourceManager;	///< we grant access to our public interface

public:
	/**
	* @brief Constructor
	*
	* @param[in] device				Pointer to render device
	* @param[in] applicationPath	Path to application
	* @param[in] projectPath		Path to project
	*
	*/
	ResourceManagerPrivate(RenderDevice* device
							, const char* applicationPath
							, const char* projectPath);
	/** @brief Destructor */
	~ResourceManagerPrivate();
	/** @brief copy constructor */
	ResourceManagerPrivate(const ResourceManagerPrivate&) = delete; // no copy constructor
	ResourceManagerPrivate& operator=(const ResourceManagerPrivate&) = delete; // no assignment operator

	/**
	* @brief GetAllocator
	*
	* @return Pointer Engine allocator
	*/
	std::shared_ptr<AllocatorGlobal> GetEngineAllocator();

	/**
	* @brief Get application path string
	*
	* @return string
	*/
	const std::string GetApplicationPath() { return _appPath; }

	/**
	* @brief Get project path string
	*
	* @return string
	*/
	const std::string GetProjectPath() { return _projectPath; }

	/**
	* @brief Get render device
	*
	* @return RenderDevice object
	*/
	RenderDevice* GetRenderDevice() { return _pRenderDevice; }

	/**
	* @brief Load a material asset
	*
	* @param file String to file
	*
	* @return true if successful
	*/
	RenderMaterial* LoadMaterialAsset(const char* file);

private:
	RenderDevice* _pRenderDevice;	///< Pointer to the render device we belong to
	std::string _appPath;		///< Application runtime path
	std::string _projectPath;	///< Project root path
	TResourceMaterialMap _materialMap;	///< RenderMaterial object map
};

}

/** @}*/

