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

/// @file vulkanConversion.cpp
///       Hal types to vulkan types

#include "vulkanConversion.h"
#include "vulkanRenderDevice.h"

#include "vulkanApi.h"

#include<limits>
#include<cassert>

namespace cave
{

VkPipelineBindPoint
VulkanTypeConversion::ConvertPipelineBindPointToVulkan(HalPipelineBindPoints pipelineBindPoint)
{
    VkPipelineBindPoint vkBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;

    switch (pipelineBindPoint)
    {
    case HalPipelineBindPoints::Graphics:
        vkBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        break;
    case HalPipelineBindPoints::Compute:
        vkBindPoint = VK_PIPELINE_BIND_POINT_COMPUTE;
        break;
    default:
        vkBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        assert(false);
        break;
    }

    return vkBindPoint;
}

VkFormat
VulkanTypeConversion::ConvertImageFormatToVulkan(HalImageFormat imageFormat)
{
    VkFormat vkImageFormat = VK_FORMAT_UNDEFINED;

    switch (imageFormat)
    {
    case HalImageFormat::R8G8B8A8UNorm:
        vkImageFormat = VK_FORMAT_R8G8B8A8_UNORM;
        break;
    case HalImageFormat::R8G8B8A8SNorm:
        vkImageFormat = VK_FORMAT_R8G8B8A8_SNORM;
        break;
    case HalImageFormat::B8G8R8A8UNorm:
        vkImageFormat = VK_FORMAT_B8G8R8A8_UNORM;
        break;
    case HalImageFormat::B8G8R8A8SNorm:
        vkImageFormat = VK_FORMAT_B8G8R8A8_SRGB;
        break;
    case HalImageFormat::R16G16B16A16SFloat:
        vkImageFormat = VK_FORMAT_R16G16B16A16_SFLOAT;
        break;
    case HalImageFormat::R32G32SFloat:
        vkImageFormat = VK_FORMAT_R32G32_SFLOAT;
        break;
    case HalImageFormat::R32G32B32SFloat:
        vkImageFormat = VK_FORMAT_R32G32B32_SFLOAT;
        break;
    case HalImageFormat::R32G32B32A32SFloat:
        vkImageFormat = VK_FORMAT_R32G32B32A32_SFLOAT;
        break;
    case HalImageFormat::D32SFloat:
        vkImageFormat = VK_FORMAT_D32_SFLOAT;
        break;
    case HalImageFormat::D24UnormS8Uint:
        vkImageFormat = VK_FORMAT_D24_UNORM_S8_UINT;
        break;
    case HalImageFormat::D32SFloatS8Uint:
        vkImageFormat = VK_FORMAT_D32_SFLOAT_S8_UINT;
        break;
    case HalImageFormat::BC1RgbUnorm:
        vkImageFormat = VK_FORMAT_BC1_RGB_UNORM_BLOCK;
        break;
    case HalImageFormat::BC1RgbSrgb:
        vkImageFormat = VK_FORMAT_BC1_RGB_SRGB_BLOCK;
        break;
    case HalImageFormat::BC1RgbaUnorm:
        vkImageFormat = VK_FORMAT_BC1_RGBA_UNORM_BLOCK;
        break;
    case HalImageFormat::BC1RgbaSrgb:
        vkImageFormat = VK_FORMAT_BC1_RGBA_SRGB_BLOCK;
        break;
    case HalImageFormat::BC2Unorm:
        vkImageFormat = VK_FORMAT_BC2_UNORM_BLOCK;
        break;
    case HalImageFormat::BC2Srgb:
        vkImageFormat = VK_FORMAT_BC2_SRGB_BLOCK;
        break;
    case HalImageFormat::BC3Unorm:
        vkImageFormat = VK_FORMAT_BC3_UNORM_BLOCK;
        break;
    case HalImageFormat::BC3Srgb:
        vkImageFormat = VK_FORMAT_BC3_SRGB_BLOCK;
        break;
    case HalImageFormat::BC4Unorm:
        vkImageFormat = VK_FORMAT_BC4_UNORM_BLOCK;
        break;
    case HalImageFormat::BC4Snorm:
        vkImageFormat = VK_FORMAT_BC4_SNORM_BLOCK;
        break;
    case HalImageFormat::BC5Unorm:
        vkImageFormat = VK_FORMAT_BC5_UNORM_BLOCK;
        break;
    case HalImageFormat::BC5Snorm:
        vkImageFormat = VK_FORMAT_BC5_SNORM_BLOCK;
        break;
    case HalImageFormat::BC7Unorm:
        vkImageFormat = VK_FORMAT_BC7_UNORM_BLOCK;
        break;
    case HalImageFormat::BC7Srgb:
        vkImageFormat = VK_FORMAT_BC7_SRGB_BLOCK;
        break;
    default:
        vkImageFormat = VK_FORMAT_UNDEFINED;
        assert(false);
        break;
    }

    return vkImageFormat;
}

HalImageFormat
VulkanTypeConversion::ConvertImageFormatFromVulkan(VkFormat imageFormat)
{
    HalImageFormat format = HalImageFormat::Undefined;

    switch (imageFormat)
    {
    case VK_FORMAT_R8G8B8A8_UNORM:
        format = HalImageFormat::R8G8B8A8UNorm;
        break;
    case VK_FORMAT_R8G8B8A8_SNORM:
        format = HalImageFormat::R8G8B8A8SNorm;
        break;
    case VK_FORMAT_B8G8R8A8_UNORM:
        format = HalImageFormat::B8G8R8A8UNorm;
        break;
    case VK_FORMAT_B8G8R8A8_SRGB:
        format = HalImageFormat::B8G8R8A8SNorm;
        break;
    case VK_FORMAT_R16G16B16A16_SFLOAT:
        format = HalImageFormat::R16G16B16A16SFloat;
        break;
    case VK_FORMAT_R32G32_SFLOAT:
        format = HalImageFormat::R32G32SFloat;
        break;
    case VK_FORMAT_R32G32B32_SFLOAT:
        format = HalImageFormat::R32G32B32SFloat;
        break;
    case VK_FORMAT_R32G32B32A32_SFLOAT:
        format = HalImageFormat::R32G32B32A32SFloat;
        break;
    case VK_FORMAT_D32_SFLOAT:
        format = HalImageFormat::D32SFloat;
        break;
    case VK_FORMAT_D24_UNORM_S8_UINT:
        format = HalImageFormat::D24UnormS8Uint;
        break;
    case VK_FORMAT_D32_SFLOAT_S8_UINT:
        format = HalImageFormat::D32SFloatS8Uint;
        break;
    case VK_FORMAT_BC1_RGB_UNORM_BLOCK:
        format = HalImageFormat::BC1RgbUnorm;
        break;
    case VK_FORMAT_BC1_RGB_SRGB_BLOCK:
        format = HalImageFormat::BC1RgbSrgb;
        break;
    case VK_FORMAT_BC1_RGBA_UNORM_BLOCK:
        format = HalImageFormat::BC1RgbaUnorm;
        break;
    case VK_FORMAT_BC1_RGBA_SRGB_BLOCK:
        format = HalImageFormat::BC1RgbaSrgb;
        break;
    case VK_FORMAT_BC2_UNORM_BLOCK:
        format = HalImageFormat::BC2Unorm;
        break;
    case VK_FORMAT_BC2_SRGB_BLOCK:
        format = HalImageFormat::BC2Srgb;
        break;
    case VK_FORMAT_BC3_UNORM_BLOCK:
        format = HalImageFormat::BC3Unorm;
        break;
    case VK_FORMAT_BC3_SRGB_BLOCK:
        format = HalImageFormat::BC3Srgb;
        break;
    case VK_FORMAT_BC4_UNORM_BLOCK:
        format = HalImageFormat::BC4Unorm;
        break;
    case VK_FORMAT_BC4_SNORM_BLOCK:
        format = HalImageFormat::BC4Snorm;
        break;
    case VK_FORMAT_BC5_UNORM_BLOCK:
        format = HalImageFormat::BC5Unorm;
        break;
    case VK_FORMAT_BC5_SNORM_BLOCK:
        format = HalImageFormat::BC5Snorm;
        break;
    case VK_FORMAT_BC7_UNORM_BLOCK:
        format = HalImageFormat::BC7Unorm;
        break;
    case VK_FORMAT_BC7_SRGB_BLOCK:
        format = HalImageFormat::BC7Srgb;
        break;
    default:
        format = HalImageFormat::Undefined;
        assert(false);
        break;
    }

    return format;
}

VkImageType 
VulkanTypeConversion::ConvertImageTypeToVulkan(HalImageType imageType)
{
    VkImageType vkImageType = VK_IMAGE_TYPE_1D;

    switch (imageType)
    {
    case HalImageType::Image1D:
        vkImageType = VK_IMAGE_TYPE_1D;
        break;
    case HalImageType::Image2D:
        vkImageType = VK_IMAGE_TYPE_2D;
        break;
    case HalImageType::Image3D:
        vkImageType = VK_IMAGE_TYPE_3D;
        break;
    default:
        assert(false);
        break;
    }

    return vkImageType;
}

HalImageType
VulkanTypeConversion::ConvertImageTypeFromVulkan(VkImageType imageType)
{
    HalImageType halImageType = HalImageType::Image1D;

    switch (imageType)
    {
    case VK_IMAGE_TYPE_1D:
        halImageType = HalImageType::Image1D;
        break;
    case VK_IMAGE_TYPE_2D:
        halImageType = HalImageType::Image2D;
        break;
    case VK_IMAGE_TYPE_3D:
        halImageType = HalImageType::Image3D;
        break;
    default:
        assert(false);
        break;
    }

    return halImageType;
}

VkImageViewType 
VulkanTypeConversion::ConvertImageViewTypeToVulkan(HalImageViewType imageViewType)
{
    VkImageViewType viewType = VK_IMAGE_VIEW_TYPE_1D;

    switch (imageViewType)
    {
    case HalImageViewType::Image1D:
        viewType = VK_IMAGE_VIEW_TYPE_1D;
        break;
    case HalImageViewType::Image2D:
        viewType = VK_IMAGE_VIEW_TYPE_2D;
        break;
    case HalImageViewType::Image3D:
        viewType = VK_IMAGE_VIEW_TYPE_3D;
        break;
    case HalImageViewType::ImageCube:
        viewType = VK_IMAGE_VIEW_TYPE_CUBE;
        break;
    case HalImageViewType::Image1DArray:
        viewType = VK_IMAGE_VIEW_TYPE_1D_ARRAY;
        break;
    case HalImageViewType::Image2DArray:
        viewType = VK_IMAGE_VIEW_TYPE_2D_ARRAY;
        break;
    case HalImageViewType::ImageCubeArray:
        viewType = VK_IMAGE_VIEW_TYPE_CUBE_ARRAY;
        break;
    default:
        assert(false);
        break;
    }

    return viewType;
}

HalImageViewType 
VulkanTypeConversion::ConvertImageViewTypeFromVulkan(VkImageViewType imageViewType)
{
    HalImageViewType viewType = HalImageViewType::Image2D;

    switch (imageViewType)
    {
    case VK_IMAGE_VIEW_TYPE_1D:
        viewType = HalImageViewType::Image1D;
        break;
    case VK_IMAGE_VIEW_TYPE_2D:
        viewType = HalImageViewType::Image2D;
        break;
    case VK_IMAGE_VIEW_TYPE_3D:
        viewType = HalImageViewType::Image3D;
        break;
    case VK_IMAGE_VIEW_TYPE_CUBE:
        viewType = HalImageViewType::ImageCube;
        break;
    case VK_IMAGE_VIEW_TYPE_1D_ARRAY:
        viewType = HalImageViewType::Image1DArray;
        break;
    case VK_IMAGE_VIEW_TYPE_2D_ARRAY:
        viewType = HalImageViewType::Image2DArray;
        break;
    case VK_IMAGE_VIEW_TYPE_CUBE_ARRAY:
        viewType = HalImageViewType::ImageCubeArray;
        break;
    default:
        assert(false);
        break;
    }

    return viewType;
}

VulkanImageSizeInfo 
VulkanTypeConversion::GetImageSizeInfo(VkFormat imageFormat)
{
    VulkanImageSizeInfo imageSizeInfo = {};

    switch (imageFormat)
    {
    case VK_FORMAT_BC3_UNORM_BLOCK:
        imageSizeInfo._blockSize = 16;
        imageSizeInfo._blockDimension = 4;
        imageSizeInfo._compressed = true;
        imageSizeInfo._elementSize = 1;
        break;
    case VK_FORMAT_BC7_UNORM_BLOCK:
        imageSizeInfo._blockSize = 16;
        imageSizeInfo._blockDimension = 4;
        imageSizeInfo._compressed = true;
        imageSizeInfo._elementSize = 1;
        break;
    default:
        assert(false);
        break;
    }

    return imageSizeInfo;
}

VkImageAspectFlags 
VulkanTypeConversion::ConvertImageAspectFlagsToVulkan(HalImageAspectFlags imageAspectFlags)
{
    VkImageAspectFlags aspectFlags = 0;

    if (imageAspectFlags & static_cast<uint32_t>(HalImageAspectFlagBits::Color))
        aspectFlags |= VK_IMAGE_ASPECT_COLOR_BIT;
    if (imageAspectFlags & static_cast<uint32_t>(HalImageAspectFlagBits::Depth))
        aspectFlags |= VK_IMAGE_ASPECT_DEPTH_BIT;
    if (imageAspectFlags & static_cast<uint32_t>(HalImageAspectFlagBits::Stencil))
        aspectFlags |= VK_IMAGE_ASPECT_STENCIL_BIT;
    if (imageAspectFlags & static_cast<uint32_t>(HalImageAspectFlagBits::MetaData))
        aspectFlags |= VK_IMAGE_ASPECT_METADATA_BIT;
    if (imageAspectFlags & static_cast<uint32_t>(HalImageAspectFlagBits::Plane0))
        aspectFlags |= VK_IMAGE_ASPECT_PLANE_0_BIT;
    if (imageAspectFlags & static_cast<uint32_t>(HalImageAspectFlagBits::Plane1))
        aspectFlags |= VK_IMAGE_ASPECT_PLANE_1_BIT;
    if (imageAspectFlags & static_cast<uint32_t>(HalImageAspectFlagBits::Plane2))
        aspectFlags |= VK_IMAGE_ASPECT_PLANE_2_BIT;
    if (imageAspectFlags & static_cast<uint32_t>(HalImageAspectFlagBits::MemoryPlane0Ext))
        aspectFlags |= VK_IMAGE_ASPECT_MEMORY_PLANE_0_BIT_EXT;
    if (imageAspectFlags & static_cast<uint32_t>(HalImageAspectFlagBits::MemoryPlane1Ext))
        aspectFlags |= VK_IMAGE_ASPECT_MEMORY_PLANE_1_BIT_EXT;
    if (imageAspectFlags & static_cast<uint32_t>(HalImageAspectFlagBits::MemoryPlane2Ext))
        aspectFlags |= VK_IMAGE_ASPECT_MEMORY_PLANE_2_BIT_EXT;
    if (imageAspectFlags & static_cast<uint32_t>(HalImageAspectFlagBits::MemoryPlane3Ext))
        aspectFlags |= VK_IMAGE_ASPECT_MEMORY_PLANE_3_BIT_EXT;

    assert(aspectFlags != 0);
    return aspectFlags;
}

HalImageAspectFlags 
VulkanTypeConversion::ConvertImageAspectFlagsFormatFromVulkan(VkImageAspectFlags imageAspectFlags)
{
    HalImageAspectFlags aspectFlags = 0;

    if (imageAspectFlags & VK_IMAGE_ASPECT_COLOR_BIT)
        aspectFlags |= static_cast<uint32_t>(HalImageAspectFlagBits::Color);
    if (imageAspectFlags & VK_IMAGE_ASPECT_DEPTH_BIT)
        aspectFlags |= static_cast<uint32_t>(HalImageAspectFlagBits::Depth);
    if (imageAspectFlags & VK_IMAGE_ASPECT_STENCIL_BIT)
        aspectFlags |= static_cast<uint32_t>(HalImageAspectFlagBits::Stencil);
    if (imageAspectFlags & VK_IMAGE_ASPECT_METADATA_BIT)
        aspectFlags |= static_cast<uint32_t>(HalImageAspectFlagBits::MetaData);
    if (imageAspectFlags & VK_IMAGE_ASPECT_PLANE_0_BIT)
        aspectFlags |= static_cast<uint32_t>(HalImageAspectFlagBits::Plane0);
    if (imageAspectFlags & VK_IMAGE_ASPECT_PLANE_1_BIT)
        aspectFlags |= static_cast<uint32_t>(HalImageAspectFlagBits::Plane1);
    if (imageAspectFlags & VK_IMAGE_ASPECT_PLANE_2_BIT)
        aspectFlags |= static_cast<uint32_t>(HalImageAspectFlagBits::Plane2);
    if (imageAspectFlags & VK_IMAGE_ASPECT_MEMORY_PLANE_0_BIT_EXT)
        aspectFlags |= static_cast<uint32_t>(HalImageAspectFlagBits::MemoryPlane0Ext);
    if (imageAspectFlags & VK_IMAGE_ASPECT_MEMORY_PLANE_1_BIT_EXT)
        aspectFlags |= static_cast<uint32_t>(HalImageAspectFlagBits::MemoryPlane1Ext);
    if (imageAspectFlags & VK_IMAGE_ASPECT_MEMORY_PLANE_2_BIT_EXT)
        aspectFlags |= static_cast<uint32_t>(HalImageAspectFlagBits::MemoryPlane2Ext);
    if (imageAspectFlags & VK_IMAGE_ASPECT_MEMORY_PLANE_3_BIT_EXT)
        aspectFlags |= static_cast<uint32_t>(HalImageAspectFlagBits::MemoryPlane2Ext);

    assert(aspectFlags != 0);
    return aspectFlags;
}

VkImageUsageFlags 
VulkanTypeConversion::ConvertImageUsageFlagsToVulkan(HalImageUsageFlags imageUsageFlags)
{
    VkImageUsageFlags vkUsageFlag = 0;

    if (imageUsageFlags & static_cast<uint32_t>(HalImageUsageFlagBits::TransferSrc))
        vkUsageFlag |= VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
    if (imageUsageFlags & static_cast<uint32_t>(HalImageUsageFlagBits::TransferDst))
        vkUsageFlag |= VK_IMAGE_USAGE_TRANSFER_DST_BIT;
    if (imageUsageFlags & static_cast<uint32_t>(HalImageUsageFlagBits::Sampled))
        vkUsageFlag |= VK_IMAGE_USAGE_SAMPLED_BIT;
    if (imageUsageFlags & static_cast<uint32_t>(HalImageUsageFlagBits::Storage))
        vkUsageFlag |= VK_IMAGE_USAGE_STORAGE_BIT;
    if (imageUsageFlags & static_cast<uint32_t>(HalImageUsageFlagBits::ColorAttachment))
        vkUsageFlag |= VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    if (imageUsageFlags & static_cast<uint32_t>(HalImageUsageFlagBits::DepthAttachment))
        vkUsageFlag |= VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
    if (imageUsageFlags & static_cast<uint32_t>(HalImageUsageFlagBits::TransientAttachment))
        vkUsageFlag |= VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT;
    if (imageUsageFlags & static_cast<uint32_t>(HalImageUsageFlagBits::InputAttachment))
        vkUsageFlag |= VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT;
    if(imageUsageFlags & static_cast<uint32_t>(HalImageUsageFlagBits::ShadingRateImageNV))
        vkUsageFlag |= VK_IMAGE_USAGE_SHADING_RATE_IMAGE_BIT_NV;

    assert(vkUsageFlag != 0);
    return vkUsageFlag;
}

HalImageUsageFlags
VulkanTypeConversion::ConvertImageUsageFlagsFromVulkan(VkImageUsageFlags imageUsageFlags)
{
    HalImageUsageFlags halUsageFlag = 0;

    if (imageUsageFlags & VK_IMAGE_USAGE_TRANSFER_SRC_BIT)
        halUsageFlag |= static_cast<uint32_t>(HalImageUsageFlagBits::TransferSrc);
    if (imageUsageFlags & VK_IMAGE_USAGE_TRANSFER_DST_BIT)
        halUsageFlag |= static_cast<uint32_t>(HalImageUsageFlagBits::TransferDst);
    if (imageUsageFlags & VK_IMAGE_USAGE_SAMPLED_BIT)
        halUsageFlag |= static_cast<uint32_t>(HalImageUsageFlagBits::Sampled);
    if (imageUsageFlags & VK_IMAGE_USAGE_STORAGE_BIT)
        halUsageFlag |= static_cast<uint32_t>(HalImageUsageFlagBits::Storage);
    if (imageUsageFlags & VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT)
        halUsageFlag |= static_cast<uint32_t>(HalImageUsageFlagBits::ColorAttachment);
    if (imageUsageFlags & VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT)
        halUsageFlag |= static_cast<uint32_t>(HalImageUsageFlagBits::DepthAttachment);
    if (imageUsageFlags & VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT)
        halUsageFlag |= static_cast<uint32_t>(HalImageUsageFlagBits::TransientAttachment);
    if (imageUsageFlags & VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT)
        halUsageFlag |= static_cast<uint32_t>(HalImageUsageFlagBits::InputAttachment);
    if (imageUsageFlags & VK_IMAGE_USAGE_SHADING_RATE_IMAGE_BIT_NV)
        halUsageFlag |= static_cast<uint32_t>(HalImageUsageFlagBits::ShadingRateImageNV);

    assert(halUsageFlag != 0);
    return halUsageFlag;
}

VkComponentSwizzle 
VulkanTypeConversion::ConvertComponentSwizzleToVulkan(HalComponentSwizzle swizzle)
{
    VkComponentSwizzle vkSwizzle = VkComponentSwizzle::VK_COMPONENT_SWIZZLE_IDENTITY;

    switch (swizzle)
    {
    case HalComponentSwizzle::Identity:
        vkSwizzle = VK_COMPONENT_SWIZZLE_IDENTITY;
        break;
    case HalComponentSwizzle::One:
        vkSwizzle = VK_COMPONENT_SWIZZLE_ONE;
        break;
    case HalComponentSwizzle::Zero:
        vkSwizzle = VK_COMPONENT_SWIZZLE_ZERO;
        break;
    case HalComponentSwizzle::Red:
        vkSwizzle = VK_COMPONENT_SWIZZLE_R;
        break;
    case HalComponentSwizzle::Green:
        vkSwizzle = VK_COMPONENT_SWIZZLE_G;
        break;
    case HalComponentSwizzle::Blue:
        vkSwizzle = VK_COMPONENT_SWIZZLE_B;
        break;
    case HalComponentSwizzle::Alpha:
        vkSwizzle = VK_COMPONENT_SWIZZLE_A;
        break;
    default:
        assert(false);
        break;
    }

    return vkSwizzle;
}

HalComponentSwizzle 
VulkanTypeConversion::ConvertComponentSwizzleFromVulkan(VkComponentSwizzle swizzle)
{
    HalComponentSwizzle halSwizzle = HalComponentSwizzle::Identity;

    switch (swizzle)
    {
    case VK_COMPONENT_SWIZZLE_IDENTITY:
        halSwizzle = HalComponentSwizzle::Identity;
        break;
    case VK_COMPONENT_SWIZZLE_ONE:
        halSwizzle = HalComponentSwizzle::One;
        break;
    case VK_COMPONENT_SWIZZLE_ZERO:
        halSwizzle = HalComponentSwizzle::Zero;
        break;
    case VK_COMPONENT_SWIZZLE_R:
        halSwizzle = HalComponentSwizzle::Red;
        break;
    case VK_COMPONENT_SWIZZLE_G:
        halSwizzle = HalComponentSwizzle::Green;
        break;
    case VK_COMPONENT_SWIZZLE_B:
        halSwizzle = HalComponentSwizzle::Blue;
        break;
    case VK_COMPONENT_SWIZZLE_A:
        halSwizzle = HalComponentSwizzle::Alpha;
        break;
    default:
        assert(false);
        break;
    }

    return halSwizzle;
}

VkComponentMapping 
VulkanTypeConversion::ConvertComponentMappingToVulkan(HalComponentMapping components)
{
    VkComponentMapping component = { 
        ConvertComponentSwizzleToVulkan(components._r),
        ConvertComponentSwizzleToVulkan(components._g),
        ConvertComponentSwizzleToVulkan(components._b),
        ConvertComponentSwizzleToVulkan(components._a) };

    return component;
}

HalComponentMapping 
VulkanTypeConversion::ConvertComponentMappingFromVulkan(VkComponentMapping components)
{
    HalComponentMapping component = {
        ConvertComponentSwizzleFromVulkan(components.r),
        ConvertComponentSwizzleFromVulkan(components.g),
        ConvertComponentSwizzleFromVulkan(components.b),
        ConvertComponentSwizzleFromVulkan(components.a) };

    return component;
}

VkSamplerAddressMode 
VulkanTypeConversion::ConvertSamplerAddressModeToVulkan(HalSamplerAddressMode addressMode)
{
    VkSamplerAddressMode vkAddressMode = VK_SAMPLER_ADDRESS_MODE_REPEAT;

    switch (addressMode)
    {
    case HalSamplerAddressMode::Repeat:
        vkAddressMode = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        break;
    case HalSamplerAddressMode::MirroredRepeat:
        vkAddressMode = VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT;
        break;
    case HalSamplerAddressMode::ClampToEdge:
        vkAddressMode = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
        break;
    case HalSamplerAddressMode::ClampToBorder:
        vkAddressMode = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
        break;
    case HalSamplerAddressMode::MirrorClampToEdge:
        vkAddressMode = VK_SAMPLER_ADDRESS_MODE_MIRROR_CLAMP_TO_EDGE;
        break;
    default:
        assert(false);
        break;
    }

    return vkAddressMode;
}

HalSamplerAddressMode 
VulkanTypeConversion::ConvertSamplerAddressModeFromVulkan(VkSamplerAddressMode addressMode)
{
    HalSamplerAddressMode halAddressMode = HalSamplerAddressMode::Repeat;

    switch (addressMode)
    {
    case VK_SAMPLER_ADDRESS_MODE_REPEAT:
        halAddressMode = HalSamplerAddressMode::Repeat;
        break;
    case VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT:
        halAddressMode = HalSamplerAddressMode::MirroredRepeat;
        break;
    case VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE:
        halAddressMode = HalSamplerAddressMode::ClampToEdge;
        break;
    case VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER:
        halAddressMode = HalSamplerAddressMode::ClampToBorder;
        break;
    case VK_SAMPLER_ADDRESS_MODE_MIRROR_CLAMP_TO_EDGE:
        halAddressMode = HalSamplerAddressMode::MirrorClampToEdge;
        break;
    default:
        assert(false);
        break;
    }

    return halAddressMode;
}

VkFilter 
VulkanTypeConversion::ConvertFilterToVulkan(HalFilter filter)
{
    VkFilter vkFilter = VK_FILTER_NEAREST;

    switch (filter)
    {
    case HalFilter::Nearest:
        vkFilter = VK_FILTER_NEAREST;
        break;
    case HalFilter::Linear:
        vkFilter = VK_FILTER_LINEAR;
        break;
    case HalFilter::Cubic:
        vkFilter = VK_FILTER_CUBIC_IMG;
        break;
    default:
        assert(false);
        break;
    }

    return vkFilter;
}

HalFilter 
VulkanTypeConversion::ConvertFilterFromVulkan(VkFilter filter)
{
    HalFilter halFilter = HalFilter::Nearest;

    switch (filter)
    {
    case VK_FILTER_NEAREST:
        halFilter = HalFilter::Nearest;
        break;
    case VK_FILTER_LINEAR:
        halFilter = HalFilter::Linear;
        break;
    case VK_FILTER_CUBIC_IMG:
        halFilter = HalFilter::Cubic;
        break;
    default:
        assert(false);
        break;
    }

    return halFilter;
}

VkSamplerMipmapMode 
VulkanTypeConversion::ConvertSamplerMipmapModeToVulkan(HalSamplerMipmapMode mipmapMode)
{
    VkSamplerMipmapMode vkSamplerMode = VK_SAMPLER_MIPMAP_MODE_NEAREST;

    switch (mipmapMode)
    {
    case HalSamplerMipmapMode::Nearest:
        vkSamplerMode = VK_SAMPLER_MIPMAP_MODE_NEAREST;
        break;
    case HalSamplerMipmapMode::Linear:
        vkSamplerMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
        break;
    default:
        assert(false);
        break;
    }

    return vkSamplerMode;
}

HalSamplerMipmapMode 
VulkanTypeConversion::ConvertSamplerMipmapModeFromVulkan(VkSamplerMipmapMode mipmapMode)
{
    HalSamplerMipmapMode halSamplerMode = HalSamplerMipmapMode::Nearest;

    switch (mipmapMode)
    {
    case VK_SAMPLER_MIPMAP_MODE_NEAREST:
        halSamplerMode = HalSamplerMipmapMode::Nearest;
        break;
    case VK_SAMPLER_MIPMAP_MODE_LINEAR:
        halSamplerMode = HalSamplerMipmapMode::Linear;
        break;
    default:
        assert(false);
        break;
    }

    return halSamplerMode;
}

VkBorderColor 
VulkanTypeConversion::ConvertBorderColorToVulkan(HalBorderColor borderColor)
{
    VkBorderColor vkBorderColor = VK_BORDER_COLOR_FLOAT_TRANSPARENT_BLACK;

    switch (borderColor)
    {
    case HalBorderColor::FloatTransparentBlack:
        vkBorderColor = VK_BORDER_COLOR_FLOAT_TRANSPARENT_BLACK;
        break;
    case HalBorderColor::IntTransparentBlack:
        vkBorderColor = VK_BORDER_COLOR_INT_TRANSPARENT_BLACK;
        break;
    case HalBorderColor::FloatOpaqueBlack:
        vkBorderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_BLACK;
        break;
    case HalBorderColor::IntOpaqueBlack:
        vkBorderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
        break;
    case HalBorderColor::FloatOpaqueWhite:
        vkBorderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;
        break;
    case HalBorderColor::IntOpaqueWhite:
        vkBorderColor = VK_BORDER_COLOR_INT_OPAQUE_WHITE;
        break;
    default:
        assert(false);
        break;
    }

    return vkBorderColor;
}

HalBorderColor 
VulkanTypeConversion::ConvertBorderColorFromVulkan(VkBorderColor borderColor)
{
    HalBorderColor halBorderColor = HalBorderColor::FloatTransparentBlack;

    switch (borderColor)
    {
    case VK_BORDER_COLOR_FLOAT_TRANSPARENT_BLACK:
        halBorderColor = HalBorderColor::FloatTransparentBlack;
        break;
    case VK_BORDER_COLOR_INT_TRANSPARENT_BLACK:
        halBorderColor = HalBorderColor::IntTransparentBlack;
        break;
    case VK_BORDER_COLOR_FLOAT_OPAQUE_BLACK:
        halBorderColor = HalBorderColor::FloatOpaqueBlack;
        break;
    case VK_BORDER_COLOR_INT_OPAQUE_BLACK:
        halBorderColor = HalBorderColor::IntOpaqueBlack;
        break;
    case VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE:
        halBorderColor = HalBorderColor::FloatOpaqueWhite;
        break;
    case VK_BORDER_COLOR_INT_OPAQUE_WHITE:
        halBorderColor = HalBorderColor::IntOpaqueWhite;
        break;
    default:
        assert(false);
        break;
    }

    return halBorderColor;
}

VkSampleCountFlagBits
VulkanTypeConversion::ConvertSampleCountToVulkan(HalSampleCount sampleCount)
{
    VkSampleCountFlagBits vkSampleCount = VK_SAMPLE_COUNT_1_BIT;

    switch (sampleCount)
    {
    case HalSampleCount::SampleCount1:
        vkSampleCount = VK_SAMPLE_COUNT_1_BIT;
        break;
    case HalSampleCount::SampleCount2:
        vkSampleCount = VK_SAMPLE_COUNT_2_BIT;
        break;
    case HalSampleCount::SampleCount4:
        vkSampleCount = VK_SAMPLE_COUNT_4_BIT;
        break;
    case HalSampleCount::SampleCount8:
        vkSampleCount = VK_SAMPLE_COUNT_8_BIT;
        break;
    case HalSampleCount::SampleCount16:
        vkSampleCount = VK_SAMPLE_COUNT_16_BIT;
        break;
    case HalSampleCount::SampleCount32:
        vkSampleCount = VK_SAMPLE_COUNT_32_BIT;
        break;
    case HalSampleCount::SampleCount64:
        vkSampleCount = VK_SAMPLE_COUNT_64_BIT;
        break;
    default:
        vkSampleCount = VK_SAMPLE_COUNT_1_BIT;
        assert(false);
        break;
    }

    return vkSampleCount;
}

VkCullModeFlagBits
VulkanTypeConversion::ConvertCullModeToVulkan(HalCullMode mode)
{
    VkCullModeFlagBits modeFlag = VK_CULL_MODE_NONE;

    switch (mode)
    {
    case HalCullMode::Front:
        modeFlag = VK_CULL_MODE_FRONT_BIT;
        break;
    case HalCullMode::Back:
        modeFlag = VK_CULL_MODE_BACK_BIT;
        break;
    case HalCullMode::FrontAndBack:
        modeFlag = VK_CULL_MODE_FRONT_AND_BACK;
        break;
    case HalCullMode::NoCull:
        modeFlag = VK_CULL_MODE_NONE;
        break;
    default:
        modeFlag = VK_CULL_MODE_NONE;
        assert(false);
        break;
    }

    return modeFlag;
}

VkFrontFace
VulkanTypeConversion::ConvertFrontFaceToVulkan(HalFrontFace face)
{
    VkFrontFace frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;

    switch (face)
    {
    case HalFrontFace::CounterClockwise:
        frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
        break;
    case HalFrontFace::Clockwise:
        frontFace = VK_FRONT_FACE_CLOCKWISE;
        break;
    default:
        frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
        assert(false);
        break;
    }

    return frontFace;
}

VkPolygonMode
VulkanTypeConversion::ConvertPolygonModeToVulkan(HalPolygonMode mode)
{
    VkPolygonMode polyMode = VK_POLYGON_MODE_FILL;

    switch (mode)
    {
    case HalPolygonMode::Fill:
        polyMode = VK_POLYGON_MODE_FILL;
        break;
    case HalPolygonMode::Line:
        polyMode = VK_POLYGON_MODE_LINE;
        break;
    case HalPolygonMode::Point:
        polyMode = VK_POLYGON_MODE_POINT;
        break;
    default:
        polyMode = VK_POLYGON_MODE_FILL;
        assert(false);
        break;
    }

    return polyMode;
}

VkStencilOp
VulkanTypeConversion::ConvertStencilOpToVulkan(HalStencilOp op)
{
    VkStencilOp stencilOp = VK_STENCIL_OP_KEEP;

    switch (op)
    {
    case HalStencilOp::Keep:
        stencilOp = VK_STENCIL_OP_KEEP;
        break;
    case HalStencilOp::Zero:
        stencilOp = VK_STENCIL_OP_ZERO;
        break;
    case HalStencilOp::Replace:
        stencilOp = VK_STENCIL_OP_REPLACE;
        break;
    case HalStencilOp::IncrementClamp:
        stencilOp = VK_STENCIL_OP_INCREMENT_AND_CLAMP;
        break;
    case HalStencilOp::DecrementClamp:
        stencilOp = VK_STENCIL_OP_DECREMENT_AND_CLAMP;
        break;
    case HalStencilOp::Invert:
        stencilOp = VK_STENCIL_OP_INVERT;
        break;
    case HalStencilOp::IncrementWarp:
        stencilOp = VK_STENCIL_OP_INCREMENT_AND_WRAP;
        break;
    case HalStencilOp::DecrementWarp:
        stencilOp = VK_STENCIL_OP_DECREMENT_AND_WRAP;
        break;
    default:
        stencilOp = VK_STENCIL_OP_KEEP;
        assert(false);
        break;
    }

    return stencilOp;
}

VkCompareOp
VulkanTypeConversion::ConvertDepthCompareOpToVulkan(HalCompareOp compOp)
{
    VkCompareOp compareOp = VK_COMPARE_OP_NEVER;

    switch (compOp)
    {
    case HalCompareOp::Never:
        compareOp = VK_COMPARE_OP_NEVER;
        break;
    case HalCompareOp::Less:
        compareOp = VK_COMPARE_OP_LESS;
        break;
    case HalCompareOp::Equal:
        compareOp = VK_COMPARE_OP_EQUAL;
        break;
    case HalCompareOp::LessEqual:
        compareOp = VK_COMPARE_OP_LESS_OR_EQUAL;
        break;
    case HalCompareOp::Greater:
        compareOp = VK_COMPARE_OP_GREATER;
        break;
    case HalCompareOp::NotEqual:
        compareOp = VK_COMPARE_OP_NOT_EQUAL;
        break;
    case HalCompareOp::GreaterEqual:
        compareOp = VK_COMPARE_OP_GREATER_OR_EQUAL;
        break;
    case HalCompareOp::AlwaysPass:
        compareOp = VK_COMPARE_OP_ALWAYS;
        break;
    default:
        compareOp = VK_COMPARE_OP_NEVER;
        assert(false);
        break;
    }

    return compareOp;
}

VkBlendFactor
VulkanTypeConversion::ConvertBlendFactorToVulkan(HalBlendFactor factor)
{
    VkBlendFactor blendFactor = VK_BLEND_FACTOR_ZERO;

    switch (factor)
    {
    case HalBlendFactor::Zero:
        blendFactor = VK_BLEND_FACTOR_ZERO;
        break;
    case HalBlendFactor::One:
        blendFactor = VK_BLEND_FACTOR_ONE;
        break;
    case HalBlendFactor::SrcColor:
        blendFactor = VK_BLEND_FACTOR_SRC_COLOR;
        break;
    case HalBlendFactor::OneMinusSrcColor:
        blendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_COLOR;
        break;
    case HalBlendFactor::DstColor:
        blendFactor = VK_BLEND_FACTOR_DST_COLOR;
        break;
    case HalBlendFactor::OneMinusDstColor:
        blendFactor = VK_BLEND_FACTOR_ONE_MINUS_DST_COLOR;
        break;
    case HalBlendFactor::SrcAlpha:
        blendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
        break;
    case HalBlendFactor::OneMinusSrcAlpha:
        blendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
        break;
    case HalBlendFactor::DstAlpha:
        blendFactor = VK_BLEND_FACTOR_DST_ALPHA;
        break;
    case HalBlendFactor::OneMinusDstAlpha:
        blendFactor = VK_BLEND_FACTOR_ONE_MINUS_DST_ALPHA;
        break;
    case HalBlendFactor::ConstantColor:
        blendFactor = VK_BLEND_FACTOR_CONSTANT_COLOR;
        break;
    case HalBlendFactor::OneMinusConstantColor:
        blendFactor = VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_COLOR;
        break;
    case HalBlendFactor::ConstantAlpha:
        blendFactor = VK_BLEND_FACTOR_CONSTANT_ALPHA;
        break;
    case HalBlendFactor::OneMinusConstantAlpha:
        blendFactor = VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_ALPHA;
        break;
    case HalBlendFactor::SrcAlphaSaturate:
        blendFactor = VK_BLEND_FACTOR_SRC_ALPHA_SATURATE;
        break;
    case HalBlendFactor::Src1Color:
        blendFactor = VK_BLEND_FACTOR_SRC1_COLOR;
        break;
    case HalBlendFactor::OneMinusSrc1Color:
        blendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC1_COLOR;
        break;
    case HalBlendFactor::Src1Alpha:
        blendFactor = VK_BLEND_FACTOR_SRC1_ALPHA;
        break;
    case HalBlendFactor::OneMinusSrc1Alpha:
        blendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC1_ALPHA;
        break;
    default:
        blendFactor = VK_BLEND_FACTOR_ZERO;
        assert(false);
        break;
    }

    return blendFactor;
}

VkLogicOp
VulkanTypeConversion::ConvertLogicOpToVulkan(HalLogicOp op)
{
    VkLogicOp logicOp = VK_LOGIC_OP_CLEAR;

    switch (op)
    {
    case HalLogicOp::Clear:
        logicOp = VK_LOGIC_OP_CLEAR;
        break;
    case HalLogicOp::And:
        logicOp = VK_LOGIC_OP_AND;
        break;
    case HalLogicOp::AndReverse:
        logicOp = VK_LOGIC_OP_AND_REVERSE;
        break;
    case HalLogicOp::Copy:
        logicOp = VK_LOGIC_OP_COPY;
        break;
    case HalLogicOp::AndInverted:
        logicOp = VK_LOGIC_OP_AND_INVERTED;
        break;
    case HalLogicOp::NoOp:
        logicOp = VK_LOGIC_OP_NO_OP;
        break;
    case HalLogicOp::Xor:
        logicOp = VK_LOGIC_OP_XOR;
        break;
    case HalLogicOp::Or:
        logicOp = VK_LOGIC_OP_OR;
        break;
    case HalLogicOp::Nor:
        logicOp = VK_LOGIC_OP_NOR;
        break;
    case HalLogicOp::Equivalent:
        logicOp = VK_LOGIC_OP_EQUIVALENT;
        break;
    case HalLogicOp::Invert:
        logicOp = VK_LOGIC_OP_INVERT;
        break;
    case HalLogicOp::OrReverse:
        logicOp = VK_LOGIC_OP_OR_REVERSE;
        break;
    case HalLogicOp::CopyInverted:
        logicOp = VK_LOGIC_OP_COPY_INVERTED;
        break;
    case HalLogicOp::OrInverted:
        logicOp = VK_LOGIC_OP_OR_INVERTED;
        break;
    case HalLogicOp::Nand:
        logicOp = VK_LOGIC_OP_NAND;
        break;
    case HalLogicOp::Set:
        logicOp = VK_LOGIC_OP_SET;
        break;
    default:
        logicOp = VK_LOGIC_OP_CLEAR;
        assert(false);
        break;
    }

    return logicOp;
}

VkBlendOp
VulkanTypeConversion::ConvertBlendOpToVulkan(HalBlendOp op)
{
    VkBlendOp blendOp = VK_BLEND_OP_ADD;

    switch (op)
    {
    case HalBlendOp::Add:
        blendOp = VK_BLEND_OP_ADD;
        break;
    case HalBlendOp::Subtract:
        blendOp = VK_BLEND_OP_SUBTRACT;
        break;
    case HalBlendOp::ReverseSubtract:
        blendOp = VK_BLEND_OP_REVERSE_SUBTRACT;
        break;
    case HalBlendOp::Min:
        blendOp = VK_BLEND_OP_MIN;
        break;
    case HalBlendOp::Max:
        blendOp = VK_BLEND_OP_MAX;
        break;
    default:
        blendOp = VK_BLEND_OP_ADD;
        assert(false);
        break;
    }

    return blendOp;
}

VkColorComponentFlags
VulkanTypeConversion::ConvertColorComponentToVulkan(HalColorComponentFlags colorComponentFlags)
{
    VkColorComponentFlags flags = 0;

    if (colorComponentFlags & static_cast<uint32_t>(HalColorComponents::Red))
        flags |= VK_COLOR_COMPONENT_R_BIT;
    if (colorComponentFlags & static_cast<uint32_t>(HalColorComponents::Green))
        flags |= VK_COLOR_COMPONENT_G_BIT;
    if (colorComponentFlags & static_cast<uint32_t>(HalColorComponents::Blue))
        flags |= VK_COLOR_COMPONENT_B_BIT;
    if (colorComponentFlags & static_cast<uint32_t>(HalColorComponents::Alpha))
        flags |= VK_COLOR_COMPONENT_A_BIT;

    assert(flags != 0);
    return flags;
}

VkDynamicState
VulkanTypeConversion::ConvertDynamicStateToVulkan(HalDynamicStates state)
{
    VkDynamicState dynamicState = VK_DYNAMIC_STATE_VIEWPORT;

    switch (state)
    {
    case HalDynamicStates::Viewport:
        dynamicState = VK_DYNAMIC_STATE_VIEWPORT;
        break;
    case HalDynamicStates::Scissor:
        dynamicState = VK_DYNAMIC_STATE_SCISSOR;
        break;
    case HalDynamicStates::LineWidth:
        dynamicState = VK_DYNAMIC_STATE_LINE_WIDTH;
        break;
    case HalDynamicStates::DepthBias:
        dynamicState = VK_DYNAMIC_STATE_DEPTH_BIAS;
        break;
    case HalDynamicStates::BlendConstants:
        dynamicState = VK_DYNAMIC_STATE_BLEND_CONSTANTS;
        break;
    case HalDynamicStates::DepthBounds:
        dynamicState = VK_DYNAMIC_STATE_DEPTH_BOUNDS;
        break;
    case HalDynamicStates::StencilCompareMask:
        dynamicState = VK_DYNAMIC_STATE_STENCIL_COMPARE_MASK;
        break;
    case HalDynamicStates::StencilWriteMask:
        dynamicState = VK_DYNAMIC_STATE_STENCIL_WRITE_MASK;
        break;
    case HalDynamicStates::StencilReference:
        dynamicState = VK_DYNAMIC_STATE_STENCIL_REFERENCE;
        break;
    default:
        dynamicState = VK_DYNAMIC_STATE_VIEWPORT;
        assert(false);
        break;
    }

    return dynamicState;
}

VkShaderStageFlags
VulkanTypeConversion::ConvertShaderStagesToVulkan(HalShaderStagesFlags flags)
{
    VkShaderStageFlags shaderStageFlags = 0;

    if (flags & static_cast<uint32_t>(HalShaderStages::Vertex))
        shaderStageFlags |= VK_SHADER_STAGE_VERTEX_BIT;
    if (flags & static_cast<uint32_t>(HalShaderStages::TessellationControl))
        shaderStageFlags |= VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
    if (flags & static_cast<uint32_t>(HalShaderStages::TessellationEvaluation))
        shaderStageFlags |= VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;
    if (flags & static_cast<uint32_t>(HalShaderStages::Geometry))
        shaderStageFlags |= VK_SHADER_STAGE_GEOMETRY_BIT;
    if (flags & static_cast<uint32_t>(HalShaderStages::Fragment))
        shaderStageFlags |= VK_SHADER_STAGE_FRAGMENT_BIT;
    if (flags & static_cast<uint32_t>(HalShaderStages::Compute))
        shaderStageFlags |= VK_SHADER_STAGE_COMPUTE_BIT;

    assert(shaderStageFlags);
    return shaderStageFlags;
}

VkAttachmentLoadOp
VulkanTypeConversion::ConvertAttachmentLoadOpToVulkan(HalAttachmentLoadOperation loadOp)
{
    VkAttachmentLoadOp vkLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;

    switch (loadOp)
    {
    case HalAttachmentLoadOperation::Clear:
        vkLoadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        break;
    case HalAttachmentLoadOperation::DontCare:
        vkLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        break;
    case HalAttachmentLoadOperation::Load:
        vkLoadOp = VK_ATTACHMENT_LOAD_OP_LOAD;
        break;
    default:
        vkLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        assert(false);
        break;
    }

    return vkLoadOp;
}

VkAttachmentStoreOp
VulkanTypeConversion::ConvertAttachmentStoreOpToVulkan(HalAttachmentStoreOperation storeOp)
{
    VkAttachmentStoreOp vkStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;

    switch (storeOp)
    {
    case HalAttachmentStoreOperation::DontCare:
        vkStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        break;
    case HalAttachmentStoreOperation::Store:
        vkStoreOp = VK_ATTACHMENT_STORE_OP_STORE;
        break;
    default:
        vkStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        assert(false);
        break;
    }

    return vkStoreOp;
}

VkImageLayout
VulkanTypeConversion::ConvertImageLayoutToVulkan(HalImageLayout imageLayout)
{
    VkImageLayout vkImageLayout = VK_IMAGE_LAYOUT_UNDEFINED;

    switch (imageLayout)
    {
    case HalImageLayout::Undefined:
        vkImageLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        break;
    case HalImageLayout::General:
        vkImageLayout = VK_IMAGE_LAYOUT_GENERAL;
        break;
    case HalImageLayout::ColorAttachment:
        vkImageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
        break;
    case HalImageLayout::DepthStencilAttachment:
        vkImageLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
        break;
    case HalImageLayout::DepthStencilReadOnly:
        vkImageLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL;
        break;
    case HalImageLayout::ShaderReadOnly:
        vkImageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        break;
    case HalImageLayout::TransferSrc:
        vkImageLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
        break;
    case HalImageLayout::TransferDst:
        vkImageLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
        break;
    case HalImageLayout::Preinitialized:
        vkImageLayout = VK_IMAGE_LAYOUT_PREINITIALIZED;
        break;
    case HalImageLayout::PresentSrcKHR:
        vkImageLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
        break;
    case HalImageLayout::SharedPresentSrcKHR:
        vkImageLayout = VK_IMAGE_LAYOUT_SHARED_PRESENT_KHR;
        break;
    default:
        vkImageLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        assert(false);
        break;
    }

    return vkImageLayout;
}

VkCommandPoolCreateFlags
VulkanTypeConversion::ConvertCommandPoolUsageFlagsToVulkan(HalCommandPoolUsageFlags usageFlags)
{
    VkCommandPoolCreateFlags createFlags = 0;

    if (usageFlags & static_cast<uint32_t>(HalCommandPoolUsage::CreateTransient))
        createFlags |= VK_COMMAND_POOL_CREATE_TRANSIENT_BIT;
    if (usageFlags & static_cast<uint32_t>(HalCommandPoolUsage::ResetCommandBuffer))
        createFlags |= VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

    return createFlags;
}

VkCommandBufferLevel
VulkanTypeConversion::ConvertCommandBufferLevelToVulkan(HalCommandBufferLevel level)
{
    VkCommandBufferLevel vkLevel = VK_COMMAND_BUFFER_LEVEL_PRIMARY;

    switch (level)
    {
    case HalCommandBufferLevel::PrimaryLevel:
        vkLevel = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        break;
    case HalCommandBufferLevel::SecondaryLevel:
        vkLevel = VK_COMMAND_BUFFER_LEVEL_SECONDARY;
        break;
    default:
        vkLevel = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        assert(false);
        break;
    }

    return vkLevel;
}

VkCommandBufferUsageFlags
VulkanTypeConversion::ConvertCommandBufferUsageFlagsToVulkan(HalCommandBufferUsageFlags usageFlags)
{
    VkCommandBufferUsageFlags vkUsageFlags = 0;

    if (usageFlags & static_cast<uint32_t>(HalCommandBufferUsage::OneTimeSubmit))
        vkUsageFlags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
    if (usageFlags & static_cast<uint32_t>(HalCommandBufferUsage::RenderPassContinue))
        vkUsageFlags |= VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT;
    if (usageFlags & static_cast<uint32_t>(HalCommandBufferUsage::SimultaneousUse))
        vkUsageFlags |= VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;

    return vkUsageFlags;
}

VkSubpassContents
VulkanTypeConversion::ConvertSubpassContentToVulkan(HalSubpassContents content)
{
    VkSubpassContents vkContent = VK_SUBPASS_CONTENTS_INLINE;

    switch (content)
    {
    case HalSubpassContents::Inline:
        vkContent = VK_SUBPASS_CONTENTS_INLINE;
        break;
    case HalSubpassContents::SecondarCommandBuffer:
        vkContent = VK_SUBPASS_CONTENTS_SECONDARY_COMMAND_BUFFERS;
        break;
    default:
        assert(false);
        vkContent = VK_SUBPASS_CONTENTS_INLINE;
        break;
    }

    return vkContent;
}

VkDependencyFlags
VulkanTypeConversion::ConvertDependencyFlagsToVulkan(HalDependencyFlags dependency)
{
    VkDependencyFlags vkFlags = 0;
    if (dependency & static_cast<uint32_t>(HalDependencyBits::DependencyByRegion))
        vkFlags |= VK_DEPENDENCY_BY_REGION_BIT;

    return vkFlags;
}

VkPipelineStageFlags
VulkanTypeConversion::ConvertPipelineFlagsToVulkan(HalPipelineStageFlags pipelineStage)
{
    VkPipelineStageFlags vkFlags = 0;

    if (pipelineStage & static_cast<uint32_t>(HalPipelineStageBits::TopOfPipe))
        vkFlags |= VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
    if (pipelineStage & static_cast<uint32_t>(HalPipelineStageBits::DrawIndirect))
        vkFlags |= VK_PIPELINE_STAGE_DRAW_INDIRECT_BIT;
    if (pipelineStage & static_cast<uint32_t>(HalPipelineStageBits::VertexInput))
        vkFlags |= VK_PIPELINE_STAGE_VERTEX_INPUT_BIT;
    if (pipelineStage & static_cast<uint32_t>(HalPipelineStageBits::DrawIndirect))
        vkFlags |= VK_PIPELINE_STAGE_DRAW_INDIRECT_BIT;
    if (pipelineStage & static_cast<uint32_t>(HalPipelineStageBits::VertexShader))
        vkFlags |= VK_PIPELINE_STAGE_VERTEX_SHADER_BIT;
    if (pipelineStage & static_cast<uint32_t>(HalPipelineStageBits::TessellationControlShader))
        vkFlags |= VK_PIPELINE_STAGE_TESSELLATION_CONTROL_SHADER_BIT;
    if (pipelineStage & static_cast<uint32_t>(HalPipelineStageBits::TessellationEvaluationShader))
        vkFlags |= VK_PIPELINE_STAGE_TESSELLATION_EVALUATION_SHADER_BIT;
    if (pipelineStage & static_cast<uint32_t>(HalPipelineStageBits::GeometryShader))
        vkFlags |= VK_PIPELINE_STAGE_GEOMETRY_SHADER_BIT;
    if (pipelineStage & static_cast<uint32_t>(HalPipelineStageBits::FragmentShader))
        vkFlags |= VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    if (pipelineStage & static_cast<uint32_t>(HalPipelineStageBits::EarlyFragmentTests))
        vkFlags |= VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
    if (pipelineStage & static_cast<uint32_t>(HalPipelineStageBits::LateFragmentTests))
        vkFlags |= VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
    if (pipelineStage & static_cast<uint32_t>(HalPipelineStageBits::ColorAttachmentOutput))
        vkFlags |= VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    if (pipelineStage & static_cast<uint32_t>(HalPipelineStageBits::ComputeShader))
        vkFlags |= VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT;
    if (pipelineStage & static_cast<uint32_t>(HalPipelineStageBits::Transfer))
        vkFlags |= VK_PIPELINE_STAGE_TRANSFER_BIT;
    if (pipelineStage & static_cast<uint32_t>(HalPipelineStageBits::BottomOfPipe))
        vkFlags |= VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
    if (pipelineStage & static_cast<uint32_t>(HalPipelineStageBits::Host))
        vkFlags |= VK_PIPELINE_STAGE_HOST_BIT;

    return vkFlags;
}

VkAccessFlags
VulkanTypeConversion::ConvertAccessFlagsToVulkan(HalAccessFlags access)
{
    VkAccessFlags vkFlags = 0;

    if (access & static_cast<uint32_t>(HalAccessBits::IndirectCommandRead))
        vkFlags |= VK_ACCESS_INDIRECT_COMMAND_READ_BIT;
    if (access & static_cast<uint32_t>(HalAccessBits::IndexRead))
        vkFlags |= VK_ACCESS_INDEX_READ_BIT;
    if (access & static_cast<uint32_t>(HalAccessBits::VertexAttributeRead))
        vkFlags |= VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT;
    if (access & static_cast<uint32_t>(HalAccessBits::UniformRead))
        vkFlags |= VK_ACCESS_UNIFORM_READ_BIT;
    if (access & static_cast<uint32_t>(HalAccessBits::InputAttachmentRead))
        vkFlags |= VK_ACCESS_INPUT_ATTACHMENT_READ_BIT;
    if (access & static_cast<uint32_t>(HalAccessBits::ShaderRead))
        vkFlags |= VK_ACCESS_SHADER_READ_BIT;
    if (access & static_cast<uint32_t>(HalAccessBits::ShaderWrite))
        vkFlags |= VK_ACCESS_SHADER_WRITE_BIT;
    if (access & static_cast<uint32_t>(HalAccessBits::ColorAttachmentRead))
        vkFlags |= VK_ACCESS_COLOR_ATTACHMENT_READ_BIT;
    if (access & static_cast<uint32_t>(HalAccessBits::ColorAttachmentWrite))
        vkFlags |= VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    if (access & static_cast<uint32_t>(HalAccessBits::DepthStencilAttachmentRead))
        vkFlags |= VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT;
    if (access & static_cast<uint32_t>(HalAccessBits::DepthStencilAttachmentWrite))
        vkFlags |= VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
    if (access & static_cast<uint32_t>(HalAccessBits::TransferRead))
        vkFlags |= VK_ACCESS_TRANSFER_READ_BIT;
    if (access & static_cast<uint32_t>(HalAccessBits::TransferWrite))
        vkFlags |= VK_ACCESS_TRANSFER_WRITE_BIT;
    if (access & static_cast<uint32_t>(HalAccessBits::HostRead))
        vkFlags |= VK_ACCESS_HOST_READ_BIT;
    if (access & static_cast<uint32_t>(HalAccessBits::HostWrite))
        vkFlags |= VK_ACCESS_HOST_WRITE_BIT;
    if (access & static_cast<uint32_t>(HalAccessBits::MemoryRead))
        vkFlags |= VK_ACCESS_MEMORY_READ_BIT;
    if (access & static_cast<uint32_t>(HalAccessBits::MemoryWrite))
        vkFlags |= VK_ACCESS_MEMORY_WRITE_BIT;

    return vkFlags;
}

VkBufferCreateFlags
VulkanTypeConversion::ConvertBufferCreateFlagsToVulkan(HalBufferCreateFlags create)
{
    VkBufferCreateFlags vkFlags = 0;

    if (create & static_cast<uint32_t>(HalBufferCreate::SparseBinding))
        vkFlags |= VK_BUFFER_CREATE_SPARSE_BINDING_BIT;
    if (create & static_cast<uint32_t>(HalBufferCreate::SparseResidency))
        vkFlags |= VK_BUFFER_CREATE_SPARSE_RESIDENCY_BIT;
    if (create & static_cast<uint32_t>(HalBufferCreate::SparseAliased))
        vkFlags |= VK_BUFFER_CREATE_SPARSE_ALIASED_BIT;

    return vkFlags;
}

VkBufferUsageFlags
VulkanTypeConversion::ConvertBufferUsageFlagsToVulkan(HalBufferUsageFlags usage)
{
    VkBufferUsageFlags vkFlags = 0;

    if (usage & static_cast<uint32_t>(HalBufferUsage::TransferSource))
        vkFlags |= VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
    if (usage & static_cast<uint32_t>(HalBufferUsage::TransferDest))
        vkFlags |= VK_BUFFER_USAGE_TRANSFER_DST_BIT;
    if (usage & static_cast<uint32_t>(HalBufferUsage::UniformTexelBuffer))
        vkFlags |= VK_BUFFER_USAGE_UNIFORM_TEXEL_BUFFER_BIT;
    if (usage & static_cast<uint32_t>(HalBufferUsage::StorageTexelBuffer))
        vkFlags |= VK_BUFFER_USAGE_STORAGE_TEXEL_BUFFER_BIT;
    if (usage & static_cast<uint32_t>(HalBufferUsage::UniformBuffer))
        vkFlags |= VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
    if (usage & static_cast<uint32_t>(HalBufferUsage::StorageBuffer))
        vkFlags |= VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
    if (usage & static_cast<uint32_t>(HalBufferUsage::IndexBuffer))
        vkFlags |= VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
    if (usage & static_cast<uint32_t>(HalBufferUsage::VertexBuffer))
        vkFlags |= VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
    if (usage & static_cast<uint32_t>(HalBufferUsage::IndirectBuffer))
        vkFlags |= VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT;

    assert(vkFlags);	// assert if zero
    return vkFlags;
}

VkSharingMode
VulkanTypeConversion::ConvertSharedModeFlagsToVulkan(HalBufferShareMode mode)
{
    VkSharingMode vkMode = VK_SHARING_MODE_EXCLUSIVE;

    if (mode == HalBufferShareMode::Comcurrent)
        vkMode = VK_SHARING_MODE_CONCURRENT;

    return vkMode;
}

VkVertexInputRate
VulkanTypeConversion::ConvertVertexInputRateToVulkan(HalVertexInputRate rate)
{
    VkVertexInputRate vkRate = VK_VERTEX_INPUT_RATE_VERTEX;

    if (rate == HalVertexInputRate::Instance)
        vkRate = VK_VERTEX_INPUT_RATE_INSTANCE;

    return vkRate;
}

VkMemoryPropertyFlags
VulkanTypeConversion::ConvertMemoryPropertiesToVulkan(HalMemoryPropertyFlags properties)
{
    VkMemoryPropertyFlags vkFlags = 0;

    if (properties & static_cast<uint32_t>(HalMemoryPropertyBits::DeviceLocal))
        vkFlags |= VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
    if (properties & static_cast<uint32_t>(HalMemoryPropertyBits::HostVisible))
        vkFlags |= VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;
    if (properties & static_cast<uint32_t>(HalMemoryPropertyBits::HostCoherent))
        vkFlags |= VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
    if (properties & static_cast<uint32_t>(HalMemoryPropertyBits::HostCached))
        vkFlags |= VK_MEMORY_PROPERTY_HOST_CACHED_BIT;
    if (properties & static_cast<uint32_t>(HalMemoryPropertyBits::LazilyAllocated))
        vkFlags |= VK_MEMORY_PROPERTY_LAZILY_ALLOCATED_BIT;

    return vkFlags;
}

VkIndexType
VulkanTypeConversion::ConvertIndexTypeToVulkan(HalIndexType indexType)
{
    VkIndexType vkIndexType;

    switch (indexType)
    {
    case HalIndexType::UInt16:
        vkIndexType = VK_INDEX_TYPE_UINT16;
        break;
    case  HalIndexType::UInt32:
        vkIndexType = VK_INDEX_TYPE_UINT32;
        break;
    default:
        vkIndexType = VK_INDEX_TYPE_UINT16;
        assert(false);
        break;
    }

    return vkIndexType;
}

VkPrimitiveTopology
VulkanTypeConversion::ConvertPrimitiveTopologyToVulkan(HalPrimitiveTopology topology)
{
    VkPrimitiveTopology vkTopology = VK_PRIMITIVE_TOPOLOGY_POINT_LIST;

    switch (topology)
    {
    case HalPrimitiveTopology::PointList:
        vkTopology = VK_PRIMITIVE_TOPOLOGY_POINT_LIST;
        break;
    case HalPrimitiveTopology::LineList:
        vkTopology = VK_PRIMITIVE_TOPOLOGY_LINE_LIST;
        break;
    case HalPrimitiveTopology::LineStrip:
        vkTopology = VK_PRIMITIVE_TOPOLOGY_LINE_STRIP;
        break;
    case HalPrimitiveTopology::TriangleList:
        vkTopology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        break;
    case HalPrimitiveTopology::TriangleStrip:
        vkTopology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;
        break;
    case HalPrimitiveTopology::TriangleFan:
        vkTopology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_FAN;
        break;
    case HalPrimitiveTopology::LineListWidthAdjacency:
        vkTopology = VK_PRIMITIVE_TOPOLOGY_LINE_LIST_WITH_ADJACENCY;
        break;
    case HalPrimitiveTopology::LineStripWidthAdjacency:
        vkTopology = VK_PRIMITIVE_TOPOLOGY_LINE_STRIP_WITH_ADJACENCY;
        break;
    case HalPrimitiveTopology::TriangleListWidthAdjacency:
        vkTopology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST_WITH_ADJACENCY;
        break;
    case HalPrimitiveTopology::TriangleStripWidthAdjacency:
        vkTopology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP_WITH_ADJACENCY;
        break;
    case HalPrimitiveTopology::PatchList:
        vkTopology = VK_PRIMITIVE_TOPOLOGY_PATCH_LIST;
        break;
    default:
        vkTopology = VK_PRIMITIVE_TOPOLOGY_POINT_LIST;
        assert(false);
        break;
    }

    return vkTopology;
}

VkDescriptorType
VulkanTypeConversion::ConvertDescriptorTypeToVulkan(HalDescriptorType descriptorType)
{
    VkDescriptorType vkDescriptorType;

    switch (descriptorType)
    {
    case HalDescriptorType::Sampler:
        vkDescriptorType = VK_DESCRIPTOR_TYPE_SAMPLER;
        break;
    case HalDescriptorType::CombinedImageSampler:
        vkDescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        break;
    case HalDescriptorType::SampledImage:
        vkDescriptorType = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
        break;
    case HalDescriptorType::StorageImage:
        vkDescriptorType = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
        break;
    case HalDescriptorType::UniformTexelBuffer:
        vkDescriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER;
        break;
    case HalDescriptorType::StorageTexelBuffer:
        vkDescriptorType = VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER;
        break;
    case HalDescriptorType::UniformBuffer:
        vkDescriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        break;
    case HalDescriptorType::StorageBuffer:
        vkDescriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
        break;
    case HalDescriptorType::UniformBufferDynamic:
        vkDescriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
        break;
    case HalDescriptorType::StorageBufferDynamic:
        vkDescriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC;
        break;
    case HalDescriptorType::InputAttachment:
        vkDescriptorType = VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT;
        break;
    default:
        vkDescriptorType = VK_DESCRIPTOR_TYPE_SAMPLER;
        assert(false);
        break;
    }

    return vkDescriptorType;
}

VkDescriptorPoolCreateFlags
VulkanTypeConversion::ConvertDescriptorPoolFlagsToVulkan(HalDescriptorPoolCreateFlags poolFlags)
{
    VkDescriptorPoolCreateFlags vkFlags = 0;

    if (poolFlags & static_cast<uint32_t>(HalDescriptorPoolCreateFlagBits::FreeDescriptorSet))
        vkFlags |= VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;

    return vkFlags;
}

VkOffset3D 
VulkanTypeConversion::ConvertOffset3DToVulkan(HalOffset3D& offset3D)
{
    return VkOffset3D{ offset3D._x, offset3D._y, offset3D._z };
}

VkExtent3D
VulkanTypeConversion::ConvertExtent3DToVulkan(HalExtent3D& extent3D)
{
    return VkExtent3D{ extent3D._width, extent3D._height, extent3D._depth };
}

VkAccessFlags 
VulkanTypeConversion::GetAccessFlagsFromLayout(HalImageLayout layout)
{
    VkAccessFlags vkFlags = 0;

    if (layout == HalImageLayout::ColorAttachment)
        vkFlags = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_COLOR_ATTACHMENT_READ_BIT;
    else if (layout == HalImageLayout::DepthStencilAttachment)
        vkFlags = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT;
    else if (layout == HalImageLayout::DepthStencilReadOnly)
        vkFlags = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT;

    return vkFlags;
}

VkFenceCreateFlags 
VulkanTypeConversion::ConvertFenceCreateFlagsToVulkan(HalFenceCreateFlags createFlags)
{
    VkFenceCreateFlags vkFlags = 0;

    if (createFlags & static_cast<uint32_t>(HalFenceCreateBits::Signaled))
        vkFlags |= VK_FENCE_CREATE_SIGNALED_BIT;

    return vkFlags;
}

}
