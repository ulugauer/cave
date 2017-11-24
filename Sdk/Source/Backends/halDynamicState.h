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

/// @file halDynamicState.h
///       Hardware dynamic abstraction

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

///< forwards
class HalRenderDevice;

/**
*  @brief A strongly typed enum class representing dynamic states
*/
enum class DynamicState
{
	Viewport = 0,
	Scissor,
	LineWidth,
	DepthBias,
	BlendConstants,
	DepthBounds,
	StencilCompareMask,
	StencilWriteMask,
	StencilReference
};

/**
* @brief Tracks what of the pipeline is a dynamic state
*/
class HalDynamicState
{
public:
	/**
	* @brief Constructor
	*
	* @param[in] renderDevice		Pointer to render device object
	* @param[in] dynamicStates		Array of dynamic states
	*/
	HalDynamicState(HalRenderDevice* renderDevice, caveVector<DynamicState>& dynamicStates);

	/** @brief Destructor */
	virtual ~HalDynamicState();

private:
	HalRenderDevice* _pDevice;	///< Pointer to device object
	caveVector<DynamicState> _dynamicStates;	///< Array of dynamic tracked states
};

}

/** @}*/

