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

/// @file frontend.h
///       windows frontend abstraction

#include "engineDefines.h"
#include "engineError.h"
#include "Memory/allocatorBase.h"

/** \defgroup frontend Frontend 
*  @{
*		This module handles the window abstractions of
*		various OS
*/

#ifdef _WIN32

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

#elif defined(__linux__)

#include <xcb/xcb.h>		// xcb support 
#include <X11/Xlib.h>		// X11 support 

#endif

#include "inputEngine.h"

#include <memory>

namespace cave
{

/**
* Window creation info
*/
typedef struct FrontendWindowInfo
{
	uint32_t xOffset;			///< Window x position
	uint32_t yOffset;			///< Window y position
	uint32_t windowWidth;		///< Window width
	uint32_t windowHeight;		///< Window height
	uint32_t colorBits;			///< Amount of total color bits (24, 32)
	uint32_t depthBits;			///< Amount of total depth bits (24, 32)
	uint32_t stencilBits;		///< Amount of total stencil bits (8)
	bool borderLess;			///< No window border
	bool fullscreen;			///< Create fullscreen window
	bool offscreen;				///< We do not render to a window surface
#ifdef _WIN32
	HINSTANCE hInstance;		///< Filled in at CreateOsWindow call
	HWND hWindow;				///< Filled in at CreateOsWindow call
#else
	xcb_connection_t* connection;		///< xcb connection. Filled in at CreateOsWindow call
	xcb_visualid_t visualId;			///< xcb visual id. Filled in at CreateOsWindow call
	xcb_window_t window;				///< Window id. Filled in at CreateOsWindow call
#endif
	const char* windowTitle;			///< Window Title
}RenderWindowInfo;

/**
* Window frontend abstract  interface
*/
class CAVE_INTERFACE IFrontend
{
public:
	IFrontend() : _inputEngine(nullptr) {}
	virtual ~IFrontend() { } 

	 /**
	 * @brief Create a window on a specific target
	 *
	 * @param[in,out] windowInfo 	Window specific creation info
	 *
	 * @return  true if successfull
	 */
	virtual bool CreateOsWindow(RenderWindowInfo& windowInfo) = 0; 

	/**
	* @brief Set Input engine
	*
	* @param[in] input 	Pointer to input engine object
	*
	*/
	virtual void RegisterInputEngine(InputEngine* input) = 0;

	/**
	* @brief Display the window
	*
	*/
	virtual void DisplayWindow() = 0;

	/**
	* @brief Handle window messages
	*
	* @return true as long we handle messages
	*/
	virtual bool HandleWindowMessage() = 0;

	/**
	* @brief Factory function for frontend creation
	*
	* @param[in] allocator 	Pointer to global allocator
	*
	* @return  true if successfull
	*/
	static IFrontend* CreateFrontend(std::shared_ptr<AllocatorBase> allocator);

public:
	InputEngine* _inputEngine;	///< Pointer to the input handling
}; 

}

/** @}*/
