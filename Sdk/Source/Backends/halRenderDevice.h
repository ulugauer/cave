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

/// @file halRenderDevice.h
///       Hardware render device abstraction

#include "engineDefines.h"
#include "halInstance.h"
#include "Memory/allocatorBase.h"
#include "Vulkan/vk_platform.h"

#include <iostream>		// includes exception handling
#include <memory>

/** \defgroup backend Backend 
*  @{
*		This module handles the hardware abstractions of
*		various rendering API like DirectX, Vulkan
*/

namespace cave
{

/**
* swap chain creation info
*/
typedef struct SwapChainInfo
{
	uint32_t surfaceWidth;	///< Surface width
	uint32_t surfaceHeight;	///< Surface height
	uint32_t colorBits;		///< Amount of total colorbits (24, 32)
	uint32_t depthBits;		///< Amount of total depthbits (24, 32)
	bool fullscreen;	///< Create fullscreen window
#ifdef VK_USE_PLATFORM_WIN32_KHR
	HINSTANCE hInstance;	///< Filled in at CreateOsWindow call
	HWND hWindow;	///< Filled in at CreateOsWindow call
#else
	xcb_connection_t* connection;		///< xcb connection. Filled in at CreateOsWindow call
	xcb_visualid_t visualId;		///< xcb visual id. Filled in at CreateOsWindow call
	xcb_window_t window;		///< Window id. Filled in at CreateOsWindow call
#endif
}SwapChainInfo;

/**
* Abstraction type of a device instance
*/
class HalRenderDevice
{
public:
	/**
	* @brief Constructor
	*
	*/
	HalRenderDevice();

	/** @brief Destructor */
	virtual ~HalRenderDevice();

	/**
	* @brief Create a swap chain
	*
	* @param[in] swapChainInfo	Swap chain creation info
	*
	*/
	virtual void CreateSwapChain(SwapChainInfo& swapChainInfo) = 0;
};

}

/** @}*/
