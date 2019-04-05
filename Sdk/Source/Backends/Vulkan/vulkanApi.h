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

/// @file vulkanApi.h
///       Vulkan instance

#include "osPlatformLib.h"
#include "vulkan.h"

#include <iostream>

/** \addtogroup backend
*  @{
*
*/

namespace cave
{

/** function pointer definitions of Vulkan API
* @{
*/
typedef PFN_vkVoidFunction(VKAPI_PTR* vkGetInstanceProcAddrPtr) (VkInstance instance, const char* pName);
typedef VkResult(VKAPI_PTR* vkEnumerateInstanceExtensionPropertiesPtr) (const char* pLayerName, uint32_t* pPropertyCount, VkExtensionProperties* pProperties);
typedef VkResult(VKAPI_PTR* vkEnumerateInstanceLayerPropertiesPtr)(uint32_t* pPropertyCount, VkLayerProperties* pProperties);
typedef VkResult(VKAPI_PTR* vkCreateInstancePtr) (const VkInstanceCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkInstance* pInstance);
typedef VkResult(VKAPI_PTR* vkEnumerateInstanceVersionPtr)(uint32_t* pApiVersion);
typedef void		(VKAPI_PTR* vkDestroyInstancePtr) (VkInstance instance, const VkAllocationCallbacks* pAllocator);
typedef VkResult(VKAPI_PTR* vkEnumeratePhysicalDevicesPtr) (VkInstance instance, uint32_t* pPhysicalDeviceCount, VkPhysicalDevice* pPhysicalDevices);
typedef VkResult(VKAPI_PTR* vkCreateDebugReportCallbackEXTPtr)(VkInstance instance, const VkDebugReportCallbackCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugReportCallbackEXT* pCallback);
typedef void		(VKAPI_PTR* vkDestroyDebugReportCallbackEXTPtr)(VkInstance instance, VkDebugReportCallbackEXT callback, const VkAllocationCallbacks* pAllocator);
typedef void		(VKAPI_PTR* vkDebugReportMessageEXTPtr)(VkInstance instance, VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objectType, uint64_t object, size_t location, int32_t messageCode, const char* pLayerPrefix, const char* pMessage);

typedef void		(VKAPI_PTR* vkGetPhysicalDevicePropertiesPtr) (VkPhysicalDevice physicalDevice, VkPhysicalDeviceProperties* pProperties);
typedef void		(VKAPI_PTR* vkGetPhysicalDeviceFeaturesPtr) (VkPhysicalDevice physicalDevice, VkPhysicalDeviceFeatures* pFeatures);
typedef void		(VKAPI_PTR* vkGetPhysicalDeviceFormatPropertiesPtr)(VkPhysicalDevice physicalDevice, VkFormat format, VkFormatProperties* pFormatProperties);
typedef void		(VKAPI_PTR* vkGetPhysicalDeviceQueueFamilyPropertiesPtr) (VkPhysicalDevice physicalDevice, uint32_t* pQueueFamilyPropertyCount, VkQueueFamilyProperties* pQueueFamilyProperties);
typedef VkResult(VKAPI_PTR* vkCreateDevicePtr) (VkPhysicalDevice physicalDevice, const VkDeviceCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDevice* pDevice);
typedef PFN_vkVoidFunction(VKAPI_PTR* vkGetDeviceProcAddrPtr) (VkDevice device, const char* pName);
typedef VkResult(VKAPI_PTR* vkEnumerateDeviceExtensionPropertiesPtr) (VkPhysicalDevice physicalDevice, const char* pLayerName, uint32_t* pPropertyCount, VkExtensionProperties* pProperties);
typedef void		(VKAPI_PTR* vkGetPhysicalDeviceMemoryPropertiesPtr) (VkPhysicalDevice physicalDevice, VkPhysicalDeviceMemoryProperties* pMemoryProperties);
typedef void		(VKAPI_PTR* vkGetDeviceQueuePtr) (VkDevice device, uint32_t queueFamilyIndex, uint32_t queueIndex, VkQueue* pQueue);
typedef void		(VKAPI_PTR* vkDestroyDevicePtr) (VkDevice device, const VkAllocationCallbacks* pAllocator);
typedef VkResult(VKAPI_PTR* vkDeviceWaitIdlePtr) (VkDevice device);

typedef void		(VKAPI_PTR* vkDestroySurfaceKHRPtr)(VkInstance instance, VkSurfaceKHR surface, const VkAllocationCallbacks* pAllocator);
typedef VkResult(VKAPI_PTR* vkGetPhysicalDeviceSurfaceSupportKHRPtr) (VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, VkSurfaceKHR surface, VkBool32* pSupported);
typedef VkResult(VKAPI_PTR* vkGetPhysicalDeviceSurfaceCapabilitiesKHRPtr) (VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, VkSurfaceCapabilitiesKHR* pSurfaceCapabilities);
typedef VkResult(VKAPI_PTR* vkGetPhysicalDeviceSurfaceFormatsKHRPtr) (VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, uint32_t* pSurfaceFormatCount, VkSurfaceFormatKHR* pSurfaceFormats);
typedef VkResult(VKAPI_PTR* vkGetPhysicalDeviceSurfacePresentModesKHRPtr) (VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, uint32_t* pPresentModeCount, VkPresentModeKHR* pPresentModes);
#if defined(VK_USE_PLATFORM_WIN32_KHR)
typedef VkResult(VKAPI_PTR* vkCreateWin32SurfaceKHRPtr) (VkInstance instance, const VkWin32SurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);
typedef VkBool32(VKAPI_PTR* vkGetPhysicalDeviceWin32PresentationSupportKHRPtr) (VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex);
#elif defined(VK_USE_PLATFORM_XCB_KHR)
typedef VkResult(VKAPI_PTR* vkCreateXcbSurfaceKHRPtr) (VkInstance instance, const VkXcbSurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);
typedef VkBool32(VKAPI_PTR* vkGetPhysicalDeviceXcbPresentationSupportKHRPtr) (VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, xcb_connection_t* connection, xcb_visualid_t visual_id);
#endif

typedef VkResult(VKAPI_PTR* vkCreateSwapchainKHRPtr) (VkDevice device, const VkSwapchainCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSwapchainKHR* pSwapchain);
typedef void		(VKAPI_PTR* vkDestroySwapchainKHRPtr) (VkDevice device, VkSwapchainKHR swapchain, const VkAllocationCallbacks* pAllocator);
typedef VkResult(VKAPI_PTR* vkGetSwapchainImagesKHRPtr) (VkDevice device, VkSwapchainKHR swapchain, uint32_t* pSwapchainImageCount, VkImage* pSwapchainImages);
typedef VkResult(VKAPI_PTR* vkAcquireNextImageKHRPtr)(VkDevice device, VkSwapchainKHR swapchain, uint64_t timeout, VkSemaphore semaphore, VkFence fence, uint32_t* pImageIndex);
typedef VkResult(VKAPI_PTR* vkQueuePresentKHRPtr) (VkQueue queue, const VkPresentInfoKHR* pPresentInfo);

typedef VkResult(VKAPI_PTR* vkAllocateMemoryPtr)(VkDevice device, const VkMemoryAllocateInfo* pAllocateInfo, const VkAllocationCallbacks*  pAllocator, VkDeviceMemory* pMemory);
typedef void		(VKAPI_PTR* vkFreeMemoryPtr)(VkDevice device, VkDeviceMemory memory, const VkAllocationCallbacks* pAllocator);
typedef VkResult(VKAPI_PTR* vkCreateSemaphorePtr) (VkDevice device, const VkSemaphoreCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSemaphore* pSemaphore);
typedef void		(VKAPI_PTR* vkDestroySemaphorePtr) (VkDevice device, VkSemaphore semaphore, const VkAllocationCallbacks* pAllocator);
typedef VkResult(VKAPI_PTR* vkCreateFencePtr)(VkDevice device, const VkFenceCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkFence* pFence);
typedef void		(VKAPI_PTR* vkDestroyFencePtr)(VkDevice device, VkFence fence, const VkAllocationCallbacks* pAllocator);
typedef VkResult(VKAPI_PTR* vkResetFencesPtr)(VkDevice device, uint32_t fenceCount, const VkFence* pFences);
typedef VkResult(VKAPI_PTR* vkGetFenceStatusPtr)(VkDevice device, VkFence fence);
typedef VkResult(VKAPI_PTR* vkWaitForFencesPtr)(VkDevice device, uint32_t fenceCount, const VkFence* pFences, VkBool32 waitAll, uint64_t timeout);
typedef VkResult(VKAPI_PTR* vkCreateBufferPtr)(VkDevice device, const VkBufferCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkBuffer* pBuffer);
typedef void		(VKAPI_PTR* vkDestroyBufferPtr)(VkDevice device, VkBuffer buffer, const VkAllocationCallbacks* pAllocator);
typedef VkResult(VKAPI_PTR* vkBindBufferMemoryPtr)(VkDevice device, VkBuffer buffer, VkDeviceMemory memory, VkDeviceSize memoryOffset);
typedef VkResult(VKAPI_PTR* vkBindImageMemoryPtr)(VkDevice device, VkImage image, VkDeviceMemory memory, VkDeviceSize memoryOffset);
typedef VkResult(VKAPI_PTR* vkMapMemoryPtr)(VkDevice device, VkDeviceMemory memory, VkDeviceSize offset, VkDeviceSize size, VkMemoryMapFlags flags, void** ppData);
typedef void		(VKAPI_PTR* vkUnmapMemoryPtr)(VkDevice device, VkDeviceMemory memory);
typedef VkResult(VKAPI_PTR* vkFlushMappedMemoryRangesPtr)(VkDevice device, uint32_t memoryRangeCount, const VkMappedMemoryRange* pMemoryRanges);
typedef VkResult(VKAPI_PTR* vkQueueSubmitPtr) (VkQueue queue, uint32_t submitCount, const VkSubmitInfo* pSubmits, VkFence fence);
typedef VkResult(VKAPI_PTR* vkQueueWaitIdlePtr)(VkQueue queue);
typedef void		(VKAPI_PTR* vkGetBufferMemoryRequirementsPtr)(VkDevice device, VkBuffer buffer, VkMemoryRequirements* pMemoryRequirements);
typedef void		(VKAPI_PTR* vkGetBufferMemoryRequirements2Ptr)(VkDevice device, const VkBufferMemoryRequirementsInfo2* pInfo, VkMemoryRequirements2* pMemoryRequirements);
typedef void		(VKAPI_PTR* vkGetImageMemoryRequirementsPtr)(VkDevice device, VkImage image, VkMemoryRequirements* pMemoryRequirements);
typedef VkResult(VKAPI_PTR* vkCreateCommandPoolPtr) (VkDevice device, const VkCommandPoolCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkCommandPool* pCommandPool);
typedef VkResult(VKAPI_PTR* vkAllocateCommandBuffersPtr) (VkDevice device, const VkCommandBufferAllocateInfo* pAllocateInfo, VkCommandBuffer* pCommandBuffers);
typedef void		(VKAPI_PTR* vkDestroyCommandPoolPtr) (VkDevice device, VkCommandPool commandPool, const VkAllocationCallbacks* pAllocator);
typedef VkResult(VKAPI_PTR* vkBeginCommandBufferPtr) (VkCommandBuffer commandBuffer, const VkCommandBufferBeginInfo* pBeginInfo);
typedef VkResult(VKAPI_PTR* vkEndCommandBufferPtr) (VkCommandBuffer commandBuffer);
typedef VkResult(VKAPI_PTR* vkResetCommandBufferPtr)(VkCommandBuffer commandBuffer, VkCommandBufferResetFlags flags);
typedef void		(VKAPI_PTR* vkFreeCommandBuffersPtr) (VkDevice device, VkCommandPool commandPool, uint32_t commandBufferCount, const VkCommandBuffer* pCommandBuffers);
typedef void		(VKAPI_PTR* vkCmdPipelineBarrierPtr) (VkCommandBuffer commandBuffer, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask
    , VkDependencyFlags dependencyFlags, uint32_t memoryBarrierCount, const VkMemoryBarrier* pMemoryBarriers
    , uint32_t bufferMemoryBarrierCount, const VkBufferMemoryBarrier* pBufferMemoryBarriers
    , uint32_t imageMemoryBarrierCount, const VkImageMemoryBarrier* pImageMemoryBarriers);
typedef void		(VKAPI_PTR* vkCmdClearColorImagePtr) (VkCommandBuffer commandBuffer, VkImage image, VkImageLayout imageLayout, const VkClearColorValue* pColor
    , uint32_t rangeCount, const VkImageSubresourceRange* pRanges);
typedef VkResult(VKAPI_PTR* vkCreateRenderPassPtr) (VkDevice device, const VkRenderPassCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkRenderPass* pRenderPass);
typedef void		(VKAPI_PTR* vkDestroyRenderPassPtr) (VkDevice device, VkRenderPass renderPass, const VkAllocationCallbacks* pAllocator);
typedef VkResult(VKAPI_PTR* vkCreateFramebufferPtr) (VkDevice device, const VkFramebufferCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkFramebuffer* pFramebuffer);
typedef void		(VKAPI_PTR* vkDestroyFramebufferPtr) (VkDevice device, VkFramebuffer framebuffer, const VkAllocationCallbacks* pAllocator);
typedef VkResult(VKAPI_PTR* vkCreateImagePtr)(VkDevice device, const VkImageCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkImage* pImage);
typedef void		(VKAPI_PTR* vkDestroyImagePtr)(VkDevice device, VkImage image, const VkAllocationCallbacks* pAllocator);
typedef void		(VKAPI_PTR* vkGetImageSubresourceLayoutPtr)(VkDevice device, VkImage image, const VkImageSubresource* pSubresource, VkSubresourceLayout* pLayout);
typedef VkResult(VKAPI_PTR* vkCreateImageViewPtr) (VkDevice device, const VkImageViewCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkImageView* pView);
typedef void		(VKAPI_PTR* vkDestroyImageViewPtr) (VkDevice device, VkImageView imageView, const VkAllocationCallbacks* pAllocator);
typedef VkResult(VKAPI_PTR* vkCreateShaderModulePtr) (VkDevice device, const VkShaderModuleCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkShaderModule* pShaderModule);
typedef void		(VKAPI_PTR* vkDestroyShaderModulePtr) (VkDevice device, VkShaderModule shaderModule, const VkAllocationCallbacks* pAllocator);
typedef VkResult(VKAPI_PTR* vkCreatePipelineLayoutPtr) (VkDevice device, const VkPipelineLayoutCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkPipelineLayout* pPipelineLayout);
typedef void		(VKAPI_PTR* vkDestroyPipelineLayoutPtr) (VkDevice device, VkPipelineLayout pipelineLayout, const VkAllocationCallbacks* pAllocator);
typedef VkResult(VKAPI_PTR* vkCreateDescriptorSetLayoutPtr) (VkDevice device, const VkDescriptorSetLayoutCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDescriptorSetLayout* pSetLayout);
typedef void		(VKAPI_PTR* vkDestroyDescriptorSetLayoutPtr) (VkDevice device, VkDescriptorSetLayout descriptorSetLayout, const VkAllocationCallbacks* pAllocator);
typedef VkResult(VKAPI_PTR* vkCreateDescriptorPoolPtr) (VkDevice device, const VkDescriptorPoolCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDescriptorPool* pDescriptorPool);
typedef void		(VKAPI_PTR* vkDestroyDescriptorPoolPtr) (VkDevice device, VkDescriptorPool descriptorPool, const VkAllocationCallbacks* pAllocator);
typedef VkResult(VKAPI_PTR* vkResetDescriptorPoolPtr)(VkDevice device, VkDescriptorPool descriptorPool, VkDescriptorPoolResetFlags flags);
typedef VkResult(VKAPI_PTR* vkAllocateDescriptorSetsPtr)(VkDevice device, const VkDescriptorSetAllocateInfo* pAllocateInfo, VkDescriptorSet* pDescriptorSets);
typedef VkResult(VKAPI_PTR* vkFreeDescriptorSetsPtr)(VkDevice device, VkDescriptorPool descriptorPool, uint32_t descriptorSetCount, const VkDescriptorSet* pDescriptorSets);
typedef void		(VKAPI_PTR* vkUpdateDescriptorSetsPtr)(VkDevice device, uint32_t descriptorWriteCount, const VkWriteDescriptorSet* pDescriptorWrites, uint32_t descriptorCopyCount, const VkCopyDescriptorSet* pDescriptorCopies);
typedef VkResult(VKAPI_PTR* vkCreateGraphicsPipelinesPtr) (VkDevice device, VkPipelineCache pipelineCache, uint32_t createInfoCount, const VkGraphicsPipelineCreateInfo* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines);
typedef VkResult(VKAPI_PTR* vkCreateComputePipelinesPtr) (VkDevice device, VkPipelineCache pipelineCache, uint32_t createInfoCount, const VkComputePipelineCreateInfo* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines);
typedef void		(VKAPI_PTR* vkDestroyPipelinePtr) (VkDevice device, VkPipeline pipeline, const VkAllocationCallbacks* pAllocator);
typedef void		(VKAPI_PTR* vkCmdBindPipelinePtr) (VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipeline pipeline);
typedef void		(VKAPI_PTR* vkCmdBindDescriptorSetsPtr)(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t firstSet, uint32_t descriptorSetCount, const VkDescriptorSet* pDescriptorSets, uint32_t dynamicOffsetCount, const uint32_t* pDynamicOffsets);
typedef void		(VKAPI_PTR* vkCmdBindIndexBufferPtr)(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkIndexType indexType);
typedef void		(VKAPI_PTR* vkCmdBindVertexBuffersPtr)(VkCommandBuffer commandBuffer, uint32_t firstBinding, uint32_t bindingCount, const VkBuffer* pBuffers, const VkDeviceSize* pOffsets);
typedef void		(VKAPI_PTR* vkCmdPushConstantsPtr)(VkCommandBuffer commandBuffer, VkPipelineLayout layout, VkShaderStageFlags stageFlags, uint32_t offset, uint32_t size, const void* pValues);
typedef void		(VKAPI_PTR* vkCmdBeginRenderPassPtr) (VkCommandBuffer commandBuffer, const VkRenderPassBeginInfo* pRenderPassBegin, VkSubpassContents contents);
typedef void		(VKAPI_PTR* vkCmdNextSubpassPtr) (VkCommandBuffer commandBuffer, VkSubpassContents contents);
typedef void		(VKAPI_PTR* vkCmdEndRenderPassPtr) (VkCommandBuffer commandBuffer);
typedef void		(VKAPI_PTR* vkCmdDrawPtr) (VkCommandBuffer commandBuffer, uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance);
typedef void		(VKAPI_PTR* vkCmdDrawIndexedPtr)(VkCommandBuffer commandBuffer, uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance);
typedef void		(VKAPI_PTR* vkCmdCopyBufferPtr) (VkCommandBuffer commandBuffer, VkBuffer srcBuffer, VkBuffer dstBuffer, uint32_t regionCount, const VkBufferCopy* pRegions);
typedef void		(VKAPI_PTR* vkCmdCopyImagePtr)(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkImageCopy* pRegions);
typedef void		(VKAPI_PTR* vkCmdBlitImagePtr)(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkImageBlit* pRegions, VkFilter filter);
typedef void		(VKAPI_PTR* vkCmdCopyImageToBufferPtr)(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkBuffer dstBuffer, uint32_t regionCount, const VkBufferImageCopy* pRegions);
typedef void        (VKAPI_PTR* vkCmdCopyBufferToImagePtr)(VkCommandBuffer commandBuffer, VkBuffer srcBuffer, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkBufferImageCopy* pRegions);

/** @}*/


/**
* Vulkan api
*/
class VulkanApi
{
private:
    /**
    * @brief Constructor
    *
    */
    VulkanApi()
    {
#if defined(_WIN32)		
        _hVulkan = OsPlatformLib::OsLoadLibrary("vulkan-1", "");
#else
        _hVulkan = OsPlatformLib::OsLoadLibrary("vulkan", ".1");
#endif
    }
    /** @brief Destructor */
    ~VulkanApi()
    {
        if (_hVulkan)
            OsPlatformLib::OsUnloadLibrary(_hVulkan);

        _hVulkan = nullptr;
    }

