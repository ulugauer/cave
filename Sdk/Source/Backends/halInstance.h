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

/// @file halInstance.h
///       Hardware instance abstraction

#include "engineDefines.h"

#include <iostream>		// includes exception handling

/** \defgroup backend Backend 
*  @{
*		This module handles the hardware abstractions of
*		various graphics API like DirectX, Vulkan
*/

namespace cave
{

/**
* Backend exception handling
*/
class BackendException : public std::exception
{
public:
	/**
	* @brief Constructor
	*
	* @param[in] message Message string
	*
	*/
	explicit BackendException(const std::string& message)
		: _message(message)
	{

	}

	/**
	* @brief return message
	*
	* @return error message string
	*/
	virtual const char* what() const throw()
	{
		return _message.c_str();
	}
private:
	std::string _message;	///< stores error string
};

/**
* Hardware instance types
*/
enum class BackendInstanceTypes
{
	InstanceVulkan = 1,	///< Vulkan instance
	InstanceDX12 = 2,	///< DX12 instance
};

/**
* Abstraction type of a device instance
*/
class CAVE_INTERFACE HalInstance
{
public:
	/**
	* @brief Constructor
	*
	* @param[in] type Instance type
	*
	*/
	HalInstance(BackendInstanceTypes type);
	/** @brief Destructor */
	virtual ~HalInstance();

	/**
	* @brief Get hardware instance type
	*
	* @return Hardware instance type
	*
	*/
	const BackendInstanceTypes GetInstanceType() const { return _type; }

	/**
	* @brief Static function to create a hardware instance
	*
	* @param[in] type Instance type
	* @param applicationName	Name of application (optional)
	*
	*/
	static HalInstance* CreateInstance(BackendInstanceTypes type, const char* applicationName);

	/**
	* @brief Static function to reelase a hardware instance
	*
	* @param[in] instance Pointer to instance
	*
	*/
	static void ReleaseInstance(HalInstance* instance);

private:

	BackendInstanceTypes _type;	///< Instance type
};

}

/** @}*/
