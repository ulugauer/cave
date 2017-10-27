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

/// @file vulkanSwapChain.cpp
///       Vulkan swapchain

#include "vulkanSwapChain.h"
#include "vulkanInstance.h"
#include "vulkanRenderDevice.h"
#include "vulkanPhysicalDevice.h"
#include "vulkanApi.h"

#include <limits>

namespace cave
{

VulkanSwapChain::VulkanSwapChain(VulkanInstance* instance, VulkanPhysicalDevice* physicalDevice
								, VulkanRenderDevice* renderDevice)
	: _pInstance(instance)
	, _pPhysicalDevice(physicalDevice)
	, _pRenderDevice(renderDevice)
	, _swapChain(nullptr)
	, _swapImageCount(0)
	, _swapChainImageArray(nullptr)
	, _swapChainImageViewArray(nullptr)
{
	CreateSwapChain();
	CreateImageViews();
}

VulkanSwapChain::~VulkanSwapChain()
{
	VulkanApi::GetApi()->vkDeviceWaitIdle(_pRenderDevice->GetDeviceHandle());

	if (_swapChainImageViewArray)
	{
		for (size_t i = 0; i < _swapImageCount; i++)
			VulkanApi::GetApi()->vkDestroyImageView(_pRenderDevice->GetDeviceHandle(), _swapChainImageViewArray[i], nullptr);

		DeallocateArray<VkImageView>(*_pInstance->GetEngineAllocator(), _swapChainImageViewArray);
	}

	if (_swapChainImageArray)
		DeallocateArray<VkImage>(*_pInstance->GetEngineAllocator(), _swapChainImageArray);

	if (_swapChain)
		VulkanApi::GetApi()->vkDestroySwapchainKHR(_pRenderDevice->GetDeviceHandle(), _swapChain, nullptr);
}

void VulkanSwapChain::CreateSwapChain()
{
	VkSurfaceKHR presentationSurface = _pRenderDevice->GetPresentationSurfaceHandle();
	VkResult success = VK_SUCCESS;
	VkSurfaceCapabilitiesKHR surfaceCapabilities;
	success = VulkanApi::GetApi()->vkGetPhysicalDeviceSurfaceCapabilitiesKHR(_pPhysicalDevice->GetPhysicalDeviceHandle()
							, presentationSurface, &surfaceCapabilities);

	uint32_t minImageCount = GetSwapChainNumImages(surfaceCapabilities);
	if (minImageCount == 0)
		throw BackendException("Error no swap chain images supported");

	// surface formats
	uint32_t surfaceFormatCount = 0;
	VulkanApi::GetApi()->vkGetPhysicalDeviceSurfaceFormatsKHR(_pPhysicalDevice->GetPhysicalDeviceHandle(), presentationSurface, &surfaceFormatCount, nullptr);
	if (surfaceFormatCount == 0)
		throw BackendException("Error no swap chain images format found");

	std::vector<VkSurfaceFormatKHR> surfaceFormats(surfaceFormatCount);
	VulkanApi::GetApi()->vkGetPhysicalDeviceSurfaceFormatsKHR(_pPhysicalDevice->GetPhysicalDeviceHandle(), presentationSurface, &surfaceFormatCount, &surfaceFormats[0]);
	VkSurfaceFormatKHR surfaceFormat = GetSwapChainFormat(surfaceFormats);

	// present modes
	uint32_t presentModesCount;
	VulkanApi::GetApi()->vkGetPhysicalDeviceSurfacePresentModesKHR(_pPhysicalDevice->GetPhysicalDeviceHandle(), presentationSurface, &presentModesCount, nullptr);
	if (presentModesCount == 0)
		throw BackendException("Error no present mode found");

	std::vector<VkPresentModeKHR> presentModes(presentModesCount);
	VulkanApi::GetApi()->vkGetPhysicalDeviceSurfacePresentModesKHR(_pPhysicalDevice->GetPhysicalDeviceHandle(), presentationSurface, &presentModesCount, &presentModes[0]);
	VkPresentModeKHR presentMode = GetSwapChainPresentMode(presentModes);

	VkSwapchainCreateInfoKHR swapChainCreateInfo = {};
	swapChainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	swapChainCreateInfo.pNext = nullptr;
	swapChainCreateInfo.flags = 0;
	swapChainCreateInfo.surface = presentationSurface;
	swapChainCreateInfo.minImageCount = minImageCount;
	swapChainCreateInfo.imageFormat = surfaceFormat.format;
	swapChainCreateInfo.imageColorSpace = surfaceFormat.colorSpace;
	swapChainCreateInfo.imageExtent = GetSwapChainExtent(surfaceCapabilities);
	swapChainCreateInfo.imageArrayLayers = 1;
	swapChainCreateInfo.imageUsage = GetSwapChainUsageFlags(surfaceCapabilities);
	swapChainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
	swapChainCreateInfo.queueFamilyIndexCount = 0;
	swapChainCreateInfo.pQueueFamilyIndices = nullptr;
	swapChainCreateInfo.preTransform = GetSwapChainTransform(surfaceCapabilities);
	swapChainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	swapChainCreateInfo.presentMode = presentMode;
	swapChainCreateInfo.clipped = VK_TRUE;
	swapChainCreateInfo.oldSwapchain = _swapChain;

	VkResult result = VulkanApi::GetApi()->vkCreateSwapchainKHR(_pRenderDevice->GetDeviceHandle(), &swapChainCreateInfo, nullptr, &_swapChain);
	if (result != VK_SUCCESS)
		throw BackendException("Failed to create swap chain");

	// remove previous swap chain
	if (swapChainCreateInfo.oldSwapchain)
		VulkanApi::GetApi()->vkDestroySwapchainKHR(_pRenderDevice->GetDeviceHandle(), swapChainCreateInfo.oldSwapchain, nullptr);

	// retrieve images
	_swapImageCount = 0;
	VulkanApi::GetApi()->vkGetSwapchainImagesKHR(_pRenderDevice->GetDeviceHandle(), _swapChain, &_swapImageCount, nullptr);
	if (!_swapImageCount)
		throw BackendException("Failed to get swap chain images");

	_swapChainImageArray = AllocateArray<VkImage>(*_pInstance->GetEngineAllocator(), _swapImageCount);
	if (!_swapChainImageArray)
		throw BackendException("Failed to allocate memory for swap chain images");

	VulkanApi::GetApi()->vkGetSwapchainImagesKHR(_pRenderDevice->GetDeviceHandle(), _swapChain, &_swapImageCount, _swapChainImageArray);

	// save for later usage
	_swapChainImageFormat = surfaceFormat.format;
	_swapChainExtent = swapChainCreateInfo.imageExtent;
}

void VulkanSwapChain::CreateImageViews()
{
	_swapChainImageViewArray = AllocateArray<VkImageView>(*_pInstance->GetEngineAllocator(), _swapImageCount);
	if (!_swapChainImageViewArray)
		throw BackendException("Failed to allocate memory for swap chain image views");

	for (uint32_t i = 0; i < _swapImageCount; ++i)
	{
		VkImageViewCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		createInfo.image = _swapChainImageArray[i];
		createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		createInfo.format = _swapChainImageFormat;
		createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		createInfo.subresourceRange.baseMipLevel = 0;
		createInfo.subresourceRange.levelCount = 1;
		createInfo.subresourceRange.baseArrayLayer = 0;
		createInfo.subresourceRange.layerCount = 1;

		if (VulkanApi::GetApi()->vkCreateImageView(_pRenderDevice->GetDeviceHandle(), &createInfo, nullptr, &_swapChainImageViewArray[i]) != VK_SUCCESS)
		{
			throw BackendException("Failed to create image views!");
		}
	}
}

uint32_t VulkanSwapChain::GetSwapChainNumImages(VkSurfaceCapabilitiesKHR &surfaceCapabilities)
{
	// Set of images defined in a swap chain may not always be available for application to render to:
	// One may be displayed and one may wait in a queue to be presented
	// If application wants to use more images at the same time it must ask for more images
	uint32_t imageCount = surfaceCapabilities.minImageCount + 1;
	if ((surfaceCapabilities.maxImageCount > 0) &&
		(imageCount > surfaceCapabilities.maxImageCount))
	{
		imageCount = surfaceCapabilities.maxImageCount;
	}
	return imageCount;
}

VkSurfaceFormatKHR VulkanSwapChain::GetSwapChainFormat(std::vector<VkSurfaceFormatKHR> &surfaceFormats)
{
	// If the list contains only one entry with undefined format
	// it means that there are no preferred surface formats and any can be chosen
	if ((surfaceFormats.size() == 1) &&
		(surfaceFormats[0].format == VK_FORMAT_UNDEFINED))
	{
		VkSurfaceFormatKHR format = { VK_FORMAT_R8G8B8A8_UNORM, VK_COLORSPACE_SRGB_NONLINEAR_KHR };
		return format;
	}

	// Check if list contains most widely used R8 G8 B8 A8 format
	// with nonlinear color space
	std::vector<VkSurfaceFormatKHR>::iterator iter = surfaceFormats.begin();
	for (; iter != surfaceFormats.end(); iter++)
	{
		if (iter->format == VK_FORMAT_R8G8B8A8_UNORM || 
			iter->format == VK_FORMAT_B8G8R8A8_UNORM)
		{
			return *iter;
		}
	}

	// Return the first format from the list
	return surfaceFormats[0];

}

VkExtent2D VulkanSwapChain::GetSwapChainExtent(VkSurfaceCapabilitiesKHR &surfaceCapabilities)
{
	// Special value of surface extent is width == height == -1
	// If this is so we define the size by ourselves but it must fit within defined confines
	if (surfaceCapabilities.currentExtent.width == -1)
	{
		VkExtent2D swapChainExtend = { 640, 480 };
		if (swapChainExtend.width < surfaceCapabilities.minImageExtent.width)
		{
			swapChainExtend.width = surfaceCapabilities.minImageExtent.width;
		}
		if (swapChainExtend.height < surfaceCapabilities.minImageExtent.height)
		{
			swapChainExtend.height = surfaceCapabilities.minImageExtent.height;
		}
		if (swapChainExtend.width > surfaceCapabilities.maxImageExtent.width)
		{
			swapChainExtend.width = surfaceCapabilities.maxImageExtent.width;
		}
		if (swapChainExtend.height > surfaceCapabilities.maxImageExtent.height)
		{
			swapChainExtend.height = surfaceCapabilities.maxImageExtent.height;
		}
		return swapChainExtend;

	}

	// Most of the cases we define size of the swap_chain images equal to current window's size
	return surfaceCapabilities.currentExtent;
}

VkImageUsageFlags VulkanSwapChain::GetSwapChainUsageFlags(VkSurfaceCapabilitiesKHR &surfaceCapabilities)
{
	// Color attachment flag must always be supported
	// We can define other usage flags but we always need to check if they are supported
	VkImageUsageFlags usageFlags = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT; // always supported
	if (surfaceCapabilities.supportedUsageFlags & VK_IMAGE_USAGE_TRANSFER_DST_BIT)
	{
		usageFlags |= VK_IMAGE_USAGE_TRANSFER_DST_BIT;
	}

	return usageFlags;
}

VkSurfaceTransformFlagBitsKHR VulkanSwapChain::GetSwapChainTransform(VkSurfaceCapabilitiesKHR &surfaceCapabilities)
{
	// Sometimes images must be transformed before they are presented (i.e. due to device's orienation
	// being other than default orientation)
	// If the specified transform is other than current transform, presentation engine will transform image
	// during presentation operation; this operation may hit performance on some platforms
	// Here we don't want any transformations to occur so if the identity transform is supported use it
	// otherwise just use the same transform as current transform
	if (surfaceCapabilities.supportedTransforms & VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR)
	{
		return VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
	}
	else
	{
		return surfaceCapabilities.currentTransform;
	}
}

VkPresentModeKHR VulkanSwapChain::GetSwapChainPresentMode(std::vector<VkPresentModeKHR> &presentModes)
{
	// MAILBOX is the lowest latency V-Sync enabled mode (something like triple-buffering) so use it if available
	std::vector<VkPresentModeKHR>::iterator iter = presentModes.begin();
	for (; iter != presentModes.end(); iter++)
	{
		if (*iter == VK_PRESENT_MODE_MAILBOX_KHR)
		{
			return *iter;
		}
	}

	std::vector<VkPresentModeKHR>::iterator iter1 = presentModes.begin();
	for (; iter1 != presentModes.end(); iter1++)
	{
		if (*iter1 == VK_PRESENT_MODE_FIFO_KHR)
		{
			return *iter1;
		}
	}

	// return the first one
	return presentModes[0];

}

}
