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

#include<limits>

namespace cave
{

VulkanSwapChain::VulkanSwapChain(VulkanInstance* instance, VulkanPhysicalDevice* physicalDevice
								, VulkanRenderDevice* renderDevice, SwapChainInfo& swapChainInfo)
	: _pInstance(instance)
	, _pPhysicalDevice(physicalDevice)
	, _pRenderDevice(renderDevice)
	, _presentationSurface(nullptr)
{
	// first query if the physical device support s presentation at all
	uint32_t presentQueueFamiliyIndex = (std::numeric_limits<uint32_t>::max)();
	if (!physicalDevice->PresentationQueueSupported(swapChainInfo, presentQueueFamiliyIndex))
		throw BackendException("Presentation queues are not supported by this device");

	// create presentation surface
	VkResult success = VK_SUCCESS;
#ifdef VK_USE_PLATFORM_WIN32_KHR
	VkWin32SurfaceCreateInfoKHR surfaceCreateInfo = {};
	surfaceCreateInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
	surfaceCreateInfo.pNext = nullptr;
	surfaceCreateInfo.flags = 0;
	surfaceCreateInfo.hinstance = swapChainInfo.hInstance;
	surfaceCreateInfo.hwnd = swapChainInfo.hWindow;
	success = VulkanApi::GetApi()->vkCreateWin32SurfaceKHR(instance->GetInstanceHandle(), &surfaceCreateInfo, nullptr, &_presentationSurface);
#else
	VkXcbSurfaceCreateInfoKHR surfaceCreateInfo = {};
	surfaceCreateInfo.sType = VK_STRUCTURE_TYPE_XCB_SURFACE_CREATE_INFO_KHR;
	surfaceCreateInfo.pNext = nullptr;
	surfaceCreateInfo.flags = 0;
	success = VulkanApi::GetApi()->vkCreateXcbSurfaceKHR(instance->GetInstanceHandle(), &surfaceCreateInfo, nullptr, &_presentationSurface);
#endif
}

VulkanSwapChain::~VulkanSwapChain()
{

}

}
