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

/// @file vulkanConversion.h
///       Hal types to vulkan types

/** \addtogroup backend
*  @{
*
*/

#include "halTypes.h"

#include "vulkan.h"

namespace cave
{

///< forwards
class VulkanRenderDevice;

/**
* @brief This class converts Hal types to vulkan types
*/
class VulkanTypeConversion
{
public:

	/**
	* @brief Convert from generic pipeline bind point to vulkan pipeline bind point
	*
	* @param[in] pipelineBindPoint	Generic pipeline bind point
	*
	* @return Vulkan pipeline bind point
	*/
	static VkPipelineBindPoint ConvertPipelineBindPointToVulkan(HalPipelineBindPoints pipelineBindPoint);

	/**
	* @brief Convert from generic image format to vulkan image format
	*
	* @param[in] imageFormat	Generic image format
	*
	* @return Vulkan image format
	*/
	static VkFormat ConvertImageFormatToVulkan(HalImageFormat imageFormat);

	/**
	* @brief Convert from generic sample count to vulkan sample count
	*
	* @param[in] sampleCount	Generic sample count
	*
	* @return Vulkan sample count flags
	*/
	static VkSampleCountFlagBits ConvertSampleCountToVulkan(HalSampleCount sampleCount);

};

}

/** @}*/

