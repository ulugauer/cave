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

/// @file vulkanConversion.h
///       Hal types to vulkan types

/** \addtogroup backend
*  @{
*
*/

#include "halTypes.h"

#include "vulkan.h"

namespace cave
{

///< forwards
class VulkanRenderDevice;

/**
* @brief Image size info struct
*/
struct VulkanImageSizeInfo
{
    uint32_t _elementSize;      ///< size of a single texel
    uint32_t _blockSize;        ///< size of a single block valid only for compressed textures
    uint32_t _blockDimension;   ///< block dimension valid only for compressed textures
    bool _compressed;           ///< true for compressed formats
};

/**
* @brief This class converts Hal types to vulkan types
*/
class VulkanTypeConversion
{
public:

    /**
    * @brief Convert from generic pipeline bind point to vulkan pipeline bind point
    *
    * @param[in] pipelineBindPoint	Generic pipeline bind point
    *
    * @return Vulkan pipeline bind point
    */
    static VkPipelineBindPoint ConvertPipelineBindPointToVulkan(HalPipelineBindPoints pipelineBindPoint);

    /**
    * @brief Convert from generic image format to vulkan image format
    *
    * @param[in] imageFormat	Generic image format
    *
    * @return Vulkan image format
    */
    static VkFormat ConvertImageFormatToVulkan(HalImageFormat imageFormat);

    /**
    * @brief Convert from vulkan image format to generic image format
    *
    * @param[in] imageFormat	Vulkan image format
    *
    * @return Generic image format
    */
    static HalImageFormat ConvertImageFormatFromVulkan(VkFormat imageFormat);

    /**
    * @brief Convert from generic image view type to vulkan view type
    *
    * @param[in] imageViewType  Generic image view type
    *
    * @return Vulkan view type
    */
    static VkImageViewType ConvertImageViewTypeToVulkan(HalImageViewType imageViewType);

    /**
   * @brief Convert from vulkan image view type to generic image view type
   *
   * @param[in] imageViewType	Vulkan view type
   *
   * @return Generic image view type
   */
    static HalImageViewType ConvertImageViewTypeFromVulkan(VkImageViewType imageViewType);

    /**
    * @brief Query image size info for an image format
    *
    * @param[in] imageFormat	Vulkan image format
    *
    * @return VulkanImageSizeInfo struct
    */
    static VulkanImageSizeInfo GetImageSizeInfo(VkFormat imageFormat);

    /**
    * @brief Convert from generic image aspect flags to vulkan aspect flags
    *
    * @param[in] imageAspectFlags  Generic image aspect flags
    *
    * @return Vulkan aspect flags
    */
    static VkImageAspectFlags ConvertImageAspectFlagsToVulkan(HalImageAspectFlags imageAspectFlags);

    /**
   * @brief Convert from vulkan image aspect flags to generic image aspect flags
   *
   * @param[in] imageAspectFlags	Vulkan aspect flags
   *
   * @return Generic image aspect flags
   */
    static HalImageAspectFlags ConvertImageAspectFlagsFormatFromVulkan(VkImageAspectFlags imageAspectFlags);

    /**
    * @brief Convert from generic component swizzle to vulkan component swizzle
    *
    * @param[in] swizzle  Generic component swizzle
    *
    * @return Vulkan component swizzle
    */
    static VkComponentSwizzle ConvertComponentSwizzleToVulkan(HalComponentSwizzle swizzle);

    /**
    * @brief Convert from vulkan component swizzle to generic component swizzle
    *
    * @param[in] swizzle  Generic component swizzle
    *
    * @return Vulkan component swizzle
    */
    static HalComponentSwizzle ConvertComponentSwizzleFromVulkan(VkComponentSwizzle swizzle);

    /**
    * @brief Convert from generic component mapping to vulkan component mapping
    *
    * @param[in] components  Generic component mapping
    *
    * @return Vulkan component mapping
    */
    static VkComponentMapping ConvertComponentMappingToVulkan(HalComponentMapping components);

    /**
   * @brief Convert from vulkan component mapping to generic component mapping
   *
   * @param[in] components	Vulkan component mapping
   *
   * @return Generic component mapping
   */
    static HalComponentMapping ConvertComponentMappingFromVulkan(VkComponentMapping components);

    /**
    * @brief Convert from generic sample count to vulkan sample count
    *
    * @param[in] sampleCount	Generic sample count
    *
    * @return Vulkan sample count flags
    */
    static VkSampleCountFlagBits ConvertSampleCountToVulkan(HalSampleCount sampleCount);

