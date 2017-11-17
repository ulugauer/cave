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

/// @file halShader.h
///       Hardware shader module abstraction

#include "engineDefines.h"
#include "Memory/allocatorBase.h"

#include <memory>

/** \addtogroup backend
*  @{
*
*/

namespace cave
{

/**
*  @brief A strongly typed enum class representing available shader types
*/
enum class ShaderType
{
	Vertex = 0,
	TessControl,
	TessEval,
	Geometry,
	Fragment,
	Compute,
	Unknown
};

/**
*  @brief A strongly typed enum class representing supported shader languages
*/
enum class ShaderLanguage
{
	Glsl = 0,
	Hlsl,
	Spirv,
	Unknown
};

/**
* Abstraction type of a device shader
*/
class HalShader
{
public:
	/**
	* @brief Constructor
	*
	* @param[in] type		Shader type
	* @param[in] language	Shader language
	*/
	HalShader( ShaderType type, ShaderLanguage language);

	/** @brief Destructor */
	virtual ~HalShader();
	
	/**
	* @brief Static helper function to convert from string to shader type
	*
	* @param[in] typeString		Shader type string
	*
	* @return ShaderType enum value
	*/
	static ShaderType ConvertToShaderType(const char* typeString);

	/**
	* @brief Static helper function to convert from string to shader language
	*
	* @param[in] languageString		Shader language string
	*
	* @return ShaderLanguage enum value
	*/
	static ShaderLanguage ConvertToShaderLanguage(const char* languageString);

	/**
	* @brief Set shader entry function
	* Note that vulkan would support multiple entry points per module.
	* However we currently support only one.
	*
	* @param[in] funcName	Name of the shader module entry point
	*
	*/
	virtual void SetShaderEntryFunc(const char* funcName);

	/**
	* @brief[in] Compile a shader and create a shader module
	*
	* @param[in] code	Source code must be 4 byte aligned for vulkan
	* @param[in] count	Source code size in bytes
	*
	* @return true if compiling was successful
	*/
	virtual bool CompileShader(const char* code, size_t count) = 0;

protected:
	ShaderType _type;				///< Shader type
	ShaderLanguage _language;		///< Shader language
	std::string _entryFunc;			///< Name of shader module entry func
};

}

/** @}*/