    /**
    * @brief Template function to load function from vulkan library
    *
    * @param[in]  module	Library handle
    * @param[in]  fnName	Function name
    * @param[out] fnPtr		Function pointer
    *
    * @return true if loading function succeded
    */
    template<typename TFuncType>
    bool LoadFunction(osLibraryHandle module, const char* fnName, TFuncType& fnPtr)
    {
        fnPtr = reinterpret_cast<TFuncType>(OsPlatformLib::OsGetProcAddress(module, fnName));

        if (fnPtr == nullptr)
            std::cerr << "Could not load function: " << fnName << "!" << std::endl;

        return fnPtr != nullptr;
    }

    /**
    * @brief Template function to load function from vulkan library
    *
    * @param[in]  fnName	Function name
    * @param[out] fnPtr		Function pointer
    *
    * @return true if loading function succeded
    */
    template<typename TFuncType>
    bool LoadGlobalFunction(const char* fnName, TFuncType& fnPtr)
    {
        fnPtr = reinterpret_cast<TFuncType>(vkGetInstanceProcAddr(nullptr, fnName));

        if (fnPtr == nullptr)
            std::cout << "Could not global load function: " << fnName << "!" << std::endl;

        return fnPtr != nullptr;
    }

    /**
    * @brief Template function to load function from vulkan library
    *
    * @param[in]  fnName	Function name
    * @param[out] fnPtr		Function pointer
    *
    * @return true if loading function succeded
    */
    template<typename TFuncType>
    bool LoadInstanceFunction(VkInstance* pInstance, const char* fnName, TFuncType& fnPtr)
    {
        fnPtr = reinterpret_cast<TFuncType>(vkGetInstanceProcAddr(*pInstance, fnName));

        if (fnPtr == nullptr)
            std::cout << "Could not load instance function: " << fnName << "!" << std::endl;

        return fnPtr != nullptr;
    }

