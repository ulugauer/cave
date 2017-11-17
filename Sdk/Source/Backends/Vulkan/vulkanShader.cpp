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

/// @file vulkanShader.cpp
///       Vulkan shader module

#include "vulkanShader.h"
#include "vulkanRenderDevice.h"
#include "vulkanApi.h"

#include<limits>
#include<set>

namespace cave
{

VulkanShader::VulkanShader(VulkanRenderDevice* device, ShaderType type, ShaderLanguage language)
	: HalShader(type, language)
	, _pDevice(device)
	, _vkShader(VK_NULL_HANDLE)
{
}

VulkanShader::~VulkanShader()
{
	if (_vkShader != VK_NULL_HANDLE)
		VulkanApi::GetApi()->vkDestroyShaderModule(_pDevice->GetDeviceHandle(), _vkShader, nullptr);
}

bool VulkanShader::CompileShader(const char* code, size_t count)
{
	// setup create info
	VkShaderModuleCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	createInfo.codeSize = count;
	// type is uint32_t for vulkan therefor code must be 4 byte aligned
	createInfo.pCode = reinterpret_cast<const uint32_t*>(code); 

	if (VulkanApi::GetApi()->vkCreateShaderModule(_pDevice->GetDeviceHandle(), &createInfo, nullptr, &_vkShader) != VK_SUCCESS)
	{
		return false;
	}

	return true;
}

}
