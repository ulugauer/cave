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

/// @file osPlatformLibLinux.cpp
///       Cave OS linux library interface.

#include <dlfcn.h>
#include <unistd.h>

#include "osPlatformLib.h"

namespace cave
{

// load a library
osLibraryHandle OsPlatformLib::OsLoadLibrary(char *name)
{
    std::string soName;
    soName = name;
    soName += ".so"; 
    osLibraryHandle handle = dlopen(soName.c_str(), RTLD_LAZY)

    return handle;
}

// unload a library
void OsPlatformLib::OsUnloadLibrary(osLibraryHandle handle)
{
	dlclose(handle);
}

// get a function pointer
osProcAddress OsPlatformLib::OsGetProcAddress(osLibraryHandle handle, char *name)
{
    osProcAddress address = nullptr;

    address = dlsym(handle, name);

    return address;
}

}