    /**
    * @brief Template function to load function from vulkan library
    *
    * @param[in]  fnName	Function name
    * @param[out] fnPtr		Function pointer
    *
    * @return true if loading function succeded
    */
    template<typename TFuncType>
    bool LoadDeviceFunction(VkDevice* pDevice, const char* fnName, TFuncType& fnPtr)
    {
        fnPtr = reinterpret_cast<TFuncType>(vkGetDeviceProcAddr(*pDevice, fnName));

        if (fnPtr == NULL)
            std::cout << "Could not load device function: " << fnName << "!" << std::endl;

        return fnPtr != NULL;
    }

public:

    /**
    * @brief factory for Vulkan API, this will load the ICD on first use
    */
    static VulkanApi* GetApi()
    {
        static VulkanApi api;
        return (api._hVulkan != 0) ? &api : nullptr;
    }

    /**
    * @brief Load globally accessible functions from vulkan lib
    *
    * @return true if loading succeded
    */
    bool LoadGlobalFunctions()
    {
        bool retValue = false;
        if (_hVulkan)
        {
            retValue = true;
            retValue &= LoadFunction(_hVulkan, "vkGetInstanceProcAddr", vkGetInstanceProcAddr);
            if (vkGetInstanceProcAddr)
            {
                retValue &= LoadGlobalFunction("vkEnumerateInstanceExtensionProperties", vkEnumerateInstanceExtensionProperties);
                retValue &= LoadGlobalFunction("vkEnumerateInstanceLayerProperties", vkEnumerateInstanceLayerProperties);
                retValue &= LoadGlobalFunction("vkCreateInstance", vkCreateInstance);
                // needs 1.1 loader -> don't error out if not available
                LoadGlobalFunction("vkEnumerateInstanceVersion", vkEnumerateInstanceVersion);
            }
        }

        return retValue;
    }

