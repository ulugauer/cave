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

/// @file engineLog.cpp
///       Engine message logging handling

#include "engineLog.h"

#include <stdio.h>
#include <time.h>
#include <cstdarg>
#include <cstring>

namespace cave
{

EngineLog::EngineLog()
{
    // show all warnings and messages by default
    _minWarningLevel = WARNING_LEVEL0;
    _minMessageLevel = MESSAGE_LEVEL0;
    
    _logFile = nullptr;
}

EngineLog::~EngineLog()
{
    if(_logFile)
        fclose((FILE*)_logFile);
}

bool EngineLog::Open()
{
	if (_logFile == nullptr)
		_logFile = fopen("caveengine.log", "a");

	return (_logFile != nullptr);
}

void EngineLog::PrintToLog(const char *string)
{
	if (Open())
		fprintf((FILE*)_logFile, "%s", string);
}

void EngineLog::Timestamp()
{
	struct tm *newTime;
	time_t systemTime;
	char buffer[26];

	// print the time stamp
	if (time(&systemTime) != -1)
	{
		// Convert to local time.
		newTime = localtime(&systemTime);
		if (newTime)
		{
			std::strcpy(buffer, asctime(newTime));

			// asctime adds a newline, remove this
			buffer[strlen(buffer) - 1] = 0;

			// and print the timestamp
			PrintToLog(buffer);
			return;
		}
	}

	PrintToLog("--:--:--");
}

void EngineLog::Message(logMessageLevel level, const char *string, ...)
{
	//osMutexLocker locker(_globalLock.getMutex());
	va_list marker;

	// ignore this message if it's below the minimum level
	if (level < _minMessageLevel)
		return;

	Timestamp();

	PrintToLog(": ");

	va_start(marker, string);
	vfprintf((FILE*)_logFile, string, marker);
	va_end(marker);

	PrintToLog("\n");
}

void EngineLog::Warning(logWarningLevel level, const char *string, ...)
{
	//osMutexLocker locker(_globalLock.getMutex());
	va_list marker;

	// ignore this warning if it's below the minimum level
	if ((level < _minWarningLevel))
		return;

	if (!Open())
		return;

	fprintf((FILE*)_logFile, "Warning (%c) (", '0' + level);

	Timestamp();

	PrintToLog(": warning ");

	va_start(marker, string);
	vfprintf((FILE*)_logFile, string, marker);
	va_end(marker);

	PrintToLog("\n");
}

void EngineLog::Error(const char *string, ...)
{
	//osMutexLocker locker(_globalLock.getMutex());
	va_list marker;
	const int stringLength = 256;
	char errorString[stringLength];

	// generate the final string
	va_start(marker, string);

	vsnprintf(errorString, stringLength - 1, string, marker);
	va_end(marker);

	//if (_globalConfig._logging._enable)
	{
		Timestamp();

		fprintf((FILE*)_logFile, ": error %s\n", errorString);
	}
}


}
