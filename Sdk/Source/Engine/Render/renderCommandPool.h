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

/// @file renderCommandPool.h
///       Render command pool interface

#include "Common/caveRefCount.h"
#include "Memory/allocatorGlobal.h"
#include "halTypes.h"

#include <memory>

/** \addtogroup engine
*  @{
*		This module contains all code related to the engine
*/

namespace cave
{

/// forward declaration
class RenderDevice;
class HalCommandPool;

/**
* @brief Represents a command pool interface
*/
class CAVE_INTERFACE RenderCommandPool : public CaveRefCount
{
public:

	/**
	* @brief Constructor
	*
	* @param[in] renderDevice		Pointer to render device
	* @param[in] commandPoolInfo	Command pool creation info
	*
	*/
	RenderCommandPool(RenderDevice& renderDevice
		, HalCommandPoolInfo& commandPoolInfo);
	/** @brief destructor */
	virtual ~RenderCommandPool();

	/**
	* @brief Get low level HAL handle
	*
	* @return HalColorBlend handle
	*/
	HalCommandPool* GetHalHandle() { return _halCommandPool; }

private:
	RenderDevice& _renderDevice;	///< Render device object
	HalCommandPool* _halCommandPool;	///< Pointer to low level command pool object
};

}
/** @}*/



