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
#include "Common/caveVector.h"
#include "vulkanMemoryManager.h"

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
	* @param[in] swapChainInfo	Pointer SwapChainInfo struct
	*
	*/
	VulkanSwapChain(VulkanInstance* instance, VulkanPhysicalDevice* physicalDevice
					, VulkanRenderDevice* renderDevice, SwapChainInfo& swapChainInfo);

	/** @brief Destructor */
	virtual ~VulkanSwapChain();

	/**
	* @brief Get swap image count
	*
	* @return swap image count
	*/
	const uint32_t GetSwapChainImageCount() const { return static_cast<uint32_t>(_swapChainImageVector.Size()); }

	/**
	* @brief Get swap image size
	*
	* @return 2D extension rect
	*/
	const VkExtent2D GetSwapChainImageExtend() const { return _swapChainExtent; }

	/**
	* @brief Get swap image format
	*
	* @return Image format
	*/
	const VkFormat GetSwapChainImageFormat() const { return _swapChainImageFormat; }

	/**
	* @brief Get swap image depth format
	*
	* @return Image format
	*/
	const VkFormat GetSwapChainDepthImageFormat() const { return _swapChainDepthImageFormat; }

	/**
	* @brief Get swap image view handle
	*
	* @param[in] index	Index into image view array
	*
	* @return Image view handle or null handle
	*/
	const VkImageView GetSwapChainImageView(size_t index) const;

	/**
	* @brief Get swap depth image view handle
	*
	* @return Image view handle or null handle
	*/
	const VkImageView GetSwapChainDepthImageView() const;

	/**
	* @brief Get next available swap chain image
	*
	* @param[in] timeout		Timeout in nano seconds
	*
	* @return Image index
	*/
	const uint32_t AcquireNextSwapChainImage(uint64_t timeout);

	/**
	* @brief Get semaphore used for image availble signaling
	*
	* @return Semaphore object
	*/
	const VkSemaphore GetImageAvailableSemaphore() { return _ImageAvailableSemaphore; }

	/**
	* @brief Get semaphore used for rendering done signaling
	*
	* @return Semaphore object
	*/
	const VkSemaphore GetRenderingDoneSemaphore() { return _RenderingFinishedSemaphore; }

	/**
	* @brief Get semaphore used for rendering done signaling
	*
	* @return Semaphore object
	*/
	const VkSwapchainKHR GetSwapChainHandle() { return _swapChain; }

	/**
	* @brief Read pixels from the last used swap chain image
	*		 The returned pixels are always in RGBA format.
	*		 data must point to a memory which has sufficient space.
	*		 WIDTH * HEIGHT * 4 bytes
	*
	* @param[in] commandPool	Command pool used to allocate command buffer
	* @param[out] data			Pixel values are returned here
	*
	*/
	void ReadPixels(VkCommandPool commandPool, void* data);

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
	* @brief Create depth buffer resource
	* 
	* This functions allocates everything need for using depth buffering.
	* E.g a vkImage, vkImageView, memory
	*
	* @param[in] depthBits		Requested depth bits
	* @param[in] stencilBits	Requested stencil bits
	*/
	void CreateDepthResource(uint32_t depthBits, uint32_t StencilBits);

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
	caveVector<VkImage> _swapChainImageVector; ///< Vector of swap chain images
	caveVector<VkImageView> _swapChainImageViewVector; ///< Array of generated swap images views
	uint32_t _imageIndex;	///< Current used image index
	VkFormat _swapChainImageFormat;	///< The chosen image format
	VkFormat _swapChainDepthImageFormat;	///< The chosen depth image format
	VkImage _swapChainDepthImage;	/// swap chain depth image
	VkDeviceMemory _swapChainDepthImageMemory;	///< swap chain depth image memory
	VkImageView _swapChainDepthImageView;	/// swap chain depth image view
	VkExtent2D _swapChainExtent;	///< The current extend
	VkSemaphore _ImageAvailableSemaphore; ///< Next image available semaphore
	VkSemaphore _RenderingFinishedSemaphore; ///< Rendering done semaphore
	
};

}

/** @}*/