    /**
    * @brief Load instance level accessible functions from vulkan lib
    *
    * param[in] pInstance	Pointer to vulkan instance
    *
    * @return true if loading succeded
    */
    bool LoadInstanceFunctions(VkInstance* pInstance)
    {
        bool retValue = false;
        if (_hVulkan && pInstance)
        {
            retValue = true;
            retValue &= LoadInstanceFunction(pInstance, "vkDestroyInstance", vkDestroyInstance);
            retValue &= LoadInstanceFunction(pInstance, "vkEnumeratePhysicalDevices", vkEnumeratePhysicalDevices);
            retValue &= LoadInstanceFunction(pInstance, "vkGetPhysicalDeviceProperties", vkGetPhysicalDeviceProperties);
            retValue &= LoadInstanceFunction(pInstance, "vkGetPhysicalDeviceFeatures", vkGetPhysicalDeviceFeatures);
            retValue &= LoadInstanceFunction(pInstance, "vkGetPhysicalDeviceFormatProperties", vkGetPhysicalDeviceFormatProperties);
            retValue &= LoadInstanceFunction(pInstance, "vkGetPhysicalDeviceQueueFamilyProperties", vkGetPhysicalDeviceQueueFamilyProperties);
            retValue &= LoadInstanceFunction(pInstance, "vkCreateDevice", vkCreateDevice);
            retValue &= LoadInstanceFunction(pInstance, "vkGetDeviceProcAddr", vkGetDeviceProcAddr);
            retValue &= LoadInstanceFunction(pInstance, "vkEnumerateDeviceExtensionProperties", vkEnumerateDeviceExtensionProperties);
            retValue &= LoadInstanceFunction(pInstance, "vkGetPhysicalDeviceMemoryProperties", vkGetPhysicalDeviceMemoryProperties);
            retValue &= LoadInstanceFunction(pInstance, "vkGetDeviceQueue", vkGetDeviceQueue);
            retValue &= LoadInstanceFunction(pInstance, "vkDestroyDevice", vkDestroyDevice);
            retValue &= LoadInstanceFunction(pInstance, "vkDeviceWaitIdle", vkDeviceWaitIdle);

            retValue &= LoadInstanceFunction(pInstance, "vkDestroySurfaceKHR", vkDestroySurfaceKHR);
            retValue &= LoadInstanceFunction(pInstance, "vkGetPhysicalDeviceSurfaceSupportKHR", vkGetPhysicalDeviceSurfaceSupportKHR);
            retValue &= LoadInstanceFunction(pInstance, "vkGetPhysicalDeviceSurfaceCapabilitiesKHR", vkGetPhysicalDeviceSurfaceCapabilitiesKHR);
            retValue &= LoadInstanceFunction(pInstance, "vkGetPhysicalDeviceSurfaceFormatsKHR", vkGetPhysicalDeviceSurfaceFormatsKHR);
            retValue &= LoadInstanceFunction(pInstance, "vkGetPhysicalDeviceSurfacePresentModesKHR", vkGetPhysicalDeviceSurfacePresentModesKHR);
#if defined(VK_USE_PLATFORM_WIN32_KHR)
            retValue &= LoadInstanceFunction(pInstance, "vkCreateWin32SurfaceKHR", vkCreateWin32SurfaceKHR);
            retValue &= LoadInstanceFunction(pInstance, "vkGetPhysicalDeviceWin32PresentationSupportKHR", vkGetPhysicalDeviceWin32PresentationSupportKHR);
#elif defined(VK_USE_PLATFORM_XCB_KHR)
            retValue &= LoadInstanceFunction(pInstance, "vkCreateXcbSurfaceKHR", vkCreateXcbSurfaceKHR);
            retValue &= LoadInstanceFunction(pInstance, "vkGetPhysicalDeviceXcbPresentationSupportKHR", vkGetPhysicalDeviceXcbPresentationSupportKHR);
#endif
        }

        return retValue;
    }

