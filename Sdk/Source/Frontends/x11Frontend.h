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

/// @file x11Frontend.h
///       X11 frontend abstraction

#include "frontend.h"

/** \addtogroup frontend 
*  @{
*		
*/

namespace cave
{

/**
* X11 frontend  implementation
*/
class CAVE_INTERFACE X11Frontend : public IFrontend
{
public:
    X11Frontend();
    virtual ~X11Frontend();

public: 

	/**
	 * @brief Create a window on a specific target
	 *
	 * @param[in,out] windowInfo 	Window specific creation info
	 *
	 * @return  true if successfull
	 */
	virtual bool CreateOsWindow(RenderWindowInfo& windowInfo) override;

	/**
	* @brief Set Input engine
	*
	* @param[in] InputEngine 	Pointer to input engine object
	*
	*/
	virtual void RegisterInputEngine(InputEngine* input) override;

	/**
	* @brief Display the window
	*
	*/
	virtual void DisplayWindow() override;

	/**
	* @brief Handle window messages
	*
	* @return true as long we handle messages
	*/
	virtual bool HandleWindowMessage() override;

private:
	xcb_connection_t* _connection;	///< xcb connection
	xcb_window_t _windowId;	///< window id
	xcb_visualid_t _visualId;		///< xcb visual id
	xcb_intern_atom_reply_t* _defaultReply; ///< default window messages
	xcb_intern_atom_reply_t* _closeReply; ///< window close message
};

}

/** @}*/
