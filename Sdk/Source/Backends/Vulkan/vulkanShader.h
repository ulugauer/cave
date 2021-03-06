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

/// @file vulkanShader.h
///       Vulkan shader module

#include "halShader.h"
#include "osPlatformLib.h"

#include "vulkan.h"

/** \addtogroup backend
*  @{
*
*/

namespace cave
{

///< forwards
class VulkanRenderDevice;

/**
* Vulkan render device
*/
class VulkanShader : public HalShader
{
public:
	/**
	* @brief Constructor
	*
	* @param[in] device	Pointer to device object
	* @param[in] type		Shader type
	* @param[in] language	Shader language
	*
	*/
	VulkanShader(VulkanRenderDevice* device, ShaderType type, ShaderLanguage language);

	/** @brief Destructor */
	virtual ~VulkanShader();

	/**
	* @brief Get vulkan logical device handle
	*
	* @return Lowlevel vulkan handle
	*/
	VkShaderModule GetShaderHandle() { return _vkShader; }


	/**
	* @brief Compile a shader and create a vulkan shader module
	*
	* @param[in] code	Source code must be 4 byte aligned for vulkan
	* @param[in] count	Source code size in bytes
	*
	* @return true if compiling was successful
	*/
	bool CompileShader(const char* code, size_t count) override;

	/**
	* @brief Fill structure with matching values
	*
	* @param[in,out] info	Vulkan structure to fill in
	*
	* @return true if succssfuly filled in
	*/
	bool GetShaderStageInfo(VkPipelineShaderStageCreateInfo& info);

private:
	VulkanRenderDevice* _pDevice;	///< Pointer to device object
	VkShaderModule _vkShader;	///< Handle to vulkan shader module
};

}

/** @}*/

