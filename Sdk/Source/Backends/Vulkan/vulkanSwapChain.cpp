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
	, VulkanRenderDevice* renderDevice, SwapChainInfo& swapChainInfo)
	: _pInstance(instance)
	, _pPhysicalDevice(physicalDevice)
	, _pRenderDevice(renderDevice)
	, _swapChain(nullptr)
	, _swapChainImageVector(instance->GetEngineAllocator())
	, _swapChainImageViewVector(instance->GetEngineAllocator())
	, _imageIndex(0)
	, _swapChainImageFormat(VK_FORMAT_UNDEFINED)
	, _swapChainDepthImageFormat(VK_FORMAT_UNDEFINED)
	, _swapChainDepthImage(VK_NULL_HANDLE)
	, _swapChainDepthImageMemory(VK_NULL_HANDLE)
	, _swapChainDepthImageView(VK_NULL_HANDLE)
	, _ImageAvailableSemaphore(VK_NULL_HANDLE)
	, _RenderingFinishedSemaphore(VK_NULL_HANDLE)
{
	CreateSwapChain();
	CreateImageViews();
	CreateDepthResource(swapChainInfo.depthBits, swapChainInfo.stencilBits);
	CreatePresentationSemaphores();
}

VulkanSwapChain::~VulkanSwapChain()
{
	VulkanApi::GetApi()->vkDeviceWaitIdle(_pRenderDevice->GetDeviceHandle());

	if (!_swapChainImageViewVector.Empty())
	{
		for (size_t i = 0; i < _swapChainImageViewVector.Size(); i++)
			VulkanApi::GetApi()->vkDestroyImageView(_pRenderDevice->GetDeviceHandle(), _swapChainImageViewVector[i], nullptr);
	}

	_swapChainImageViewVector.Clear();
	_swapChainImageVector.Clear();

	if (_swapChainDepthImageView != VK_NULL_HANDLE)
		VulkanApi::GetApi()->vkDestroyImageView(_pRenderDevice->GetDeviceHandle(), _swapChainDepthImageView, nullptr);

	if (_swapChainDepthImage != VK_NULL_HANDLE)
		VulkanApi::GetApi()->vkDestroyImage(_pRenderDevice->GetDeviceHandle(), _swapChainDepthImage, nullptr);

	if (_swapChainDepthImageMemory != VK_NULL_HANDLE)
		VulkanApi::GetApi()->vkFreeMemory(_pRenderDevice->GetDeviceHandle(), _swapChainDepthImageMemory, nullptr);

	if (_ImageAvailableSemaphore)
		VulkanApi::GetApi()->vkDestroySemaphore(_pRenderDevice->GetDeviceHandle(), _ImageAvailableSemaphore, nullptr);
	if (_RenderingFinishedSemaphore)
		VulkanApi::GetApi()->vkDestroySemaphore(_pRenderDevice->GetDeviceHandle(), _RenderingFinishedSemaphore, nullptr);

	if (_swapChain)
		VulkanApi::GetApi()->vkDestroySwapchainKHR(_pRenderDevice->GetDeviceHandle(), _swapChain, nullptr);
}