    /**
    * @brief Load instance level accessible debug EXT functions from vulkan lib
    *
    * param[in] pInstance	Pointer to vulkan instance
    *
    * @return true if loading succeded
    */
    bool LoadInstanceDebugEXTFunctions(VkInstance* pInstance)
    {
        bool retValue = false;
        if (_hVulkan && pInstance)
        {
            retValue = true;
            retValue &= LoadInstanceFunction(pInstance, "vkCreateDebugReportCallbackEXT", vkCreateDebugReportCallbackEXT);
            retValue &= LoadInstanceFunction(pInstance, "vkDestroyDebugReportCallbackEXT", vkDestroyDebugReportCallbackEXT);
            retValue &= LoadInstanceFunction(pInstance, "vkDebugReportMessageEXT", vkDebugReportMessageEXT);
        }

        return retValue;
    }

    /**
    * @brief Load device level accessible functions from vulkan lib
    *
    * param[in] pDevice	Pointer to vulkan device
    *
    * @return true if loading succeded
    */
    bool LoadDeviceFunctions(VkDevice* pDevice, uint32_t apiVersion)
    {
        bool retValue = false;
        if (_hVulkan && pDevice)
        {
            retValue = true;
            retValue &= LoadDeviceFunction(pDevice, "vkAllocateMemory", vkAllocateMemory);
            retValue &= LoadDeviceFunction(pDevice, "vkFreeMemory", vkFreeMemory);
            retValue &= LoadDeviceFunction(pDevice, "vkCreateSemaphore", vkCreateSemaphore);
            retValue &= LoadDeviceFunction(pDevice, "vkDestroySemaphore", vkDestroySemaphore);
            retValue &= LoadDeviceFunction(pDevice, "vkCreateFence", vkCreateFence);
            retValue &= LoadDeviceFunction(pDevice, "vkDestroyFence", vkDestroyFence);
            retValue &= LoadDeviceFunction(pDevice, "vkResetFences", vkResetFences);
            retValue &= LoadDeviceFunction(pDevice, "vkGetFenceStatus", vkGetFenceStatus);
            retValue &= LoadDeviceFunction(pDevice, "vkWaitForFences", vkWaitForFences);
            retValue &= LoadDeviceFunction(pDevice, "vkCreateBuffer", vkCreateBuffer);
            retValue &= LoadDeviceFunction(pDevice, "vkDestroyBuffer", vkDestroyBuffer);
            retValue &= LoadDeviceFunction(pDevice, "vkBindBufferMemory", vkBindBufferMemory);
            retValue &= LoadDeviceFunction(pDevice, "vkBindImageMemory", vkBindImageMemory);
            retValue &= LoadDeviceFunction(pDevice, "vkMapMemory", vkMapMemory);
            retValue &= LoadDeviceFunction(pDevice, "vkUnmapMemory", vkUnmapMemory);
            retValue &= LoadDeviceFunction(pDevice, "vkFlushMappedMemoryRanges", vkFlushMappedMemoryRanges);
            retValue &= LoadDeviceFunction(pDevice, "vkQueueSubmit", vkQueueSubmit);
            retValue &= LoadDeviceFunction(pDevice, "vkQueueWaitIdle", vkQueueWaitIdle);
            retValue &= LoadDeviceFunction(pDevice, "vkGetBufferMemoryRequirements", vkGetBufferMemoryRequirements);
            retValue &= LoadDeviceFunction(pDevice, "vkGetImageMemoryRequirements", vkGetImageMemoryRequirements);
            retValue &= LoadDeviceFunction(pDevice, "vkCreateCommandPool", vkCreateCommandPool);
            retValue &= LoadDeviceFunction(pDevice, "vkAllocateCommandBuffers", vkAllocateCommandBuffers);
            retValue &= LoadDeviceFunction(pDevice, "vkDestroyCommandPool", vkDestroyCommandPool);
            retValue &= LoadDeviceFunction(pDevice, "vkBeginCommandBuffer", vkBeginCommandBuffer);
            retValue &= LoadDeviceFunction(pDevice, "vkEndCommandBuffer", vkEndCommandBuffer);
            retValue &= LoadDeviceFunction(pDevice, "vkResetCommandBuffer", vkResetCommandBuffer);
            retValue &= LoadDeviceFunction(pDevice, "vkFreeCommandBuffers", vkFreeCommandBuffers);
            retValue &= LoadDeviceFunction(pDevice, "vkCmdPipelineBarrier", vkCmdPipelineBarrier);
            retValue &= LoadDeviceFunction(pDevice, "vkCmdClearColorImage", vkCmdClearColorImage);
            retValue &= LoadDeviceFunction(pDevice, "vkCreateRenderPass", vkCreateRenderPass);
            retValue &= LoadDeviceFunction(pDevice, "vkDestroyRenderPass", vkDestroyRenderPass);
            retValue &= LoadDeviceFunction(pDevice, "vkCreateFramebuffer", vkCreateFramebuffer);
            retValue &= LoadDeviceFunction(pDevice, "vkDestroyFramebuffer", vkDestroyFramebuffer);
            retValue &= LoadDeviceFunction(pDevice, "vkCreateImage", vkCreateImage);
            retValue &= LoadDeviceFunction(pDevice, "vkDestroyImage", vkDestroyImage);
            retValue &= LoadDeviceFunction(pDevice, "vkGetImageSubresourceLayout", vkGetImageSubresourceLayout);
            retValue &= LoadDeviceFunction(pDevice, "vkCreateImageView", vkCreateImageView);
            retValue &= LoadDeviceFunction(pDevice, "vkDestroyImageView", vkDestroyImageView);
            retValue &= LoadDeviceFunction(pDevice, "vkCreateShaderModule", vkCreateShaderModule);
            retValue &= LoadDeviceFunction(pDevice, "vkDestroyShaderModule", vkDestroyShaderModule);
            retValue &= LoadDeviceFunction(pDevice, "vkCreatePipelineLayout", vkCreatePipelineLayout);
            retValue &= LoadDeviceFunction(pDevice, "vkDestroyPipelineLayout", vkDestroyPipelineLayout);
            retValue &= LoadDeviceFunction(pDevice, "vkCreateDescriptorSetLayout", vkCreateDescriptorSetLayout);
            retValue &= LoadDeviceFunction(pDevice, "vkDestroyDescriptorSetLayout", vkDestroyDescriptorSetLayout);
            retValue &= LoadDeviceFunction(pDevice, "vkCreateDescriptorPool", vkCreateDescriptorPool);
            retValue &= LoadDeviceFunction(pDevice, "vkDestroyDescriptorPool", vkDestroyDescriptorPool);
            retValue &= LoadDeviceFunction(pDevice, "vkResetDescriptorPool", vkResetDescriptorPool);
            retValue &= LoadDeviceFunction(pDevice, "vkAllocateDescriptorSets", vkAllocateDescriptorSets);
            retValue &= LoadDeviceFunction(pDevice, "vkFreeDescriptorSets", vkFreeDescriptorSets);
            retValue &= LoadDeviceFunction(pDevice, "vkUpdateDescriptorSets", vkUpdateDescriptorSets);
            retValue &= LoadDeviceFunction(pDevice, "vkCreateGraphicsPipelines", vkCreateGraphicsPipelines);
            retValue &= LoadDeviceFunction(pDevice, "vkCreateComputePipelines", vkCreateComputePipelines);
            retValue &= LoadDeviceFunction(pDevice, "vkDestroyPipeline", vkDestroyPipeline);
            retValue &= LoadDeviceFunction(pDevice, "vkCmdBindPipeline", vkCmdBindPipeline);
            retValue &= LoadDeviceFunction(pDevice, "vkCmdBindDescriptorSets", vkCmdBindDescriptorSets);
            retValue &= LoadDeviceFunction(pDevice, "vkCmdBindIndexBuffer", vkCmdBindIndexBuffer);
            retValue &= LoadDeviceFunction(pDevice, "vkCmdBindVertexBuffers", vkCmdBindVertexBuffers);
            retValue &= LoadDeviceFunction(pDevice, "vkCmdPushConstants", vkCmdPushConstants);
            retValue &= LoadDeviceFunction(pDevice, "vkCmdBeginRenderPass", vkCmdBeginRenderPass);
            retValue &= LoadDeviceFunction(pDevice, "vkCmdNextSubpass", vkCmdNextSubpass);
            retValue &= LoadDeviceFunction(pDevice, "vkCmdEndRenderPass", vkCmdEndRenderPass);
            retValue &= LoadDeviceFunction(pDevice, "vkCmdDraw", vkCmdDraw);
            retValue &= LoadDeviceFunction(pDevice, "vkCmdDrawIndexed", vkCmdDrawIndexed);
            retValue &= LoadDeviceFunction(pDevice, "vkCmdCopyBuffer", vkCmdCopyBuffer);
            retValue &= LoadDeviceFunction(pDevice, "vkCmdCopyImage", vkCmdCopyImage);
            retValue &= LoadDeviceFunction(pDevice, "vkCmdBlitImage", vkCmdBlitImage);
            retValue &= LoadDeviceFunction(pDevice, "vkCmdCopyImageToBuffer", vkCmdCopyImageToBuffer);
            retValue &= LoadDeviceFunction(pDevice, "vkCmdCopyBufferToImage", vkCmdCopyBufferToImage);

            retValue &= LoadDeviceFunction(pDevice, "vkCreateSwapchainKHR", vkCreateSwapchainKHR);
            retValue &= LoadDeviceFunction(pDevice, "vkDestroySwapchainKHR", vkDestroySwapchainKHR);
            retValue &= LoadDeviceFunction(pDevice, "vkGetSwapchainImagesKHR", vkGetSwapchainImagesKHR);
            retValue &= LoadDeviceFunction(pDevice, "vkAcquireNextImageKHR", vkAcquireNextImageKHR);
            retValue &= LoadDeviceFunction(pDevice, "vkQueuePresentKHR", vkQueuePresentKHR);

            if (apiVersion >= VK_MAKE_VERSION(1, 1, 0))
            {
                retValue &= LoadDeviceFunction(pDevice, "vkGetBufferMemoryRequirements2", vkGetBufferMemoryRequirements2);
            }
        }

        return retValue;
    }

private:
    osLibraryHandle _hVulkan;	///< Vulkan library handle

public:
    /** function pointers into Vulkan ICD
    * @{
    */

