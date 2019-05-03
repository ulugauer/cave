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
/// @file vulkanFence.h
///       vulkan fence abstraction

#include "halFence.h"
#include "osPlatformLib.h"

#include "vulkan.h"

/** \addtogroup backend
*  @{
*
*/

namespace cave
{

///< forwards
class VulkanRenderDevice;

/**
* @brief Handles GPU fence on the hardware level
*/
class VulkanFence : public HalFence
{
public:
    /**
    * @brief Constructor
    *
    * @param[in] device	    Pointer to device object
    * @param[in] fenceDesc	Fence creation description
    *
    */
    VulkanFence(VulkanRenderDevice* device, HalFenceDesc& fenceDesc);

    /** @brief Destructor */
    virtual ~VulkanFence();

    /**
    * @brief Get fence object
    *
    * @return Vulkan VkFence object
    */
    const VkFence& GetFence() const
    {
        return _vkFence;
    }

private:
    VulkanRenderDevice* _pDevice;	///< Pointer to device object
    VkFence _vkFence;	///< Low level fence handle
};

}

/** @}*/




