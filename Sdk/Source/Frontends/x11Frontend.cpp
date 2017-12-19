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
	_windowId = xcb_generate_id(_connection);
  	uint32_t propNames = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;
  	uint32_t propValues[2];
  	propValues[0] = screen->white_pixel;
  	propValues[1] = XCB_EVENT_MASK_EXPOSURE | XCB_EVENT_MASK_KEY_PRESS;

	uint16_t windowBorder = (windowInfo.borderLess) ? 0 : 1;

  	// Create window
  	xcb_create_window(
  		_connection, 
  		screen->root_depth, 
     	_windowId, 
     	screen->root, 
     	windowInfo.xOffset, windowInfo.yOffset, 
     	windowInfo.windowWidth, windowInfo.windowHeight, 
     	windowBorder,
     	XCB_WINDOW_CLASS_INPUT_OUTPUT, 
     	screen->root_visual, 
     	propNames, 
		 propValues);

	// xcb style of window close message via close button
	xcb_intern_atom_cookie_t defaultCooky = xcb_intern_atom(_connection, 1, 12, "WM_PROTOCOLS");
	_defaultReply = xcb_intern_atom_reply(_connection, defaultCooky, 0);
	xcb_intern_atom_cookie_t closeCooky = xcb_intern_atom(_connection, 0, 16, "WM_DELETE_WINDOW");
	_closeReply = xcb_intern_atom_reply(_connection, closeCooky, 0);

	xcb_change_property(_connection, XCB_PROP_MODE_REPLACE, _windowId, (*_defaultReply).atom, 4, 32, 1, &(*_closeReply).atom);
		 
	// Fill in connection data
	windowInfo.connection = _connection;
	windowInfo.visualId = screen->root_visual;
	windowInfo.window = _windowId;

	return true;
}

void X11Frontend::RegisterInputEngine(InputEngine* input)
{
	_inputEngine = input;
}

void X11Frontend::DisplayWindow()
{
	if (_connection)
	{
		xcb_map_window(_connection, _windowId);
  		xcb_flush(_connection);
  	}
}

bool X11Frontend::HandleWindowMessage()
{
	xcb_generic_event_t* event;

	while((event = xcb_wait_for_event(_connection)) ) 
	{
		switch(event->response_type & ~0x80) 
		{
			case XCB_KEY_PRESS:
			{
				xcb_key_press_event_t* keyEvent = (xcb_key_press_event_t*)event;
				if (keyEvent->detail == 9) // ESC key
					return false;
			}
			break;
			case XCB_EXPOSE:
			break;
			case XCB_CLIENT_MESSAGE:
			{
				xcb_client_message_event_t* clientEvent = (xcb_client_message_event_t*)event;
				if (clientEvent->data.data32[0] == (*_closeReply).atom)
					return 0;
			}
		}
		 
		free(event);
  	}

	return true;
}

}
