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

// forwards
class RenderMaterial;

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
	* @param[in] objectFinder	Helper class to find resource
	* @param[in] file			String to json file
	*
	* @return RenderMaterial object
	*/
	virtual RenderMaterial* LoadMaterialAsset(ResourceObjectFinder& objectFinder, const char* file);

private:

	/**
	* @brief Load a material asset from a json file
	*
	* @param[in] objectFinder	Helper class to find resource
	* @param[in] fileStream	File stream where we read from
	* @param[in|out] material	Pointer to material we fill with data
	*
	* @return true if successful
	*/
	bool LoadMaterialJson(ResourceObjectFinder& objectFinder, std::ifstream& fileStream, RenderMaterial* material);

private:
	ResourceManagerPrivate* _pResourceManagerPrivate;	///< Pointer to private resource manger
};

}

/** @}*/

