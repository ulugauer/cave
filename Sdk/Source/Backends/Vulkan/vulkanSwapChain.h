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

/// @file vulkanSwapChain.h
///       Vulkan swapchain

#include "halRenderDevice.h"

#include "vulkan.h"

/** \addtogroup backend 
*  @{
*		
*/

namespace cave
{

///< forwards
class VulkanInstance;
class VulkanPhysicalDevice;
class VulkanRenderDevice;

/**
* Vulkan swap chain handling
*/
class VulkanSwapChain
{
public:
	/**
	* @brief Constructor
	*
	* @param[in] instance	Pointer to instance
	* @param[in] physicalDevice	Pointer to physical device
	* @param[in] renderDevice	Pointer to render device
	* @param[in] swapChainInfo	Swap chain creation info
	*
	*/
	VulkanSwapChain(VulkanInstance* instance, VulkanPhysicalDevice* physicalDevice
					, VulkanRenderDevice* renderDevice, SwapChainInfo& swapChainInfo);

	/** @brief Destructor */
	virtual ~VulkanSwapChain();

private:
	VulkanInstance* _pInstance;	///< Pointer to instance
	VulkanPhysicalDevice* _pPhysicalDevice;	///< Pointer to physical device
	VulkanRenderDevice* _pRenderDevice;	///< Handle to render device
	VkSurfaceKHR _presentationSurface;	///< Handle to presentation surface
};

}

/** @}*/
