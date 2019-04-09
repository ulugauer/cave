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

/// @file vulkanSampler.cpp
///       Vulkan device sampler

#include "vulkanSampler.h"
#include "vulkanRenderDevice.h"
#include "vulkanConversion.h"

#include "vulkanApi.h"

#include<limits>
#include<cstring>

namespace cave
{

VulkanSampler::VulkanSampler(VulkanRenderDevice* device, const HalSamplerCreateInfo& samplerInfo)
    : HalSampler(device, samplerInfo)
    , _pDevice(device)
    , _vkSampler(VK_NULL_HANDLE)
{
    bool anisotropic = samplerInfo._anisotropyEnable && AnisotropicFilteringSupported();

    _vkSamplerCreateInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    _vkSamplerCreateInfo.pNext = nullptr;
    _vkSamplerCreateInfo.flags = 0;
    _vkSamplerCreateInfo.addressModeU = VulkanTypeConversion::ConvertSamplerAddressModeToVulkan(samplerInfo._addressModeU);
    _vkSamplerCreateInfo.addressModeV = VulkanTypeConversion::ConvertSamplerAddressModeToVulkan(samplerInfo._addressModeV);
    _vkSamplerCreateInfo.addressModeW = VulkanTypeConversion::ConvertSamplerAddressModeToVulkan(samplerInfo._addressModeW);
    _vkSamplerCreateInfo.anisotropyEnable = anisotropic;
    _vkSamplerCreateInfo.borderColor = VulkanTypeConversion::ConvertBorderColorToVulkan(samplerInfo._borderColor);
    _vkSamplerCreateInfo.compareEnable = samplerInfo._compareEnable;
    _vkSamplerCreateInfo.compareOp = VulkanTypeConversion::ConvertDepthCompareOpToVulkan(samplerInfo._compareOp);
    _vkSamplerCreateInfo.magFilter = VulkanTypeConversion::ConvertFilterToVulkan(samplerInfo._magFilter);
    _vkSamplerCreateInfo.minFilter = VulkanTypeConversion::ConvertFilterToVulkan(samplerInfo._minFilter);
    _vkSamplerCreateInfo.maxAnisotropy = samplerInfo._maxAnisotropy;
    _vkSamplerCreateInfo.maxLod = samplerInfo._maxLod;
    _vkSamplerCreateInfo.minLod = samplerInfo._minLod;
    _vkSamplerCreateInfo.mipLodBias = samplerInfo._mipLodBias;
    _vkSamplerCreateInfo.mipmapMode = VulkanTypeConversion::ConvertSamplerMipmapModeToVulkan(samplerInfo._mipmapMode);
    _vkSamplerCreateInfo.unnormalizedCoordinates = samplerInfo._unnormalizedCoordinates;

    VkResult result = VulkanApi::GetApi()->vkCreateSampler(_pDevice->GetDeviceHandle(), &_vkSamplerCreateInfo, nullptr, &_vkSampler);
    if (result != VK_SUCCESS)
        throw BackendException("Error failed to create vulkan sampler");

}

VulkanSampler::~VulkanSampler()
{
    if (_vkSampler)
        VulkanApi::GetApi()->vkDestroySampler(_pDevice->GetDeviceHandle(), _vkSampler, nullptr);
}


}
