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
#include "Memory/allocatorBase.h"

#include "Vulkan/vk_platform.h"

#include <iostream>		// includes exception handling
#include <memory>

/** \defgroup backend Backend 
*  @{
*		This module handles the hardware abstractions of
*		various graphics API like DirectX, Vulkan
*/

namespace cave
{

///< forward
class HalRenderDevice;

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
* swap chain creation info
*/
typedef struct SwapChainInfo
{
	uint32_t surfaceWidth;	///< Surface width
	uint32_t surfaceHeight;	///< Surface height
	uint32_t colorBits;		///< Amount of total colorbits (24, 32)
	uint32_t depthBits;		///< Amount of total depthbits (24, 32)
	bool fullscreen;	///< Create fullscreen window
	bool offscreen;	///< We do not render to a window
#ifdef VK_USE_PLATFORM_WIN32_KHR
	HINSTANCE hInstance;	///< Filled in at CreateOsWindow call
	HWND hWindow;	///< Filled in at CreateOsWindow call
#else
	xcb_connection_t* connection;		///< xcb connection. Filled in at CreateOsWindow call
	xcb_visualid_t visualId;		///< xcb visual id. Filled in at CreateOsWindow call
	xcb_window_t window;		///< Window id. Filled in at CreateOsWindow call
#endif
}SwapChainInfo;

/**
* Abstraction type of a device instance
*/
class HalInstance
{
public:
	/**
	* @brief Constructor
	*
	* @param[in] type	Instance type
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
	* @brief Create a list of physical devices supported by this instance
	*
	* @return True if a list was successfuly build
	*/
	virtual bool QueryPhysicalDevices() = 0;

	/**
	* @brief Create a hardware render device
	*
	* @param[in] allocator	Engine global allocator
	* @param[in] swapChainInfo	Swap chain creation info
	*
	* @return Pointer to a hardware render device
	*/
	virtual HalRenderDevice* CreateRenderDevice(std::shared_ptr<AllocatorBase> allocator, SwapChainInfo& swapChainInfo) = 0;

	/**
	* @brief Static function to create a hardware instance
	*
	* @param[in] allocator	Engine global allocator
	* @param[in] type Instance type
	* @param applicationName	Name of application (optional)
	*
	* @return Pointer to an API instance
	*/
	static HalInstance* CreateInstance(std::shared_ptr<AllocatorBase> allocator, BackendInstanceTypes type, const char* applicationName);

	/**
	* @brief Static function to reelase a hardware instance
	*
	* @param[in] allocator	Engine global allocator
	* @param[in] instance Pointer to instance
	*
	*/
	static void ReleaseInstance(std::shared_ptr<AllocatorBase> allocator, HalInstance* instance);


private:
	std::shared_ptr<AllocatorBase> _allocator; ///< Global allocator
	BackendInstanceTypes _type;	///< Instance type
};

}

/** @}*/
