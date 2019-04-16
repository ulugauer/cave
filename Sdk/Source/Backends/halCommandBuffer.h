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

/// @file halCommandBuffer.h
///       Hardware command buffer abstraction

#include "engineDefines.h"
#include "halInstance.h"
#include "halTypes.h"
#include "Memory/allocatorBase.h"

#include <memory>

/** \addtogroup backend
*  @{
*
*/

namespace cave
{

///< forwards
class HalRenderDevice;
class HalCommandPool;
class HalRenderPass;
class HalFrameBuffer;
class HalCommandBuffer;


/**
* @brief HAL command render pass info
*/
struct CAVE_INTERFACE HalCmdRenderPassInfo
{
	HalRenderPass* _renderPass;			///< Pointer to an instance of RenderPass object
	HalFrameBuffer* _framebuffer;	///< to do
	int32_t _swapChainIndex;				///< If _framebuffer = nullptr fetch framebuffer from swap chain 
	HalRect2D _renderRect;				///< Render areaa within the framebuffer
	uint32_t _clearValueCount;			///< Count of clear value arrau
	const HalClearValue* _clearValues;	///< Array of clear values

	HalCmdRenderPassInfo()
		: _renderPass(nullptr)
		, _swapChainIndex(-1)
	{}
};

/**
* @brief Represents a command buffer interface
*/
class HalCommandBuffer
{
public:
	/**
	* @brief Constructor
	*
	* @param[in] renderDevice		Pointer to render device object
	*/
	HalCommandBuffer(HalRenderDevice* renderDevice);

	/** @brief Destructor */
	virtual ~HalCommandBuffer();

private:
	HalRenderDevice* _pDevice;	///< Pointer to device object
};

}
 
/** @}*/



