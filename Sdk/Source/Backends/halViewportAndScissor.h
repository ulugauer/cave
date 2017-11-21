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

/// @file halViewportAndScissor.h
///       Hardware viewport ande scissor abstraction

#include "engineDefines.h"
#include "halInstance.h"
#include "Memory/allocatorBase.h"
#include "Math/vector2.h"

#include <iostream>		// includes exception handling
#include <memory>

/** \addtogroup backend
*  @{
*
*/

namespace cave
{

/**
* @brief Describes viewport dimensions
*/
struct HalViewport
{
	Vector2f _offset;	///< xy offset
	Vector2f _extend;	///< width, height
	Vector2f _depthRange;	///< viewport min/max depth range
};

/**
* @brief Describes scissor dimensions
*/
struct HalScissor
{
	Vector2ui _offset;	///< xy offset
	Vector2ui _extend;	///< width, height
};

/**
* @brief Describes viewport and scissor settings
*/
class HalViewportAndScissor
{
public:
	/**
	* @brief default Constructor
	*
	* @param[in] viewport	Viewport dimension
	* @param[in] scissor	Scissor dimension
	*
	*/
	HalViewportAndScissor(HalViewport& viewport, HalScissor& scissor);

	/** @brief Destructor */
	virtual ~HalViewportAndScissor();
private:
	HalViewport _viewport; ///< Viewport dimension
	HalScissor _scissor; ///< Scissor dimension
};

}

/** @}*/