    // ************************************************************ //
    // Exported functions                                           //
    //                                                              //
    // These functions are always exposed by vulkan libraries.      //
    // ************************************************************ //
    vkGetInstanceProcAddrPtr	vkGetInstanceProcAddr;

    // ************************************************************ //
    // Global level functions                                       //
    //                                                              //
    // They allow checking what instance extensions are available   //
    // and allow creation of a Vulkan Instance.                     //
    // ************************************************************ // 
    vkEnumerateInstanceExtensionPropertiesPtr	vkEnumerateInstanceExtensionProperties;
    vkEnumerateInstanceLayerPropertiesPtr		vkEnumerateInstanceLayerProperties;
    vkCreateInstancePtr							vkCreateInstance;
    vkEnumerateInstanceVersionPtr				vkEnumerateInstanceVersion;

    // ************************************************************ //
    // Instance level functions                                     //
    //                                                              //
    // These functions allow for device queries and creation.       //
    // They help choose which device is well suited for our needs.  //
    // ************************************************************ // 
    vkDestroyInstancePtr						vkDestroyInstance;
    vkEnumeratePhysicalDevicesPtr				vkEnumeratePhysicalDevices;
    vkGetPhysicalDevicePropertiesPtr			vkGetPhysicalDeviceProperties;
    vkGetPhysicalDeviceFeaturesPtr				vkGetPhysicalDeviceFeatures;
    vkGetPhysicalDeviceFormatPropertiesPtr		vkGetPhysicalDeviceFormatProperties;
    vkGetPhysicalDeviceQueueFamilyPropertiesPtr	vkGetPhysicalDeviceQueueFamilyProperties;
    vkCreateDevicePtr							vkCreateDevice;
    vkGetDeviceProcAddrPtr						vkGetDeviceProcAddr;
    vkEnumerateDeviceExtensionPropertiesPtr		vkEnumerateDeviceExtensionProperties;
    vkGetPhysicalDeviceMemoryPropertiesPtr		vkGetPhysicalDeviceMemoryProperties;
    vkGetDeviceQueuePtr							vkGetDeviceQueue;
    vkDestroyDevicePtr							vkDestroyDevice;
    vkDeviceWaitIdlePtr							vkDeviceWaitIdle;
    vkCreateDebugReportCallbackEXTPtr			vkCreateDebugReportCallbackEXT;
    vkDestroyDebugReportCallbackEXTPtr			vkDestroyDebugReportCallbackEXT;
    vkDebugReportMessageEXTPtr					vkDebugReportMessageEXT;