void VulkanSwapChain::CreateSwapChain()
{
	VkSurfaceKHR presentationSurface = _pRenderDevice->GetPresentationSurfaceHandle();
	
	VkSurfaceCapabilitiesKHR surfaceCapabilities;
	VulkanApi::GetApi()->vkGetPhysicalDeviceSurfaceCapabilitiesKHR(_pPhysicalDevice->GetPhysicalDeviceHandle()
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
	uint32_t swapImageCount = 0;
	VulkanApi::GetApi()->vkGetSwapchainImagesKHR(_pRenderDevice->GetDeviceHandle(), _swapChain, &swapImageCount, nullptr);
	if (!swapImageCount)
		throw BackendException("Failed to get swap chain images");

	_swapChainImageVector.Resize(swapImageCount);
	if (!_swapChainImageVector.Size())
		throw BackendException("Failed to allocate memory for swap chain images");

	VulkanApi::GetApi()->vkGetSwapchainImagesKHR(_pRenderDevice->GetDeviceHandle(), _swapChain, &swapImageCount, _swapChainImageVector.Data());

	// save for later usage
	_swapChainImageFormat = surfaceFormat.format;
	_swapChainExtent = swapChainCreateInfo.imageExtent;
}

void VulkanSwapChain::CreateImageViews()
{
	_swapChainImageViewVector.Resize(_swapChainImageVector.Size());
	if (!_swapChainImageViewVector.Size())
		throw BackendException("Failed to allocate memory for swap chain image views");

	for (uint32_t i = 0; i < _swapChainImageVector.Size(); ++i)
	{
		VkImageViewCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		createInfo.image = _swapChainImageVector[i];
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

		if (VulkanApi::GetApi()->vkCreateImageView(_pRenderDevice->GetDeviceHandle(), &createInfo, nullptr, &_swapChainImageViewVector[i]) != VK_SUCCESS)
		{
			throw BackendException("Failed to create image views!");
		}
	}
}

void VulkanSwapChain::CreateDepthResource(uint32_t depthBits, uint32_t StencilBits)
{
	// Check if we are required to allocate depth resource
	if (depthBits == 0)
		return; // We do not support stencil only

	// 1. Select and find matching format in order
	caveVector<VkFormat> formats(_pInstance->GetEngineAllocator());
	if (depthBits == 32 && StencilBits == 8)
	{
		formats.Push(VK_FORMAT_D32_SFLOAT_S8_UINT);
		formats.Push(VK_FORMAT_D24_UNORM_S8_UINT);
		formats.Push(VK_FORMAT_D32_SFLOAT);
	}
	if (depthBits == 32)
	{
		formats.Push(VK_FORMAT_D32_SFLOAT);
		formats.Push(VK_FORMAT_D32_SFLOAT_S8_UINT);
		formats.Push(VK_FORMAT_D24_UNORM_S8_UINT);
	}
	if (depthBits == 24)
	{
		formats.Push(VK_FORMAT_D24_UNORM_S8_UINT);
		formats.Push(VK_FORMAT_D32_SFLOAT_S8_UINT);
		formats.Push(VK_FORMAT_D32_SFLOAT);
	}

	_swapChainDepthImageFormat = _pRenderDevice->FindMatchingImageFormat(formats, VK_IMAGE_TILING_OPTIMAL, VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);

	if (_swapChainDepthImageFormat == VK_FORMAT_UNDEFINED)
		throw BackendException("Error image depth format not supported");

	// 2. Create image
	VkImageCreateInfo imageInfo = {};
	imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	imageInfo.imageType = VK_IMAGE_TYPE_2D;
	imageInfo.extent.width = _swapChainExtent.width;
	imageInfo.extent.height = _swapChainExtent.height;
	imageInfo.extent.depth = 1;
	imageInfo.mipLevels = 1;
	imageInfo.arrayLayers = 1;
	imageInfo.format = _swapChainDepthImageFormat;
	imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
	imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	imageInfo.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
	imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
	imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	if (VulkanApi::GetApi()->vkCreateImage(_pRenderDevice->GetDeviceHandle(), &imageInfo, nullptr, &_swapChainDepthImage) != VK_SUCCESS)
		throw BackendException("Error creating swapchain depthimage!");

	VkMemoryRequirements memRequirements;
	VulkanApi::GetApi()->vkGetImageMemoryRequirements(_pRenderDevice->GetDeviceHandle(), _swapChainDepthImage, &memRequirements);

	VulkanMemoryManager* memManager = _pRenderDevice->GetMemoryManager();

	VkMemoryAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = memManager->ChooseMemoryType(memRequirements, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

	if (VulkanApi::GetApi()->vkAllocateMemory(_pRenderDevice->GetDeviceHandle(), &allocInfo, nullptr, &_swapChainDepthImageMemory) != VK_SUCCESS)
		throw BackendException("Error allocating depth image memory!");

	VulkanApi::GetApi()->vkBindImageMemory(_pRenderDevice->GetDeviceHandle(), _swapChainDepthImage, _swapChainDepthImageMemory, 0);

	// 3. Create depth image view
	VkImageViewCreateInfo viewInfo = {};
	viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	viewInfo.image = _swapChainDepthImage;
	viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
	viewInfo.format = _swapChainDepthImageFormat;
	viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
	viewInfo.subresourceRange.baseMipLevel = 0;
	viewInfo.subresourceRange.levelCount = 1;
	viewInfo.subresourceRange.baseArrayLayer = 0;
	viewInfo.subresourceRange.layerCount = 1;

	if (VulkanApi::GetApi()->vkCreateImageView(_pRenderDevice->GetDeviceHandle(), &viewInfo, nullptr, &_swapChainDepthImageView) != VK_SUCCESS) 
		throw BackendException("Error creating swapchain depth image view!");
}

void VulkanSwapChain::CreatePresentationSemaphores()
{
	VkSemaphoreCreateInfo semaphoreCreateInfo = {};
	semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
	semaphoreCreateInfo.flags = 0;
	

	VulkanApi::GetApi()->vkCreateSemaphore(_pRenderDevice->GetDeviceHandle(), &semaphoreCreateInfo, nullptr, &_ImageAvailableSemaphore);
	VulkanApi::GetApi()->vkCreateSemaphore(_pRenderDevice->GetDeviceHandle(), &semaphoreCreateInfo, nullptr, &_RenderingFinishedSemaphore);
	if (_ImageAvailableSemaphore == VK_NULL_HANDLE || _RenderingFinishedSemaphore == VK_NULL_HANDLE)
	{
		throw BackendException("Failed to create presentation semaphores!");
	}
}

const VkImageView VulkanSwapChain::GetSwapChainImageView(size_t index) const
{
	VkImageView imageView = VK_NULL_HANDLE;
	if (index < _swapChainImageViewVector.Size())
	{
		return _swapChainImageViewVector[index];
	}

	return imageView;
}

const VkImageView VulkanSwapChain::GetSwapChainDepthImageView() const
{
	return _swapChainDepthImageView;
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
	if (surfaceCapabilities.currentExtent.width == static_cast<uint32_t>(-1))
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

const uint32_t VulkanSwapChain::AcquireNextSwapChainImage(uint64_t timeout)
{
	if (!_swapChain)
		return (std::numeric_limits<uint32_t>::max)();

	VulkanApi::GetApi()->vkAcquireNextImageKHR(_pRenderDevice->GetDeviceHandle(), _swapChain, timeout, _ImageAvailableSemaphore, VK_NULL_HANDLE, &_imageIndex);

	return _imageIndex;
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
	if (surfaceCapabilities.supportedUsageFlags & VK_IMAGE_USAGE_TRANSFER_SRC_BIT)
	{
		usageFlags |= VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
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

void VulkanSwapChain::ReadPixels(VkCommandPool commandPool, void* data)
{
	VulkanMemoryManager* memManager = _pRenderDevice->GetMemoryManager();
	// wait until all rendering is done
	_pRenderDevice->WaitIdle();

	// Get source image
	VkImage srcImage = _swapChainImageVector[_imageIndex];

	// allocate buffer
	VkBufferCreateInfo bufferCreateInfo = {};
	bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferCreateInfo.pNext = nullptr;
	bufferCreateInfo.size = _swapChainExtent.height * _swapChainExtent.width * 4 * sizeof(int8_t);
	bufferCreateInfo.usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT;
	bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	VkBuffer destBuffer;
	VulkanApi::GetApi()->vkCreateBuffer(_pRenderDevice->GetDeviceHandle(), &bufferCreateInfo, nullptr, &destBuffer);

	// query memory requirements for this buffer
	VkMemoryRequirements memRequirements;
	VulkanApi::GetApi()->vkGetBufferMemoryRequirements(_pRenderDevice->GetDeviceHandle(), destBuffer, &memRequirements);
	VkMemoryPropertyFlags memProperties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
	// allcoate memory
	VulkanDeviceMemory destMemory;
	memManager->AllocateBufferMemory(memRequirements, memProperties, destMemory);
	VulkanApi::GetApi()->vkBindBufferMemory(_pRenderDevice->GetDeviceHandle(), destBuffer, destMemory._vkDeviceMemory, destMemory._offset);

	// allocate command buffer used for read backs
	VkCommandBufferAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandPool = commandPool;
	allocInfo.commandBufferCount = 1;

	VkCommandBuffer copyCmdBuffer;
	VulkanApi::GetApi()->vkAllocateCommandBuffers(_pRenderDevice->GetDeviceHandle(), &allocInfo, &copyCmdBuffer);

	VkCommandBufferBeginInfo beginInfo = {};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
	VulkanApi::GetApi()->vkBeginCommandBuffer(copyCmdBuffer, &beginInfo);

	// Transition source image from present to transfer layout
	VkImageMemoryBarrier srcImageStartBarrier;
	srcImageStartBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	srcImageStartBarrier.pNext = nullptr;
	srcImageStartBarrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
	srcImageStartBarrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
	srcImageStartBarrier.oldLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
	srcImageStartBarrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
	srcImageStartBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	srcImageStartBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	srcImageStartBarrier.image = srcImage;
	srcImageStartBarrier.subresourceRange = VkImageSubresourceRange{ VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 };
	VulkanApi::GetApi()->vkCmdPipelineBarrier(copyCmdBuffer, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0
		, 0, nullptr
		, 0, nullptr
		, 1, &srcImageStartBarrier);

	// copy image to buffer
	VkBufferImageCopy bufferImageCopy = {};
	bufferImageCopy.bufferOffset = 0;
	bufferImageCopy.bufferRowLength = 0;		// tightly packed
	bufferImageCopy.bufferImageHeight = 0;		// tightly packed
	bufferImageCopy.imageExtent.width = _swapChainExtent.width;
	bufferImageCopy.imageExtent.height = _swapChainExtent.height;
	bufferImageCopy.imageExtent.depth = 1;
	bufferImageCopy.imageSubresource = VkImageSubresourceLayers{ VK_IMAGE_ASPECT_COLOR_BIT, 0, 0, 1};
	VulkanApi::GetApi()->vkCmdCopyImageToBuffer(copyCmdBuffer, srcImage, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, destBuffer, 1, &bufferImageCopy);

	// Transition source image back to present layout
	VkImageMemoryBarrier srcImageEndBarrier;
	srcImageEndBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	srcImageEndBarrier.pNext = nullptr;
	srcImageEndBarrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
	srcImageEndBarrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
	srcImageEndBarrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
	srcImageEndBarrier.newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
	srcImageEndBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	srcImageEndBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	srcImageEndBarrier.image = srcImage;
	srcImageEndBarrier.subresourceRange = VkImageSubresourceRange{ VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 };
	VulkanApi::GetApi()->vkCmdPipelineBarrier(copyCmdBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, 0
		, 0, nullptr
		, 0, nullptr
		, 1, &srcImageEndBarrier);

	VulkanApi::GetApi()->vkEndCommandBuffer(copyCmdBuffer);

	// submit job
	VkSubmitInfo submitInfo = {};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &copyCmdBuffer;

	// Create fence to ensure that the command buffer has finished executing
	VkFenceCreateInfo fenceInfo = {};
	fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceInfo.pNext = nullptr;
	fenceInfo.flags = 0;
	VkFence fence;
	VulkanApi::GetApi()->vkCreateFence(_pRenderDevice->GetDeviceHandle(), &fenceInfo, nullptr, &fence);

	if ( VulkanApi::GetApi()->vkQueueSubmit(_pRenderDevice->GetGraphicsQueue(), 1, &submitInfo, fence) == VK_SUCCESS)
		VulkanApi::GetApi()->vkWaitForFences(_pRenderDevice->GetDeviceHandle(), 1, &fence, VK_TRUE, (std::numeric_limits<uint64_t>::max)());

	VulkanApi::GetApi()->vkDestroyFence(_pRenderDevice->GetDeviceHandle(), fence, nullptr);

	VulkanApi::GetApi()->vkFreeCommandBuffers(_pRenderDevice->GetDeviceHandle(), commandPool, 1, &copyCmdBuffer);

	// map buffer
	const uint8_t* pixels;
	uint32_t rowPitch = _swapChainExtent.width * 4;
	VulkanApi::GetApi()->vkMapMemory(_pRenderDevice->GetDeviceHandle(), destMemory._vkDeviceMemory, 0, VK_WHOLE_SIZE, 0, (void**)&pixels);
	pixels += memRequirements.size - rowPitch; // we start at last row

	// check for swizzle
	bool needSwizzle = false;
	if ((_swapChainImageFormat == VK_FORMAT_B8G8R8A8_UNORM))
		needSwizzle = true;

	uint8_t* dataPtr = static_cast<uint8_t*>(data);

	// copy data
	for (uint32_t h = 0; h < _swapChainExtent.height; h++)
	{
		uint8_t *row = (uint8_t*)pixels;
		for (uint32_t w = 0; w < _swapChainExtent.width; w++)
		{
			if (needSwizzle)
			{
				*dataPtr++ = *(row + 2);
				*dataPtr++ = *(row + 1);
				*dataPtr++ = *(row + 0);
				*dataPtr++ = *(row + 3);
			}
			else
			{
				*dataPtr++ = *(row + 0);
				*dataPtr++ = *(row + 1);
				*dataPtr++ = *(row + 2);
				*dataPtr++ = *(row + 3);
			}
			row += 4;
		}
		pixels -= rowPitch; // from bottom to top
	}

	VulkanApi::GetApi()->vkUnmapMemory(_pRenderDevice->GetDeviceHandle(), destMemory._vkDeviceMemory);
	VulkanApi::GetApi()->vkDestroyBuffer(_pRenderDevice->GetDeviceHandle(), destBuffer, nullptr);
	memManager->ReleaseBufferMemory(destMemory);
}

}
