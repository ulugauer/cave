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

/// @file engineError.cpp
///       Engine error handling

#include "engineError.h"

#include <cstring>

namespace cave
{

EngineError::EngineError(const std::string& message)
{
	_errorString = new char[message.size()+1];
    std::strcpy(_errorString, message.c_str());
}

EngineError::EngineError(const EngineError& copy)
{
	const size_t stringLength = std::strlen(copy._errorString) + 1;

	_errorString = new char[stringLength];
	std::strcpy(_errorString, copy._errorString);
}

EngineError::~EngineError()
{
	if (_errorString)
	{
		delete [] _errorString;
		_errorString = nullptr;
	}
}

const char* EngineError::what() const throw()
{
    return _errorString;
}

}