    // ************************************************************ //
    // Instance level functions                                     //
    //                                                              //
    // Khronos extensions                                           //
    // ************************************************************ // 
    vkDestroySurfaceKHRPtr								vkDestroySurfaceKHR;
    vkGetPhysicalDeviceSurfaceSupportKHRPtr				vkGetPhysicalDeviceSurfaceSupportKHR;
    vkGetPhysicalDeviceSurfaceCapabilitiesKHRPtr		vkGetPhysicalDeviceSurfaceCapabilitiesKHR;
    vkGetPhysicalDeviceSurfaceFormatsKHRPtr				vkGetPhysicalDeviceSurfaceFormatsKHR;
    vkGetPhysicalDeviceSurfacePresentModesKHRPtr		vkGetPhysicalDeviceSurfacePresentModesKHR;
#if defined(VK_USE_PLATFORM_WIN32_KHR)
    vkCreateWin32SurfaceKHRPtr							vkCreateWin32SurfaceKHR;
    vkGetPhysicalDeviceWin32PresentationSupportKHRPtr	vkGetPhysicalDeviceWin32PresentationSupportKHR;
#elif defined(VK_USE_PLATFORM_XCB_KHR)
    vkCreateXcbSurfaceKHRPtr							vkCreateXcbSurfaceKHR;
    vkGetPhysicalDeviceXcbPresentationSupportKHRPtr		vkGetPhysicalDeviceXcbPresentationSupportKHR;
#endif

