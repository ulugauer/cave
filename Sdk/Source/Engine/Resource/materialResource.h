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

/// @file materialResource.h
///       Handles material assets

#include "resourceManagerPrivate.h"

/** \addtogroup engine
*  @{
*	
*/

namespace cave
{

/**
* Load material assets
*/
class CAVE_INTERFACE MaterialResource
{
public:
	/**
	* @brief Constructor
	*
	* @param[in] rm	Pointer to resource manager
	*
	*/
	MaterialResource(ResourceManagerPrivate* rm);
	/** @brief Destructor */
	~MaterialResource();


	/**
	* @brief Load a material asset
	*
	* @param objectFinder	Helper class to find resource
	* @param file			String to json file
	*
	* @return true if successful
	*/
	virtual bool LoadMaterialAsset(ResourceObjectFinder& objectFinder, const char* file);

private:

	/**
	* @brief Load a material asset from a json file
	*
	* @param objectFinder	Helper class to find resource
	* @param fileStream	File stream where we read from
	*
	* @return true if successful
	*/
	bool LoadMaterialJson(ResourceObjectFinder& objectFinder, std::ifstream& fileStream);

private:
	ResourceManagerPrivate* _pResourceManagerPrivate;	///< Pointer to private resource manger
};

}

/** @}*/