    /**
    * @brief Convert from generic cull mode to vulkan cull mode
    *
    * @param[in] mode	Generic cull mode
    *
    * @return Vulkan cull mode
    */
    static VkCullModeFlagBits ConvertCullModeToVulkan(HalCullMode mode);

    /**
    * @brief Convert from generic front face to vulkan front face
    *
    * @param[in] face	Generic front face value
    *
    * @return Vulkan front face
    */
    static VkFrontFace ConvertFrontFaceToVulkan(HalFrontFace face);

    /**
    * @brief Convert from generic polygon mode to vulkan polygon mode
    *
    * @param[in] mode	Generic polygon mode
    *
    * @return Vulkan polygon mode
    */
    static VkPolygonMode ConvertPolygonModeToVulkan(HalPolygonMode mode);

    /**
    * @brief Convert from generic stencil operation to vulkan stencil operation
    *
    * @param[in] op	Generic stencil operation
    *
    * @return Vulkan stencil operation
    */
    static VkStencilOp ConvertStencilOpToVulkan(HalStencilOp op);

    /**
    * @brief Convert from generic compare operation to vulkan compare operation
    *
    * @param[in] compOp	Generic compare operation
    *
    * @return Vulkan compare operation
    */
    static VkCompareOp ConvertDepthCompareOpToVulkan(HalCompareOp compOp);

    /**
    * @brief Convert from generic blend factor to vulkan blend factor
    *
    * @param[in] factor	Generic blend factor
    *
    * @return Vulkan blend factor
    */
    static VkBlendFactor ConvertBlendFactorToVulkan(HalBlendFactor factor);

    /**
    * @brief Convert from generic logical operation to vulkan logical operationr
    *
    * @param[in] op		Generic logical operation
    *
    * @return Vulkan logical operation
    */
    static VkLogicOp ConvertLogicOpToVulkan(HalLogicOp op);

    /**
    * @brief Convert from generic blend operation to vulkan blend operationr
    *
    * @param[in] op		Generic blend operation
    *
    * @return Vulkan blend operation
    */
    static VkBlendOp ConvertBlendOpToVulkan(HalBlendOp op);

    /**
    * @brief Convert from generic color components to vulkan color components
    *
    * @param[in] colorComponentFlags	Generic color components
    *
    * @return Vulkan color components
    */
    static VkColorComponentFlags ConvertColorComponentToVulkan(HalColorComponentFlags colorComponentFlags);

    /**
    * @brief Convert from generic dynamic state to vulkan bdynamic state
    *
    * @param[in] state		Generic dynamic state
    *
    * @return Vulkan dynamic state value
    */
    static VkDynamicState ConvertDynamicStateToVulkan(HalDynamicStates state);

    /**
    * @brief Convert from generic shader stage flags to vulkan blend shader stage flags
    *
    * @param[in] flags		Generic shader stage flags
    *
    * @return Vulkan shader stage flags
    */
    static VkShaderStageFlags ConvertShaderStagesToVulkan(HalShaderStagesFlags flags);

    /**
    * @brief Convert from generic attachment load op to vulkan load op
    *
    * @param[in] loadOp		Generic attachment load operation
    *
    * @return Vulkan attachment load operation
    */
    static VkAttachmentLoadOp ConvertAttachmentLoadOpToVulkan(HalAttachmentLoadOperation loadOp);

    /**
    * @brief Convert from generic attachment store op to vulkan store op
    *
    * @param[in] storeOp	Generic attachment store operation
    *
    * @return Vulkan attachment store operation
    */
    static VkAttachmentStoreOp ConvertAttachmentStoreOpToVulkan(HalAttachmentStoreOperation storeOp);

    /**
    * @brief Convert from generic image layout to vulkan image layouyt
    *
    * @param[in] imageLayout	Generic image layout
    *
    * @return Vulkan image layout
    */
    static VkImageLayout ConvertImageLayoutToVulkan(HalImageLayout imageLayout);

    /**
    * @brief Convert from generic command pool usage to vulkan command pool usage
    *
    * @param[in] usageFlags	Generic command pool usage flags
    *
    * @return Vulkan command pool create flags
    */
    static VkCommandPoolCreateFlags ConvertCommandPoolUsageFlagsToVulkan(HalCommandPoolUsageFlags usageFlags);

    /**
    * @brief Convert from generic command buffer level to vulkan command buffer leve
    *
    * @param[in] level	Generic command uffer level
    *
    * @return Vulkan commnad buffer level
    */
    static VkCommandBufferLevel ConvertCommandBufferLevelToVulkan(HalCommandBufferLevel level);

