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

/// @file engineInstancePrivate.cpp
///       Engine instance abstraction

#include "engineInstancePrivate.h"

#ifdef __GNUC__
#include <unistd.h>	// Needed for readlink
#include <limits.h>
#endif

namespace cave
{

/**
* @brief Helper function to get runtime path
*
* @return application runtime path string
*/
static std::string GetAppPath()
{
	static std::string theAppDir;
#if defined(_WIN32)
	char theBuf[_MAX_PATH];
	GetModuleFileName(0, theBuf, _MAX_PATH);
	theAppDir.assign(theBuf);
	size_t theSlashPos = theAppDir.find_last_of("\\/");
	if (theSlashPos != std::string::npos)
	{
		theAppDir.resize(theSlashPos);
	}

#elif defined(__linux__)
	char theBuf[PATH_MAX] = { 0 };
	int  rc = readlink("/proc/self/exe", theBuf, sizeof(theBuf) - 1);
	if (rc > 0)
	{
		theAppDir.assign(theBuf);
		size_t lastSlash = theAppDir.find_last_of("/");
		if (lastSlash != std::string::npos)
		{
			theAppDir.resize(lastSlash);
		}
	}

#elif defined(__QNXNTO__)
	char  theBuf[1024] = { 0 };
	FILE* exefile = fopen("/proc/self/exefile", "r");
	if (exefile != NULL)
	{
		fgets(theBuf, sizeof(theBuf), exefile);
		fclose(exefile);
		theAppDir.assign(theBuf);
	}

#endif
	return theAppDir;
}

EngineInstancePrivate::EngineInstancePrivate(EngineCreateStruct& engineCreate)
	: _pAllocator(nullptr)
	, _pRenderInstance(nullptr)
	, _pFrontend(nullptr)
	, _pEngineLog(nullptr)
	, _ApplicationName(engineCreate.applicationName)
	, _ProjectPath(engineCreate.projectPath)
{
	// Create our engine wide allocate interface
	_pAllocator = std::make_shared<AllocatorGlobal>(0);
	if (_pAllocator)
	{
		// get runtime binary path
		_ApplicationPath = GetAppPath();
		// Create our logger. By default no logging
		_pEngineLog = AllocateObject<EngineLog>(*_pAllocator, EngineLog::WARNING_LEVEL_NOENE, EngineLog::MESSAGE_LEVEL_NOENE, true);
	}
}

EngineInstancePrivate::~EngineInstancePrivate()
{
	if (_pEngineLog && _pAllocator)
		DeallocateDelete(*_pAllocator, *_pEngineLog);

	if (_pFrontend)
	{
		DeallocateDelete(*_pAllocator, *_pFrontend);
		_pFrontend = nullptr;
	}

	if (_pRenderInstance)
	{
		DeallocateDelete(*_pAllocator, *_pRenderInstance);
		_pRenderInstance = nullptr;
	}
}

RenderInstance* EngineInstancePrivate::CreateRenderInstance(RenderInstanceTypes type)
{
	if (!_pRenderInstance)
	{
		_pRenderInstance = AllocateObject<RenderInstance>(*_pAllocator, this, type, _ApplicationName.c_str());
	}

	if (!_pRenderInstance && _pEngineLog)
	{
		_pEngineLog->Error("Failed to create renderer");
	}

	return _pRenderInstance;
}

IFrontend* EngineInstancePrivate::CreateFrontend()
{
	if (!_pFrontend)
	{

		_pFrontend = IFrontend::CreateFrontend(_pAllocator);
	}

	if (!_pFrontend && _pEngineLog)
	{
		_pEngineLog->Error("Failed to create frontend");
		throw EngineError("Failed to create frontend");
	}

	return _pFrontend;
}

}
