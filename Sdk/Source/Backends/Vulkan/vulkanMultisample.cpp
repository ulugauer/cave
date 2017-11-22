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

/// @file vulkanMultisample.cpp
///       Vulkan multisample state

#include "vulkanMultisample.h"
#include "vulkanRenderDevice.h"
#include "vulkanApi.h"

#include<limits>

namespace cave
{

/**
* @brief Convert from generic cull mode to vulkan cull mode
*
* @param[in] mode	Generic cull mode
*
* @return Vulkan cull mode
*/
static VkSampleCountFlagBits ConvertCullModeToVulkan(SampleCount samples)
{
	VkSampleCountFlagBits sampleCount = VK_SAMPLE_COUNT_1_BIT;

	switch (samples)
	{
	case SampleCount::SampleCount1:
		sampleCount = VK_SAMPLE_COUNT_1_BIT;
		break;
	case SampleCount::SampleCount2:
		sampleCount = VK_SAMPLE_COUNT_2_BIT;
		break;
	case SampleCount::SampleCount4:
		sampleCount = VK_SAMPLE_COUNT_4_BIT;
		break;
	case SampleCount::SampleCount8:
		sampleCount = VK_SAMPLE_COUNT_8_BIT;
		break;
	case SampleCount::SampleCount16:
		sampleCount = VK_SAMPLE_COUNT_16_BIT;
		break;
	case SampleCount::SampleCount32:
		sampleCount = VK_SAMPLE_COUNT_32_BIT;
		break;
	case SampleCount::SampleCount64:
		sampleCount = VK_SAMPLE_COUNT_64_BIT;
		break;
	default:
		sampleCount = VK_SAMPLE_COUNT_1_BIT;
		break;
	}

	return sampleCount;
}

VulkanMultisample::VulkanMultisample(VulkanRenderDevice* device, HalMultisampleData& multisampleData)
	: HalMultisample(multisampleData)
	, _pDevice(device)
{
	_multisampleStateStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	_multisampleStateStateInfo.pNext = nullptr;
	_multisampleStateStateInfo.flags = 0;
	_multisampleStateStateInfo.alphaToCoverageEnable = multisampleData._alphaToCoverageEnable;
	_multisampleStateStateInfo.alphaToOneEnable = multisampleData._alphaToOneEnable;
	_multisampleStateStateInfo.minSampleShading = multisampleData._minSampleShading;
	_multisampleStateStateInfo.pSampleMask = static_cast<VkSampleMask*>(multisampleData._pSampleMask);
	_multisampleStateStateInfo.rasterizationSamples = ConvertCullModeToVulkan(multisampleData._rasterizationSamples);
	_multisampleStateStateInfo.sampleShadingEnable = multisampleData._sampleShadingEnable;
}

VulkanMultisample::~VulkanMultisample()
{
}


}
