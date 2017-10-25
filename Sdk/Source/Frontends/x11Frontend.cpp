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

/// @file x11Frontend.cpp
///       Hardware render device abstraction

#include "x11Frontend.h"

namespace cave
{

X11Frontend::X11Frontend()
	: _connection(nullptr)
	, _windowId(0)
	, _visualId(0)
{

}

X11Frontend::~X11Frontend()
{
	if (_connection)
	 	xcb_disconnect(_connection);
} 

bool X11Frontend::CreateOsWindow(RenderWindowInfo& windowInfo)
{
	// Create xcb connection
	_connection = xcb_connect(NULL, NULL);
	if (xcb_connection_has_error(_connection)) 
	{
		throw EngineError("Failed to create xcb connection");
	}

	// Access screen
	const xcb_setup_t* setup = xcb_get_setup(_connection);
	xcb_screen_t* screen = xcb_setup_roots_iterator(setup).data;
	if (!screen) 
	{
		throw EngineError("Failed to get screen data");
	}

	// Generate a window id
	_windowId = xcb_generate_id(conn);
  	uint32_t propNames = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;
  	uint32_t propValues[2];
  	propValues[0] = screen->white_pixel;
  	propValues[1] = XCB_EVENT_MASK_EXPOSURE | XCB_EVENT_MASK_KEY_PRESS;

  	// Create window
  	xcb_create_window(
  		_connection, 
  		screen->root_depth, 
     	_windowId, 
     	screen->root, 
     	windowInfo.xOffset, windowInfo.yOffset, 
     	indowInfo.windowWidth, indowInfo.windowHeight, 
     	1,													// window border
     	XCB_WINDOW_CLASS_INPUT_OUTPUT, 
     	screen->root_visual, 
     	prop_name, 
     	&prop_value);

	return true;
}

void WindowFrontend::DisplayWindow()
{
	if (_connection)
	{
		xcb_map_window(_connection, _windowId);
  		xcb_flush(_connection);
  	}
}

bool WindowFrontend::HandleWindowMessage()
{
	xcb_generic_event_t event;

	while((event = xcb_wait_for_event(conn)) ) 
	{
  		switch (event->response_type) 
  		{
  			switch(event->response_type) 
  			{
			  case XCB_KEY_PRESS:
			    printf("Keycode: %d\n", ((xcb_key_press_event_t*)event)->detail);
			    return false;

			  case XCB_EXPOSE:
			    break;
			}
  		}
  	}

	return true;
}

}
