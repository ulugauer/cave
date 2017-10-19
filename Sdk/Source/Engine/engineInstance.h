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

/// @file engineInstance.h
///       Engine runtime

#include "engineInstancePrivate.h"
#include "engineTypes.h"

#include <string>

/** \addtogroup engine
*  @{
*		This module contains all code related to the engine
*/

namespace cave
{


/**
* Our main engine runtime interface
*/
class CAVE_INTERFACE EngineInstance
{
public:
	/**
	* @brief Constructor
	*
	* @param[in] engineCreate Engine instance create info
	*
	*/
	EngineInstance(EngineCreateStruct& engineCreate);
	/** @brief Destructor */
	~EngineInstance();

	/**
	* @brief Create Renderer
	*
	* @param[in] type Graphics instance type
	*
	* @return RenderInstance object or nullptr
	*/
	RenderInstance* CreateRenderer(RenderInstanceTypes type)
	{
		return _pEnginePrivate->CreateRenderer(type);
	}

private:
	EngineInstancePrivate* _pEnginePrivate;		///< Pointer to not exported engine instance

};

}

/** @}*/
