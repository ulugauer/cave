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

#include <vector>

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
	*
	*/
	VulkanSwapChain(VulkanInstance* instance, VulkanPhysicalDevice* physicalDevice
					, VulkanRenderDevice* renderDevice);

	/** @brief Destructor */
	virtual ~VulkanSwapChain();

	/**
	* @brief Get swap image count
	*
	* @return swap image count
	*/
	const uint32_t GetSwapChainImageCount() const { return _swapImageCount; }

	/**
	* @brief Get swap image size
	*
	* @return 2D extension rect
	*/
	const VkExtent2D GetSwapChainImageExtend() const { return _swapChainExtent; }

private:
	/**
	* @brief Create a swap chain for the device
	*
	*/
	void CreateSwapChain();

	/**
	* @brief Create swap chain image views
	*
	*/
	void CreateImageViews();

	/**
	* @brief Create swap presentsation semaphores
	*
	*/
	void CreatePresentationSemaphores();

	/**
	* @brief Get swap image count supported by the device
	*
	* @param[in] surfaceCapabilities	Surface capabilites
	*
	* @return supported swap chain image counts
	*/
	uint32_t GetSwapChainNumImages(VkSurfaceCapabilitiesKHR &surfaceCapabilities);

	/**
	* @brief Search surface format which matches the surface capabilites
	*
	* @param[in] surfaceFormats	Surface formats queried
	*
	* @return matching surface format
	*/
	VkSurfaceFormatKHR GetSwapChainFormat(std::vector<VkSurfaceFormatKHR> &surfaceFormats);

	/**
	* @brief Determine image extend.
	*
	* @param[in] surfaceCapabilities	Surface capabilites
	*
	* @return Matching iamge extends. Maybe clipped to max extend supported
	*/
	VkExtent2D GetSwapChainExtent(VkSurfaceCapabilitiesKHR &surfaceCapabilities);

	/**
	* @brief Determine image usage flag.
	*		 For the presentation image we need color attachment at minmum
	*
	* @param[in] surfaceCapabilities	Surface capabilites
	*
	* @return Usage flags
	*/
	VkImageUsageFlags GetSwapChainUsageFlags(VkSurfaceCapabilitiesKHR &surfaceCapabilities);

	/**
	* @brief Determine pre swap transform
	*		 Can be used to transform to portrait mode
	*
	* @param[in] surfaceCapabilities	Surface capabilites
	*
	* @return Transform bit
	*/
	VkSurfaceTransformFlagBitsKHR GetSwapChainTransform(VkSurfaceCapabilitiesKHR &surfaceCapabilities);

	/**
	* @brief Choose an available present mode
	*
	* @param[in] presentModes	Present modes queried
	*
	* @return matching present mode
	*/
	VkPresentModeKHR GetSwapChainPresentMode(std::vector<VkPresentModeKHR> &presentModes);

private:
	VulkanInstance* _pInstance;	///< Pointer to instance
	VulkanPhysicalDevice* _pPhysicalDevice;	///< Pointer to physical device
	VulkanRenderDevice* _pRenderDevice;	///< Handle to render device
	VkSwapchainKHR _swapChain;	///< Handle to a vulkan swap chain
	uint32_t _swapImageCount;	///< Image count
	VkImage* _swapChainImageArray; ///< Array of generated swap images
	VkImageView* _swapChainImageViewArray; ///< Array of generated swap images views
	VkFormat _swapChainImageFormat;	///< The chosen image format
	VkExtent2D _swapChainExtent;	///< The current extend
	VkSemaphore _ImageAvailableSemaphore; ///< Next image available semaphore
	VkSemaphore _RenderingFinishedSemaphore; ///< Rendering done semaphore
	
};

}

/** @}*/
