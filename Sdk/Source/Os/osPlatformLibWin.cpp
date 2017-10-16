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

/// @file osPlatformLibWin.cpp
///       Cave OS windows library interface.

/// Define WIN32_LEAN_AND_MEAN to exclude APIs such as Cryptography, DDE, RPC, Shell, and Windows Sockets. 
#define WIN32_LEAN_AND_MEAN
/// Exclude rarely-used stuff from Windows headers
#define VC_EXTRALEAN
#include <windows.h>

#include "osPlatformLib.h"

namespace cave
{

// load a library
osLibraryHandle OsPlatformLib::OsLoadLibrary(char *name)
{
    // build name
    std::string dllName;
    dllName = name;
    dllName += ".dll"; 
	osLibraryHandle handle = LoadLibrary((LPCSTR)dllName.c_str());

    return handle;
}

// unload a library
void OsPlatformLib::OsUnloadLibrary(osLibraryHandle handle)
{
	FreeLibrary((HMODULE)handle);
}

// get a function pointer
osProcAddress OsPlatformLib::OsGetProcAddress(osLibraryHandle handle, char *name)
{
    osProcAddress address = nullptr;

	address = GetProcAddress((HMODULE)handle, (LPCSTR)name);

    return address;
}

} 

