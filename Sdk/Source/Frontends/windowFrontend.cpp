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

/// @file windowFrontend.cpp
///       Hardware render device abstraction

#include "windowFrontend.h"

namespace cave
{

/**
* @brief Static window event handling function
*
* @param[hWnd] hWnd 	Window handle
* @param[hWnd] message 	Window message id
* @param[hWnd] wParam 	Window message param
* @param[hWnd] lParam 	Window message param
*
* @return  callback result
*/
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{

	case WM_CREATE:
	{
		//ShowWindow(GetConsoleWindow(), SW_HIDE);
		CREATESTRUCT *pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
		WindowFrontend* pFrontend = reinterpret_cast<WindowFrontend*>(pCreate->lpCreateParams);
		SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)pFrontend);
	}
	return 0;
	/*
	case WM_SIZE:
	{
		LONG_PTR ptr = GetWindowLongPtr(hWnd, GWLP_USERDATA);
		WindowFrontend* pFrontend = reinterpret_cast<WindowFrontend*>(ptr);
	}
	return 0;
	*/
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;

	case WM_DESTROY:
		return 0;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
}


WindowFrontend::WindowFrontend()
	: _hWnd(nullptr)
	, _hInstance(nullptr)
{

}

WindowFrontend::~WindowFrontend()
{
    // destroy window
	if (_hWnd)
		DestroyWindow(_hWnd);
} 

bool WindowFrontend::CreateOsWindow(RenderWindowInfo& windowInfo)
{
	WNDCLASSA   wc;                     // Windows Class Structure
	DWORD       dwExStyle;              // Window Extended Style
	DWORD       dwStyle;                // Window Style
	RECT		WindowRect;
	WindowRect.left = windowInfo.xOffset;
	WindowRect.top = windowInfo.yOffset;
	WindowRect.bottom = windowInfo.yOffset + windowInfo.windowHeight;
	WindowRect.right = windowInfo.xOffset + windowInfo.windowWidth;

	_hInstance = GetModuleHandle(NULL);       // Grab an Instance handle

											  // create window
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;		// Redraw On Size, And Own DC For Window.
	wc.lpfnWndProc = (WNDPROC)WndProc;                  // WndProc Handles Messages
	wc.cbClsExtra = 0;                                  // No Extra Window Data
	wc.cbWndExtra = 0;                                  // No Extra Window Data
	wc.hInstance = _hInstance;                           // Set The Instance
	wc.hIcon = NULL;									// Load The Default Icon
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);			// Load The Arrow Pointer
	wc.hbrBackground = NULL;                            // No Background Required For GL
	wc.lpszMenuName = NULL;                             // We Don't Want A Menu
	wc.lpszClassName = "CaveFrontend";             // Set The Class Name

	//register the class but ignore the error; you will get an error if it is already registered.
	RegisterClassA(&wc);

	// common Window Extended Style
	dwExStyle = WS_EX_APPWINDOW;

	if (windowInfo.fullscreen)                                             // Are We Still In Fullscreen Mode?
	{
		dwStyle = WS_POPUP;                                     // Windows Style
		WindowRect = WindowRect;
	}
	else if (windowInfo.borderLess)
	{
		dwStyle = WS_POPUP;                                     //window style (no border)
	}
	else
	{
		dwExStyle |= WS_EX_WINDOWEDGE;                          // Window Extended Style
		dwStyle = WS_OVERLAPPEDWINDOW;                          // Windows Style
	}

	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);     // Adjust window to true size

	char* title = (windowInfo.windowTitle) ? windowInfo.windowTitle : "caveEngine";

	_hWnd = CreateWindowExA(dwExStyle,							// Extended Style For The Window
		"CaveFrontend",											// Class Name
		title,													// Window Title
		dwStyle |												// Defined Window Style
		WS_CLIPSIBLINGS |										// Required Window Style
		WS_CLIPCHILDREN,										// Required Window Style
		WindowRect.left, WindowRect.top,						// Window Position
		WindowRect.right - WindowRect.left,						// Calculate Window Width
		WindowRect.bottom - WindowRect.top,						// Calculate Window Height
		NULL,													// No Parent Window
		NULL,													// No Menu
		_hInstance,												// Instance
		this);

	if (!_hWnd)
	{
		throw EngineError("Failed to create window");
	}

	windowInfo.hInstance = _hInstance;
	windowInfo.hWindow = _hWnd;

	return true;
}

void WindowFrontend::DisplayWindow()
{
	if (_hWnd)
		ShowWindow(_hWnd, SW_SHOW);
}

bool WindowFrontend::HandleWindowMessage()
{
	MSG msg;

	while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		// Translate the message and dispatch it to WindowProc()
		TranslateMessage(&msg);
		DispatchMessage(&msg);

		// If the message is WM_QUIT, exit the while loop
		if (msg.message == WM_QUIT)
			return false;
		if (msg.message == WM_KEYDOWN)
		{
			switch (msg.wParam)
			{
			case VK_ESCAPE:
				return 0;
			default:
				break;
			}
		}
	}

	return true;
}

}
