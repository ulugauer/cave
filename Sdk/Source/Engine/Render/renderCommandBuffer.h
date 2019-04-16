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

/// @file renderCommandBuffer.h
///       Render command buffer interface

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
class RenderPass;
class RenderFrameBuffer;
class HalCommandBuffer;
class RenderCommandBuffer;


/**
* @brief Render command render pass info
*/
struct CAVE_INTERFACE RenderCmdRenderPassInfo
{
	RenderPass* _renderPass;			///< Pointer to an instance of RenderPass object
	RenderFrameBuffer* _framebuffer;	///< FrameBuffer object we render to or nullptr for swapchain framebuffer
	int32_t _swapChainIndex;			///< If _framebuffer = nullptr fetch framebuffer from swap chain 
	HalRect2D _renderRect;				///< Render areaa within the framebuffer
	uint32_t _clearValueCount;			///< Count of clear value arrau
	const HalClearValue* _clearValues;	///< Array of clear values

	RenderCmdRenderPassInfo()
		: _renderPass(nullptr)
        , _framebuffer(nullptr)
		, _swapChainIndex(-1)
	{}
};

/**
* @brief Represents a command buffer interface
*/
class CAVE_INTERFACE RenderCommandBuffer : public CaveRefCount
{
public:

	/**
	* @brief Constructor
	*
	* @param[in] renderDevice		Pointer to render device
	* @param[in] commandBuffer		HAL command buffer
	*
	*/
	RenderCommandBuffer(RenderDevice& renderDevice
		, HalCommandBuffer* commandBuffer);
	/** @brief destructor */
	virtual ~RenderCommandBuffer();

	/**
	* @brief Get low level HAL handle
	*
	* @return HalCommandBuffer handle
	*/
	HalCommandBuffer* GetHalHandle() { return _halCommandBuffer; }

private:
	RenderDevice& _renderDevice;			///< Render device object
	HalCommandBuffer* _halCommandBuffer;	///< Pointer to low level command buffer object
};

}
/** @}*/