    /**
    * @brief Convert from generic command buffer usage to vulkan command buffer usage
    *
    * @param[in] usageFlags	Generic command buffer usage flags
    *
    * @return Vulkan command buffer usage flags
    */
    static VkCommandBufferUsageFlags ConvertCommandBufferUsageFlagsToVulkan(HalCommandBufferUsageFlags usageFlags);

    /**
    * @brief Convert from generic subpass command content to vulkan subpass command content
    *
    * @param[in] content	Generic subpass command content
    *
    * @return Vulkan subpass command content
    */
    static VkSubpassContents ConvertSubpassContentToVulkan(HalSubpassContents content);

    /**
    * @brief Convert from generic dependency flags to vulkan dependency flags
    *
    * @param[in] dependency		Generic dependency flags
    *
    * @return Vulkan VkDependencyFlags
    */
    static VkDependencyFlags ConvertDependencyFlagsToVulkan(HalDependencyFlags dependency);

    /**
    * @brief Convert from generic pipeline stage flags to vulkan dependency flags
    *
    * @param[in] pipelineStage		Generic pipeline stage flags
    *
    * @return Vulkan VkPipelineStageFlags
    */
    static VkPipelineStageFlags ConvertPipelineFlagsToVulkan(HalPipelineStageFlags pipelineStage);

    /**
    * @brief Convert from generic access flags to vulkan access flags
    *
    * @param[in] access		Generic access flags
    *
    * @return Vulkan VkAccessFlags
    */
    static VkAccessFlags ConvertAccessFlagsToVulkan(HalAccessFlags access);

    /**
    * @brief Convert from generic create flags to vulkan crweate flags
    *
    * @param[in] create		Generic create flags
    *
    * @return Vulkan VkBufferCreateFlags
    */
    static VkBufferCreateFlags ConvertBufferCreateFlagsToVulkan(HalBufferCreateFlags create);

    /**
    * @brief Convert from generic usage flags to vulkan usage flags
    *
    * @param[in] usage		Generic usage flags
    *
    * @return Vulkan VkBufferUsageFlags
    */
    static VkBufferUsageFlags ConvertBufferUsageFlagsToVulkan(HalBufferUsageFlags usage);

    /**
    * @brief Convert from generic shared mode to vulkan shared mode
    *
    * @param[in] mode		Generic shared mode
    *
    * @return Vulkan VkSharingMode
    */
    static VkSharingMode ConvertSharedModeFlagsToVulkan(HalBufferShareMode mode);

    /**
    * @brief Convert from generic vertex input rate to vulkan vertex input rate
    *
    * @param[in] rate		Generic vertex input rate
    *
    * @return Vulkan VkVertexInputRate
    */
    static VkVertexInputRate ConvertVertexInputRateToVulkan(HalVertexInputRate rate);

    /**
    * @brief Convert from generic memory properties to vulkan memory properties
    *
    * @param[in] properties		Generic memory properties
    *
    * @return Vulkan VkMemoryPropertyFlags
    */
    static VkMemoryPropertyFlags ConvertMemoryPropertiesToVulkan(HalMemoryPropertyFlags properties);

    /**
    * @brief Convert from generic index type to vulkan index type
    *
    * @param[in] indexType		Generic index type
    *
    * @return Vulkan VkIndexType
    */
    static VkIndexType ConvertIndexTypeToVulkan(HalIndexType indexType);

    /**
    * @brief Convert from generic primitive topology to vulkan primitive topology
    *
    * @param[in] topology		Generic primitive topology
    *
    * @return Vulkan VkPrimitiveTopology
    */
    static VkPrimitiveTopology ConvertPrimitiveTopologyToVulkan(HalPrimitiveTopology topology);

    /**
    * @brief Convert from generic descriptor type to vulkan descriptor type
    *
    * @param[in] descriptorType		Generic descriptor type
    *
    * @return Vulkan VkDescriptorType
    */
    static VkDescriptorType ConvertDescriptorTypeToVulkan(HalDescriptorType descriptorType);

    /**
    * @brief Convert from generic descriptor pool create flags to vulkan descriptor pool create flags
    *
    * @param[in] poolFlags		Generic descriptor pool create flags
    *
    * @return Vulkan VkDescriptorPoolCreateFlags
    */
    static VkDescriptorPoolCreateFlags ConvertDescriptorPoolFlagsToVulkan(HalDescriptorPoolCreateFlags poolFlags);
};

}

/** @}*/

