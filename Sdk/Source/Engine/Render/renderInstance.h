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

/// @file renderInstance.h
///       Render instance abstraction

#include "halInstance.h"
#include "engineTypes.h"

/** \addtogroup engine
*  @{
*		This module contains all code related to the engine
*/

namespace cave
{

/// forward declaration
class EngineInstancePrivate;

/**
* Abstraction type of a device instance
*/
class CAVE_INTERFACE RenderInstance
{
public:
	/**
	* @brief Constructor
	*
	* @param[in] engine	Pointer to engine instance
	* @param[in] type	Instance type
	* @param[in] applicationName	Name of application (optional)
	*
	*/
	RenderInstance(EngineInstancePrivate* engine, RenderInstanceTypes type, const char* applicationName);
	/** @brief Destructor */
	~RenderInstance();

private:
	EngineInstancePrivate* _pEngineInstance;	///< Pointer to the engine instance we belong to
	HalInstance* _pHalInstance;	///< Pointer to HAL Instance
	
};

}

/** @}*/