    // ************************************************************ //
    // Device level functions                                       //
    //                                                              //
    // These functions are used mainly for drawing                  //
    // ************************************************************ // 
    vkAllocateMemoryPtr							vkAllocateMemory;
    vkFreeMemoryPtr								vkFreeMemory;
    vkCreateSemaphorePtr						vkCreateSemaphore;
    vkDestroySemaphorePtr						vkDestroySemaphore;
    vkCreateFencePtr							vkCreateFence;
    vkDestroyFencePtr							vkDestroyFence;
    vkResetFencesPtr							vkResetFences;
    vkGetFenceStatusPtr							vkGetFenceStatus;
    vkWaitForFencesPtr							vkWaitForFences;
    vkCreateBufferPtr							vkCreateBuffer;
    vkDestroyBufferPtr							vkDestroyBuffer;
    vkBindBufferMemoryPtr						vkBindBufferMemory;
    vkBindImageMemoryPtr						vkBindImageMemory;
    vkMapMemoryPtr								vkMapMemory;
    vkUnmapMemoryPtr							vkUnmapMemory;
    vkFlushMappedMemoryRangesPtr				vkFlushMappedMemoryRanges;
    vkQueueSubmitPtr							vkQueueSubmit;
    vkQueueWaitIdlePtr							vkQueueWaitIdle;
    vkGetBufferMemoryRequirementsPtr			vkGetBufferMemoryRequirements;
    vkGetBufferMemoryRequirements2Ptr			vkGetBufferMemoryRequirements2;
    vkGetImageMemoryRequirementsPtr				vkGetImageMemoryRequirements;
    vkCreateCommandPoolPtr						vkCreateCommandPool;
    vkAllocateCommandBuffersPtr					vkAllocateCommandBuffers;
    vkDestroyCommandPoolPtr						vkDestroyCommandPool;
    vkBeginCommandBufferPtr						vkBeginCommandBuffer;
    vkEndCommandBufferPtr						vkEndCommandBuffer;
    vkResetCommandBufferPtr						vkResetCommandBuffer;
    vkFreeCommandBuffersPtr						vkFreeCommandBuffers;
    vkCmdPipelineBarrierPtr						vkCmdPipelineBarrier;
    vkCmdClearColorImagePtr						vkCmdClearColorImage;
    vkCreateRenderPassPtr						vkCreateRenderPass;
    vkDestroyRenderPassPtr						vkDestroyRenderPass;
    vkCreateFramebufferPtr						vkCreateFramebuffer;
    vkDestroyFramebufferPtr						vkDestroyFramebuffer;
    vkCreateImagePtr							vkCreateImage;
    vkDestroyImagePtr							vkDestroyImage;
    vkGetImageSubresourceLayoutPtr				vkGetImageSubresourceLayout;
    vkCreateImageViewPtr						vkCreateImageView;
    vkDestroyImageViewPtr						vkDestroyImageView;
    vkCreateShaderModulePtr						vkCreateShaderModule;
    vkDestroyShaderModulePtr					vkDestroyShaderModule;
    vkCreatePipelineLayoutPtr					vkCreatePipelineLayout;
    vkDestroyPipelineLayoutPtr					vkDestroyPipelineLayout;
    vkCreateDescriptorSetLayoutPtr				vkCreateDescriptorSetLayout;
    vkDestroyDescriptorSetLayoutPtr				vkDestroyDescriptorSetLayout;
    vkCreateDescriptorPoolPtr					vkCreateDescriptorPool;
    vkDestroyDescriptorPoolPtr					vkDestroyDescriptorPool;
    vkResetDescriptorPoolPtr					vkResetDescriptorPool;
    vkAllocateDescriptorSetsPtr					vkAllocateDescriptorSets;
    vkFreeDescriptorSetsPtr						vkFreeDescriptorSets;
    vkUpdateDescriptorSetsPtr					vkUpdateDescriptorSets;
    vkCreateGraphicsPipelinesPtr				vkCreateGraphicsPipelines;
    vkCreateComputePipelinesPtr					vkCreateComputePipelines;
    vkDestroyPipelinePtr						vkDestroyPipeline;
    vkCmdBindPipelinePtr						vkCmdBindPipeline;
    vkCmdBindDescriptorSetsPtr					vkCmdBindDescriptorSets;
    vkCmdBindIndexBufferPtr						vkCmdBindIndexBuffer;
    vkCmdBindVertexBuffersPtr					vkCmdBindVertexBuffers;
    vkCmdPushConstantsPtr						vkCmdPushConstants;
    vkCmdBeginRenderPassPtr						vkCmdBeginRenderPass;
    vkCmdNextSubpassPtr							vkCmdNextSubpass;
    vkCmdEndRenderPassPtr						vkCmdEndRenderPass;
    vkCmdDrawPtr								vkCmdDraw;
    vkCmdDrawIndexedPtr							vkCmdDrawIndexed;
    vkCmdCopyBufferPtr							vkCmdCopyBuffer;
    vkCmdCopyImagePtr							vkCmdCopyImage;
    vkCmdBlitImagePtr							vkCmdBlitImage;
    vkCmdCopyImageToBufferPtr					vkCmdCopyImageToBuffer;
    vkCmdCopyBufferToImagePtr                   vkCmdCopyBufferToImage;

    // ************************************************************ //
    // Device level functions                                       //
    //                                                              //
    // Khronos extensions                                           //
    // ************************************************************ // 
    vkCreateSwapchainKHRPtr						vkCreateSwapchainKHR;
    vkDestroySwapchainKHRPtr					vkDestroySwapchainKHR;
    vkGetSwapchainImagesKHRPtr					vkGetSwapchainImagesKHR;
    vkAcquireNextImageKHRPtr					vkAcquireNextImageKHR;
    vkQueuePresentKHRPtr						vkQueuePresentKHR;

};

}

/** @}*/
