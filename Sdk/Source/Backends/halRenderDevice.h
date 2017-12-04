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

/// @file halRenderDevice.h
///       Hardware render device abstraction

#include "engineDefines.h"
#include "halInstance.h"
#include "halShader.h"
#include "halVertexInput.h"
#include "halInputAssembly.h"
#include "halViewportAndScissor.h"
#include "Memory/allocatorBase.h"

#include <iostream>		// includes exception handling
#include <memory>

/** \addtogroup backend
*  @{
*
*/

namespace cave
{


/**
* Abstraction type of a device instance
*/
class HalRenderDevice
{
public:
	/**
	* @brief Constructor
	*
	*/
	HalRenderDevice(HalInstance* instance);

	/** @brief Destructor */
	virtual ~HalRenderDevice();

	/**
	* @brief GetAllocator
	*
	* @return Pointer Engine allocator
	*/
	std::shared_ptr<AllocatorBase> GetEngineAllocator() {
		return _pInstance->GetEngineAllocator();
	}

	/**
	* @brief Create a swap chain
	*
	* @param[in] swapChainInfo	Swap chain creation info
	*
	*/
	virtual void CreateSwapChain(SwapChainInfo& swapChainInfo) = 0;

	/**
	* @brief Create a low level shader
	*
	* @param[in] type		Shader type
	* @param[in] language	Shader language
	*
	* @return shader abstraction interface
	*/
	virtual HalShader* CreateShader(ShaderType type, ShaderLanguage language) = 0;

	/**
	* @brief Create a low level vertex input state
	*
	* @return Vertex input abstraction interface
	*/
	virtual HalVertexInput* CreateVertexInput() = 0;

	/**
	* @brief Create a low level input assembly state
	*
	* @return Input assembly abstraction interface
	*/
	virtual HalInputAssembly* CreateInputAssembly() = 0;

	/**
	* @brief Create a low level viewport, scissor state
	*
	* @param[in] viewport	Viewport setup
	* @param[in] scissor	Scissor setup
	*
	* @return  Viewport, scissor abstraction interface
	*/
	virtual HalViewportAndScissor* CreateViewportAndScissor(HalViewport& viewport, HalScissor& scissor) = 0;

private:
	HalInstance* _pInstance;	///< Pointer to instance object
};

}

/** @}*/
