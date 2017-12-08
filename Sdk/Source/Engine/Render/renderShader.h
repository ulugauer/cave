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

/// @file renderShader.h
///       Render shader interface

#include "engineTypes.h"
#include "Memory/allocatorGlobal.h"

#include <memory>
#include <mutex>
#include <atomic>
#include <vector>

/** \addtogroup engine
*  @{
*		This module contains all code related to the engine
*/

namespace cave
{

/// forward declaration
class HalShader;
class RenderDevice;

/**
* Interface of renderer materials
*/
class CAVE_INTERFACE RenderShader
{
public:
	/**
	* @brief Constructor
	*
	* @param[in] renderDevice	Pointer to render device
	* @param[in] shaderType		Shader type string
	* @param[in] shaderLanguage	Shader language string
	*
	*/
	RenderShader(RenderDevice& renderDevice, const char* shaderType, const char* shaderLanguage);
	/** @brief Destructor */
	virtual ~RenderShader();
	/** @brief copy constructor */
	RenderShader(const RenderShader& rhs) = delete;
	/** assigment operator */
	RenderShader& operator=(const RenderShader& rhs) = delete;

	/**
	* @brief Increment usage count of this shader module
	*
	*/
	void IncrementUsageCount();
	/**
	* @brief Decrement usage count of this shader module
	*
	*/
	void DecrementUsageCount();

	/**
	* @brief Set shader source code
	*
	* @param[in] code	Pointer to source code (Readable shader code or byte code)
	*
	*/
	void SetShaderSource(const std::vector<char>& code);

	/**
	* @brief Set shader entry function
	*
	* @param[in] funcName	Name of the shader module entry point
	*
	*/
	void SetShaderEntryFunc(const char* funcName);

	/**
	* @brief[in] Compile a shader and create a vulkan shader module
	*
	* @return true if compiling was successful
	*/
	bool CompileShader();

	/**
	* @brief Get low level HAL handle
	*
	* @return HalShader handle
	*/
	HalShader* GetHalHandle() { return _halShader; }

private:
	RenderDevice& _renderDevice;	///< Render device object
	HalShader* _halShader;	///< Pointer to low level shader object
	size_t _sourceSize;	///< Size of source code in bytes
	char* _source;	///< Pointer to source code might be a readable string or byte code
	int32_t _refCount;	///< Our reference count
	class CAVE_INTERFACE std::mutex _refCountMutex; ///< mutex object for ref counter
};

}

/** @}*/


