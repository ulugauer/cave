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

/// @file halPipelineLayout.h
///       Hardware pipeline layout abstraction

#include "engineDefines.h"
#include "halInstance.h"
#include "Memory/allocatorBase.h"
#include "Common/caveVector.h"

#include <iostream>		// includes exception handling
#include <memory>

/** \addtogroup backend
*  @{
*
*/

namespace cave
{

/**
*  @brief A strongly typed enum class representing the shader stages
*/
enum class ShaderStages
{
	Vertex = 1,
	TessellationControl = 2,
	TessellationEvaluation = 4,
	Geometry = 8,
	Fragment = 16,
	Compute = 32
};

typedef uint32_t ShaderStagesFlags;	///< Shader stages flags

typedef struct DescriptorSetLayout_T *setLayout;	///< Opqaue handle
/**
* @brief Pipeline descriptor set layout
*/
struct HalDescriptorSetLayout
{
	setLayout _descriptorLayoutSet; ///< dummy for now
};

/**
* @brief Pipeline push constants setup
*/
struct HalPushConstantRange
{
	ShaderStagesFlags _shaderStagesFlags; ///< Enabled shader stages
	uint32_t _offset;	///< Range offset in bytes (must be a multiple of 4)
	uint32_t _size;		///< Range size on bytes (must be a multiple of 4)
};

/**
* @brief Describes the pipeline layout
*/
class HalPipelineLayout
{
public:
	
	/**
	* @brief Constructor
	*
	* @param[in] renderDevice			Pointer to render device object
	* @param[in] descriptorSetLayouts	Pipeline set layouts array
	* @param[in] pushConstants			Pipeline push constant ranges array
	*/
	HalPipelineLayout(HalRenderDevice* renderDevice
					, caveVector<HalDescriptorSetLayout>& descriptorSetLayouts
					, caveVector<HalPushConstantRange>& pushConstants);

	/** @brief Destructor */
	virtual ~HalPipelineLayout();

private:
	HalRenderDevice* _pDevice;	///< Pointer to device object
	caveVector<HalDescriptorSetLayout> _descriptorSetLayouts;	///< Pipeline set layouts array
	caveVector<HalPushConstantRange> _pushConstants;	///< Pipeline push constant ranges array
};

}

/** @}*/

#pragma once
