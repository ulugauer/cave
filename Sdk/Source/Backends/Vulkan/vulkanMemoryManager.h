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

/// @file vulkanMemoryManager.h
///       Manage device memory

#include "halTypes.h"
#include "osPlatformLib.h"
#include "Common/caveList.h"
#include "Common/caveVector.h"

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
* Vulkan device memory allocation
*/
struct VulkanDeviceMemory
{
    VulkanDeviceMemory()
    {
        _offset = _size = 0;
        _vkDeviceMemory = VK_NULL_HANDLE;
        _memoryTypeIndex = 0;
        _needsFlush = false;
        _mappedAddress = nullptr;
    }

    uint64_t _offset;	///< Memory offset
    uint64_t _size;		///< Actual allocated size
    uint32_t _memoryTypeIndex; ///< type of memory
    VkDeviceMemory _vkDeviceMemory;		///< Vulkan device memory handle
    bool _needsFlush;		///< does this memory need a flush before usage
    void* _mappedAddress;	///< Pointer to virtual memory if mapped
};

/**
* Vulkan device memory page allocation
* This represents a page entry from where
* we do sub-allocations
*/
struct VulkanDeviceMemoryPageEntry
{
    VulkanDeviceMemoryPageEntry()
    {
        _allocationCount = 0;
        _vkBuffer = VK_NULL_HANDLE;
    }

    uint64_t _allocationCount;
    VulkanDeviceMemory _deviceMemory;
    VkBuffer _vkBuffer;	///< buffer for this page
};

/**
* Vulkan device memory allocation
*/
struct VulkanStagingBufferInfo
{
    /** @brief constructor */
    VulkanStagingBufferInfo()
    {
        _stagingBuffer = VK_NULL_HANDLE;
    }

    /**
    * @brief Query memory offset
    *
    * @return memory offset
    */
    uint64_t GetOffset() const { return _statgingMemory._offset; }

    /**
    * @brief Query memory size
    *
    * @return memory size
    */
    uint64_t GetSize() const { return _statgingMemory._size; }

    /**
    * @brief Get device handle
    *
    * @return vulkan device handle
    */
    VkDeviceMemory GetDeviceHandle() const { return _statgingMemory._vkDeviceMemory; }

    /**
    * @brief Get mapped device memory address
    *
    * @return mapped address
    */
    void* GetMappedAddress() const { return _statgingMemory._mappedAddress; }

    /**
    * @brief Query if the memory requires a memory flush sync
    *
    * @return true if flush is required
    */
    bool NeedsFlush() const { return _statgingMemory._needsFlush; }

    VkBuffer _stagingBuffer;	///< Handled to staging buffer
    VulkanDeviceMemory _statgingMemory; ///< Memory  associated with the buffer
};

/**
* Vulkan device memory manager
*/
class VulkanMemoryManager
{
public:
    /**
    * @brief Constructor
    *
    * @param[in] instance			Pointer to instance object
    * @param[in] physicalDevice		Pointer to physical device
    * @param[in] renderDevice		Pointer to logical device
    *
    */
    VulkanMemoryManager(VulkanInstance* instance, VulkanPhysicalDevice* physicalDevice, VulkanRenderDevice* renderDevice);
    /** @brief Destructor */
    virtual ~VulkanMemoryManager();

    static constexpr uint64_t StagingBufferSize = 8388608;	///< 8 MB

    /**
    * @brief Allocate device memory for any buffer usage
    *
    * @param[in] memRequirements	VkMemoryRequirements struct
    * @param[in] properties			VkMemoryPropertyFlags required memory properties
    * @param[out] deviceMemory		Filled in VulkanDeviceMemory struct on success
    *
    */
    void AllocateBufferMemory(VkMemoryRequirements& memRequirements, VkMemoryPropertyFlags properties, VulkanDeviceMemory& deviceMemory);

    /**
    * @brief Release device memory back to system
    *
    * @param[in] deviceMemory		VulkanDeviceMemory struct returned on AllocateBufferMemory call
    *
    */
    void ReleaseBufferMemory(VulkanDeviceMemory& deviceMemory);

    /**
    * @brief Allocate device memory for any image usage
    *
    * @param[in] memRequirements	VkMemoryRequirements struct
    * @param[in] properties			VkMemoryPropertyFlags required memory properties
    * @param[out] deviceMemory		Filled in VulkanDeviceMemory struct on success
    *
    */
    void AllocateImageMemory(VkMemoryRequirements& memRequirements, VkMemoryPropertyFlags properties, VulkanDeviceMemory& deviceMemory);

    /**
    * @brief Release device memory back to system
    *
    * @param[in] deviceMemory		VulkanDeviceMemory struct returned on AllocateBufferMemory call
    *
    */
    void ReleaseImageMemory(VulkanDeviceMemory& deviceMemory);

