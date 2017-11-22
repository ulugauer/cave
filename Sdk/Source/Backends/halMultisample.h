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

/// @file halMultisample.h
///       Hardware multisample state abstraction

#include "engineDefines.h"
#include "halInstance.h"
#include "Memory/allocatorBase.h"

#include <iostream>		// includes exception handling
#include <memory>

/** \addtogroup backend
*  @{
*
*/

namespace cave
{

/**
*  @brief A strongly typed enum class representing polygon cull mode
*/
enum class SampleCount
{
	SampleCount1 = 0,
	SampleCount2,
	SampleCount4,
	SampleCount8,
	SampleCount16,
	SampleCount32,
	SampleCount64
};

/**
* @brief Multisample state data
*/
struct HalMultisampleData
{
	bool _alphaToCoverageEnable;		///< Enable temporary coverage value based on the alpha value
	bool _alphaToOneEnable;				///< Enable replacing alpah output to one
	float _minSampleShading;			///< Minimum fraction value
	uint32_t* _pSampleMask;				///< Coverage bitmask
	SampleCount _rasterizationSamples;	///< Sample count
	bool _sampleShadingEnable;			///< Enable per sample fragment execution
};

/**
* @brief Describes the multisample state
*/
class HalMultisample
{
public:
	/**
	* @brief Constructor
	*
	*/
	HalMultisample(HalMultisampleData& multisampleData);

	/** @brief Destructor */
	virtual ~HalMultisample();

private:
	HalMultisampleData _multisampleData;	///< Multisample state
};

}

/** @}*/

