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

/// @file inputEngine.h
///       Input and windows event handling

#include "engineDefines.h"
#include "engineError.h"
#include "Memory/allocatorBase.h"

/** \defgroup input Input
*  @{
*		This module handles input and window events of
*		various OS
*/

#include <memory>

namespace cave
{

/**
*  @brief A strongly typed enum class representing key codes
*/
enum class CAVE_KeyCode
{
	CAVE_KEY_NOKEY = 0,
	CAVE_KEY_ESCAPE,
	CAVE_KEY_1,
	CAVE_KEY_2,
	CAVE_KEY_3,
	CAVE_KEY_4,
	CAVE_KEY_5,
	CAVE_KEY_6,
	CAVE_KEY_7,
	CAVE_KEY_8,
	CAVE_KEY_9,
	CAVE_KEY_0,
	CAVE_KEY_A,
};

/**
* @brief Interface to the render egine for input and window events
*/
class CAVE_INTERFACE InputEngine
{
public:
	/**
	* @brief Constructor
	*
	* @param[in] userData 	User specificdata  pointer
	*
	*/
	InputEngine(void* userData) : _userData(userData) {}
	/** @brief Destructor */
	virtual ~InputEngine() { }

public:

	/**
	* @brief handle window resize
	*
	* @param[in] width				Window width
	* @param[in] height				Window height
	*
	* @return  none
	*/
	virtual void HandleWindowResize(uint32_t width, uint32_t height);

	/**
	* @brief handle keyboard input
	*
	* @param[in] keyCode				keycode
	*
	* @return  none
	*/
	virtual void HandleKey(CAVE_KeyCode keyCode);

public:
	void*	_userData;	///< Pointer to user data
};

}


/** @}*/

