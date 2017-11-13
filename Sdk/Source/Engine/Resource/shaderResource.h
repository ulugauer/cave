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

/// @file shaderResource.h
///       Handles shader assets

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
class CAVE_INTERFACE ShaderResource
{
public:
	/**
	* @brief Constructor
	*
	* @param[in] rm	Pointer to resource manager
	*
	*/
	ShaderResource(ResourceManagerPrivate* rm);
	/** @brief Destructor */
	~ShaderResource();


	/**
	* @brief Load a shader asset
	*
	* @param objectFinder	Helper class to find resource
	* @param shaderFile		String to shader file including path
	* @param language		Language type string (glsl, hlsl, spirv)
	* @param type			Shader type string (vertex, fragment, geometry, tessellation)
	*
	* @return true if successful
	*/
	virtual bool LoadShaderAsset(ResourceObjectFinder& objectFinder, const char* shaderFile, const char* language, const char* type);

private:

	/**
	* @brief Load a shader code from file
	*
	* @param fileStream	File stream where we read from
	*
	* @return true if successful
	*/
	bool ParseShader(std::ifstream& fileStream);

	/**
	* @brief Check if shader languge is supported
	*
	* @param language	Shader language string (glsl, spirv, hlsl)
	*
	* @return true if supported
	*/
	bool IsLanguageSupported(const char* language);

	/**
	* @brief Check if program type is supported
	*
	* @param type	Program type string (vertex, fragment)
	*
	* @return true if supported
	*/
	bool IsProgramTypeSupported(const char* type);

private:
	ResourceManagerPrivate* _pResourceManagerPrivate;	///< Pointer to private resource manger
};

}

/** @}*/


