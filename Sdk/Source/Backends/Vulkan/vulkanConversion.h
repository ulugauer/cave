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

	/**
	* @brief Convert from generic cull mode to vulkan cull mode
	*
	* @param[in] mode	Generic cull mode
	*
	* @return Vulkan cull mode
	*/
	static VkCullModeFlagBits ConvertCullModeToVulkan(HalCullMode mode);

	/**
	* @brief Convert from generic front face to vulkan front face
	*
	* @param[in] face	Generic front face value
	*
	* @return Vulkan front face
	*/
	static VkFrontFace ConvertFrontFaceToVulkan(HalFrontFace face);

	/**
	* @brief Convert from generic polygon mode to vulkan polygon mode
	*
	* @param[in] mode	Generic polygon mode
	*
	* @return Vulkan polygon mode
	*/
	static VkPolygonMode ConvertPolygonModeToVulkan(HalPolygonMode mode);

	/**
	* @brief Convert from generic stencil operation to vulkan stencil operation
	*
	* @param[in] op	Generic stencil operation
	*
	* @return Vulkan stencil operation
	*/
	static VkStencilOp ConvertStencilOpToVulkan(HalStencilOp op);

	/**
	* @brief Convert from generic compare operation to vulkan compare operation
	*
	* @param[in] compOp	Generic compare operation
	*
	* @return Vulkan compare operation
	*/
	static VkCompareOp ConvertDepthCompareOpToVulkan(HalCompareOp compOp);

	/**
	* @brief Convert from generic blend factor to vulkan blend factor
	*
	* @param[in] factor	Generic blend factor
	*
	* @return Vulkan blend factor
	*/
	static VkBlendFactor ConvertBlendFactorToVulkan(HalBlendFactor factor);

	/**
	* @brief Convert from generic logical operation to vulkan logical operationr
	*
	* @param[in] op		Generic logical operation
	*
	* @return Vulkan logical operation
	*/
	static VkLogicOp ConvertLogicOpToVulkan(HalLogicOp op);

	/**
	* @brief Convert from generic blend operation to vulkan blend operationr
	*
	* @param[in] op		Generic blend operation
	*
	* @return Vulkan blend operation
	*/
	static VkBlendOp ConvertBlendOpToVulkan(HalBlendOp op);

	/**
	* @brief Convert from generic color components to vulkan color components
	*
	* @param[in] colorComponentFlags	Generic color components
	*
	* @return Vulkan color components
	*/
	static VkColorComponentFlags ConvertColorComponentToVulkan(HalColorComponentFlags colorComponentFlags);

	/**
	* @brief Convert from generic dynamic state to vulkan bdynamic state
	*
	* @param[in] state		Generic dynamic state
	*
	* @return Vulkan dynamic state value
	*/
	static VkDynamicState ConvertDynamicStateToVulkan(HalDynamicStates state);

	/**
	* @brief Convert from generic shader stage flags to vulkan blend shader stage flags
	*
	* @param[in] flags		Generic shader stage flags
	*
	* @return Vulkan shader stage flags
	*/
	static VkShaderStageFlags ConvertBlendOpToVulkan(HalShaderStagesFlags flags);
};

}

/** @}*/

