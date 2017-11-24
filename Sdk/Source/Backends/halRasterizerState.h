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

/// @file halRasterizerState.h
///       Hardware rasterizer state abstraction

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
enum class CullMode
{
	None = 0,
	Front,
	Back,
	FrontAndBack
};

/**
*  @brief A strongly typed enum class representing polygons front face
*/
enum class FrontFace
{
	CounterClockwise = 0,
	Clockwise,
};

/**
*  @brief A strongly typed enum class representing polygon render mode
*/
enum class PolygonMode
{
	Fill = 0,
	Line,
	Point
};

/**
* @brief Rasterizer state setup
*/
struct HalRasterizerSetup
{
	CullMode _cullMode;				///< Polygon culling mode
	FrontFace _frontFace;			///< Front face mode
	bool _depthClampEnable;			///< Clamp depth values to depth range
	bool _depthBiasEnable;			///< Add constant value to depth
	float _depthBiasConstantFactor;	///< Constant bias value
	float _depthBiasClamp;			///< Depth bias clamp value
	float _depthBiasSlopeFactor;	///< Depth bias slope
	float _lineWidth;				///< Line thickness
	PolygonMode _polygonMode;		///< Polygon render mode
	bool _rasterizerDiscardEnable;	///< Disable rasterizer state
};

/**
* @brief Describes setup of the rasterizer state
*/
class HalRasterizerState
{
public:
	/**
	* @brief Constructor
	*
	* @param[in] rasterizerState	Rasterizer setup struct
	*/
	HalRasterizerState(HalRasterizerSetup& rasterizerState);

	/** @brief Destructor */
	virtual ~HalRasterizerState();

private:
	HalRasterizerSetup _rasterizerState;	///< Rasterizer state
};

}

/** @}*/

