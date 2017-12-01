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

/// @file renderDevice.h
///       Render device interface

#include "Resource/resourceManager.h"
#include "halRenderDevice.h"
#include "engineTypes.h"
#include "frontend.h"

/** @addtogroup engine
*  @{
*		This module contains all code related to the engine
*/

namespace cave
{

/// forward declaration
class HalInstance;
class HalRenderDevice;
class RenderInstance;
class RenderVertexInput;
class EngineLog;

/**
* Abstraction type of a device instance
*/
class CAVE_INTERFACE RenderDevice
{
public:
	/**
	* @brief Constructor
	*
	* @param[in] renderInstance	Pointer to render instance
	* @param[in] halInstance	Pointer to HAL render instance
	* @param[in] windowInfo	Window creation info
	*
	*/
	RenderDevice(RenderInstance* renderInstance, HalInstance* halInstance, FrontendWindowInfo& windowInfo);
	/** @brief Destructor */
	~RenderDevice();

	/**
	* @brief GetAllocator
	*
	* @return Pointer Engine allocator
	*/
	std::shared_ptr<AllocatorGlobal> GetEngineAllocator();

	/**
	* @brief GetEngineLog
	*
	* @return Pointer to engine logging
	*/
	EngineLog* GetEngineLog() const;

	/**
	* @brief Create a swap chain
	*
	*/
	void CreateSwapChain();

	/**
	* @brief Get Resource manager
	*
	* @return ResourceManager Object
	*/
	ResourceManager& GetResourceManager()
	{
		return *_pResourceManager;
	}

	/**
	* @brief Get pointer to HAL render device
	*
	* @return Low level HAL device object
	*/
	HalRenderDevice* GetHalRenderDevice()
	{
		return _pHalRenderDevice;
	}

	/**
	* @brief Get pointer to HAL render device
	*
	* @return Vertex input object
	*/
	RenderVertexInput* CreateVertexInput();

	/**
	* @brief Release a vertex input object
	*
	* @param[in] vertexInput	Vertex input object to release
	*/
	void ReleaseVertexInput(RenderVertexInput* vertexInput);

private:
	RenderInstance* _pRenderInstance;	///< Pointer to the render instance we belong to
	HalInstance* _pHalInstance;	///< Pointer to HAL Instance
	HalRenderDevice* _pHalRenderDevice;	///< Pointer to HAL render device
	ResourceManager* _pResourceManager;	///< Our device resource manager
	SwapChainInfo _swapChainInfo;	///< swap chain info
};

}

/** @}*/
