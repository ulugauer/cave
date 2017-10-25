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

namespace cave
{

EngineInstancePrivate::EngineInstancePrivate(EngineCreateStruct& engineCreate)
	: _pAllocator(nullptr)
	, _pRenderInstance(nullptr)
	, _ApplicationName(engineCreate.applicationName)
{
	// Create our engine wide allocate interface
	_pAllocator = std::make_shared<AllocatorGlobal>(0);
	if (_pAllocator)
	{
		// Create our logger. By default no logging
		_pEngineLog = AllocateObject<EngineLog>(*_pAllocator, EngineLog::WARNING_LEVEL_NOENE, EngineLog::MESSAGE_LEVEL_NOENE, true);
	}
}

EngineInstancePrivate::~EngineInstancePrivate()
{
	if (_pEngineLog && _pAllocator)
		DeallocateDelete(*_pAllocator, *_pEngineLog);

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

}