    /**
    * @brief Allocate host visible staging buffer
    *
    * @param[in] size				Required Size
    * @param[out] stagingBufferInfo	Filled in VulkanStagingBuffer struct on success
    *
    */
    void GetStagingBuffer(uint64_t size, VulkanStagingBufferInfo& stagingBufferInfo);

    /**
    * @brief Flush host visible memory
    *
    * @param[in] deviceMemory	VulkanDeviceMemory struct returned on AllocateBufferMemory call
    *
    */
    void FlushStagingMemory(VulkanDeviceMemory& deviceMemory);

    /**
    * @brief Copy host visible memory to device memory
    *
    * @param[in] srcBuffer	VkBuffer source handle
    * @param[in] dstBuffer	VkBuffer dest handle
    * @param[in] srcOffset	Source offset in bytes
    * @param[in] dstOffset	Dest offset in bytes
    * @param[in] size		Copy size in bytes
    *
    */
    void CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, uint64_t srcOffset, uint64_t dstOffset, uint64_t size);

    /**
    * @brief Copy buffer memory to image memory
    *
    * @param[in] srcBuffer	VkBuffer source handle
    * @param[in] dstBuffer	VkBuffer dest handle
    * @param[in] regions	Regions array to copy
    *
    */
    void CopyBufferToImage(VkBuffer srcBuffer, VkImage dstImage, caveVector<VkBufferImageCopy>& regions);

    /**
    * @brief Submit possibly scheduled copies
    *
    *
    */
    void SubmitCopies();

    /**
    * @brief Wait for possibly scheduled copies
    *
    *
    */
    void WaitForCopies();

    /**
    * @brief Select memory type from property flags
    *
    * @param[in] memRequirements	VkBuffer memory requilrements
    * @param[in] properties			Memory property requirement
    *
    * return index into memory type array
    */
    uint32_t ChooseMemoryType(VkMemoryRequirements& memRequirements, VkMemoryPropertyFlags properties);

private:

    /**
    * @brief Allocate host visible new page for sub-allocations
    * Note: We keep always one page arround for temp copies.
    * On a per frame basis we free temp allocations and remove additional pages
    *
    * @param[in] pageSize	Allocation size
    *
    */
    void AllocateStagingPage(uint64_t pageSize);

    /**
    * @brief Allocate host visible memory for memory copy
    *
    * @param[in] memRequirements	VkMemoryRequirements struct
    * @param[out] deviceMemory		Filled in VulkanDeviceMemory struct on success
    *
    */
    void AllocateStagingMemory(VkMemoryRequirements& memRequirements, VulkanDeviceMemory& deviceMemory);

    /**
    * @brief Release staging memory back to system
    *
    * @param[in] deviceMemory		VulkanDeviceMemory struct returned on AllocateStagingMemory call
    *
    */
    void ReleaseStagingMemory(VulkanDeviceMemory& deviceMemory);

    /**
    * @brief Find a suitable and available staging buffer
    *
    * @param[in] size			Required buffer size
    * @param[out] memoryInfo	Returned memory info required
    *
    * return Vulkan buffer handle
    */
    VkBuffer FindStagingBuffer(uint64_t size, VulkanDeviceMemory& memoryInfo);

private:
    VulkanInstance* _pInstance;	///< Pointer to instance object
    VulkanPhysicalDevice* _pPhysicalDevice;	///< Pointer to physical device
    VulkanRenderDevice* _pRenderDevice;	///< Pointer to logical device
    uint64_t _nonCoherentAlignment;	///< Minimum alignment for non-coherent memory
    VkCommandPool _vkCommandPool;	///< Vulkan command pool handle
    VkCommandBuffer _vkTransferCommandBuffer;	///< Vulkan command buffer for data transfers
    VkCommandBuffer _vkImageTransferCommandBuffer;	///< Vulkan command buffer for image transfers
    CaveList<VulkanDeviceMemoryPageEntry> _bufferMemoryPages;	///< Memory pages allocated for bufferse
    VkFence _vkCopyFence;	///< Fence used to wait for submited buffer copies
    VkFence _vkCopyImageFence;	///< Fence used to wait for submited image copies
    CaveList<VulkanDeviceMemoryPageEntry> _stagingMemoryPages;	///< Memory pages allocated for staging operations
    size_t _copyCount;	///< keep track of submitted buffer copies
    size_t _copyWaitCount;	///< keep track if we need to wait for buffer copies
    size_t _copyImageCount;	///< keep track of submitted image copies
    size_t _copyImageWaitCount;	///< keep track if we need to wait for image copies
};

}

/** @}*/

