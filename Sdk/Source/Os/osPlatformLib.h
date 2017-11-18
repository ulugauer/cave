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


/// @file osPlatformLib.h
///       cave OS library interface.

#include <string>

/** \defgroup os System
*  @{
*		This module handles the operating system abstraction
*/


namespace cave
{

/// osLibrary handle (should be treated as opaque at all layers)
typedef void* osLibraryHandle;
/// osProcAddress function pointer
typedef void* osProcAddress;

/**
* Abstraction of os library loading functions
*/
class OsPlatformLib
{
	public:
	/**
	* @brief Load a library
	*
	* @param[in] name		Name of the library to load
	* @param[in] numSuffix	Add suffix to lib name
	*
	* @returns a handle to a library
	*/
	static osLibraryHandle OsLoadLibrary(const char *name, const char* numSuffix);

	/**
	* @brief Unload a library
	*
	* @param[in] handle Handle returned from a osLoadLibrary call
	*
	* @returns none
	*/
	static void OsUnloadLibrary(osLibraryHandle handle);

	/**
	* @brief Load a function from a library
	*
	* @param[in] handle   Handle to a module
	* @param[in] name     Name of the function to load
	*
	* @returns a address to a function or nullptr on failure
	*/
	static osProcAddress OsGetProcAddress(osLibraryHandle handle, const char *name);
};

}

/** @}*/
