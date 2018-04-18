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

/// @file caveSanityTestBase.h
///       Base class for all low level sanity tests

#include "Render/renderDevice.h"
#include "Render/renderVertexInput.h"
#include "Render/renderVertexBuffer.h"
#include "Render/renderIndexBuffer.h"
#include "Render/renderUniformBuffer.h"
#include "Render/renderCommandPool.h"
#include "Render/renderLayerSection.h"
#include "Render/renderGraphicsPipeline.h"
#include "Render/renderCommandBuffer.h"
#include "Render/renderCommandPool.h"
#include "Render/renderDescriptorSet.h"

#include <iostream>		// includes exception handling
#include <memory>

/**
* Backend exception handling
*/
class CaveSanityTestException : public std::exception
{
public:
	/**
	* @brief Constructor
	*
	* @param[in] message Message string
	*
	*/
	explicit CaveSanityTestException(const std::string& message)
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
* @brief User data passed on to the test
*/
typedef struct SUSER_CONTEXT_DATA 
{
	uint32_t winWidth;		///< Window width
	uint32_t winHeight;		///< Window height
} userContextData;

/**
* @brief Our base class for all low level sanity tests
*/
class CaveSanityTestBase
{
public:
	/** constructor */
	CaveSanityTestBase() { };
	/** destructor */
	virtual ~CaveSanityTestBase() { };

	/** 
	* @brief Checks if this test is supported
	*
	* @param device[in]		Pointer to a RenderDevice
	*
	* @return true if supported
	*/
	virtual bool IsSupported(cave::RenderDevice *device) = 0;


	/**
	* @brief Checks if this test supportes image compare for success
	* Not all tests may support this. Overwrite if not.
	*
	* @param device[in]		Pointer to a RenderDevice
	*
	* @return true if supported
	*/
	virtual bool IsImageCompareSupported(cave::RenderDevice*) { return true; }

	/**
	* @brief This runs the test
	*
	* @param device[in]			Pointer to a RenderDevice
	* @param commandPool[in]	Pointer to a command pool object
	* @param renderPass[in]		Pointer to a RenderPass object
	* @param pUserData[in]		Pointer to pUserData
	*
	* @return false if failed
	*/
	virtual bool Run(cave::RenderDevice *device, cave::RenderCommandPool* commandPool, cave::RenderPass* renderPass, userContextData* pUserData) = 0;

	/**
	* @brief This cleans up state after the test if needed
	*
	* @param device[in]		Pointer to a RenderDevice
	* @param pUserData[in]	Pointer to pUserData
	*
	* @return false if failed
	*/
	virtual void Cleanup(cave::RenderDevice *device, userContextData* pUserData) = 0;

	/**
	* @brief This runs the performance test
	*
	* @param device[in]		Pointer to a RenderDevice
	* @param pUserData[in]	Pointer to pUserData
	*
	* @return false if failed
	*/
	virtual bool RunPerformance(cave::RenderDevice *device, userContextData* pContextData) = 0;

};

