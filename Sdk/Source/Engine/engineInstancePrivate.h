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

/// @file engineInstancePrivate.h
///       Engine runtime

#include "Render/renderInstance.h"
#include "Render/renderDevice.h"
#include "Resource/resourceManager.h"
#include "Memory/allocatorGlobal.h"
#include "frontend.h"
#include "engineTypes.h"
#include "engineLog.h"

#include <string>
#include <memory>

/** \addtogroup engine
*  @{
*		This module contains all code related to the engine
*/

namespace cave
{

/**
* Engine creation info
*/
struct EngineCreateStruct
{
	uint32_t 		flags; 				///< currently unused
	const char*		applicationName; 	///< application name
	const char*		projectPath; 		///< project path
};

/**
* Our main engine runtime interface
*/
class EngineInstancePrivate
{
	friend class EngineInstance;	///< we grant access to our public interface
public:
	/**
	* @brief Constructor
	*
	* @param[in] engineCreate Engine instance create info
	*
	*/
	EngineInstancePrivate(EngineCreateStruct& engineCreate);
	/** @brief Destructor */
	~EngineInstancePrivate();

	/**
	* @brief Create a render instance
	*
	* @param[in] type Graphics instance type
	*
	* @return RenderInstance object or nullptr
	*/
	RenderInstance* CreateRenderInstance(RenderInstanceTypes type);

	/**
	* @brief Create os frontend interface
	*
	* @return IFrontend interface
	*/
	IFrontend* CreateFrontend();

	/**
	* @brief GetEngineLog
	*
	* @return Pointer to engine logging
	*/
	EngineLog* GetEngineLog() const { return _pEngineLog; }

	/**
	* @brief GetAllocator
	*
	* @return Pointer Engine allocator
	*/
	std::shared_ptr<AllocatorGlobal> GetEngineAllocator() { return _pAllocator; }

	
	/**
	* @brief Get Ppoject path
	*
	* @return path string
	*/
	const char* GetProjectPath() const { return _ProjectPath.c_str(); }

	/**
	* @brief Get application path
	*
	* @return path string
	*/
	const char* GetApplicationPath() const { return _ApplicationPath.c_str(); }

private:
	std::shared_ptr<AllocatorGlobal>    _pAllocator;	///< Pointer to engine custom allocations
	RenderInstance* _pRenderInstance;	///< Pointer to render instance
	IFrontend* _pFrontend;	///< Interface pointer to widow frontend
	EngineLog* _pEngineLog;	///< Our engine wide message logger
	std::string _ApplicationName;	///< Optional specified at creation time
	std::string _ProjectPath;	///< Path to project provided by the caller
	std::string _ApplicationPath;	///< Path to runtime binary
};

}

/** @}*/

