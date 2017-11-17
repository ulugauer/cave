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

/// @file vulkanRenderDevice.h
///       Vulkan render device

#include "halRenderDevice.h"
#include "osPlatformLib.h"

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
class VulkanSwapChain;

/**
* Vulkan render device
*/
class VulkanRenderDevice : public HalRenderDevice
{
public:
	/**
	* @brief Constructor
	*
	* @param[in] instance	Pointer to instance object
	* @param[in] physicalDevice	Pointer to physical device
	* @param[in] surface	Handle to presentation surface [optional]
	*
	*/
	VulkanRenderDevice(VulkanInstance* instance, VulkanPhysicalDevice* physicalDevice, VkSurfaceKHR surface);

	/** @brief Destructor */
	virtual ~VulkanRenderDevice();

	/**
	* @brief Get vulkan logical device handle
	*
	* @return Lowlevel vulkan handle
	*/
	VkDevice GetDeviceHandle() { return _vkDevice; }

	/**
	* @brief Get vulkan surface handle
	*
	* @return Lowlevel vulkan handle
	*/
	VkSurfaceKHR GetPresentationSurfaceHandle() { return _presentationSurface; }

	/**
	* @brief Create a vulkan swap chain
	*
	* @param[in] swapChainInfo	Swap chain creation info
	*
	*/
	void CreateSwapChain(SwapChainInfo& swapChainInfo) override;

	/**
	* @brief Create a low level shader
	*
	* @param[in] type		Shader type
	* @param[in] language	Shader language
	*
	* @return shader abstraction interface
	*/
	HalShader* CreateShader(ShaderType type, ShaderLanguage language) override;

private:
	VulkanInstance* _pInstance;	///< Pointer to instance object
	VulkanPhysicalDevice* _pPhysicalDevice;	///< Pointer to physical device
	VkSurfaceKHR _presentationSurface; ///< Handle to presentation surface
	VkDevice _vkDevice;	///< Handle to vulkan device
	VkQueue _presentQueue;	///< Handle to vulkan queue used for presentations
	VulkanSwapChain* _pSwapChain;	///< Handle to a swap chain
};

}

/** @}*/
