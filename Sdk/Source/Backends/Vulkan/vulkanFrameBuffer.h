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

/// @file vulkanFrameBuffer.h
///       Vulkan device framebuffer

#include "halFrameBuffer.h"
#include "vulkanMemoryManager.h"

#include "Common/caveVector.h"

#include "vulkan.h"

/** \addtogroup backend
*  @{
*
*/

namespace cave
{

///< forwards
class VulkanRenderDevice;
class VulkanImageView;

/**
* @brief Vulkan device framebuffer
*/
class VulkanFrameBuffer : public HalFrameBuffer
{
public:
    /**
    * @brief Constructor
    *
    * @param[in] device             Pointer to render device object
    * @param[in] width              Framebuffer width
    * @param[in] height             Framebuffer height
    * @param[in] layers             Framebuffer layer count
    * @param[in] vkImageViews       Array of vkImageViews handles
    */
    VulkanFrameBuffer(VulkanRenderDevice* device, VkRenderPass renderPass,
        uint32_t width, uint32_t height, uint32_t layers, caveVector<VkImageView>& vkImageViews);

    /** @brief Destructor */
    virtual ~VulkanFrameBuffer();

    /**
    * @brief Get framebuffer object
    *
    * @return Vulkan framebuffer object
    */
    VkFramebuffer GetFrameBuffer() { return _vkFrameBuffer; }

private:
    VulkanRenderDevice* _pDevice;	///< Pointer to device object
    VkFramebuffer _vkFrameBuffer;   ///< Low level vulkan handle
};

}

/** @}*/




