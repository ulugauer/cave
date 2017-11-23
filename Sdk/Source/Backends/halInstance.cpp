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

/// @file halInstance.cpp
///       Hardware instance abstraction

#include "Memory/allocatorBase.h"
#include "halInstance.h"
#include "vulkanInstance.h"
#include "vulkanRenderDevice.h"

namespace cave
{

HalInstance::HalInstance(std::shared_ptr<AllocatorBase> allocator, BackendInstanceTypes type)
	: _allocator(allocator)
	, _type(type)
{

}

HalInstance::~HalInstance()
{

}

HalInstance* HalInstance::CreateInstance(std::shared_ptr<AllocatorBase> allocator, BackendInstanceTypes type, const char* applicationName)
{
	HalInstance* pHalInstance = nullptr;
	if (type == BackendInstanceTypes::InstanceVulkan)
		pHalInstance = AllocateObject<VulkanInstance>(*allocator, allocator, type, applicationName);

	if (pHalInstance)
		pHalInstance->QueryPhysicalDevices();

	return pHalInstance;
}

void HalInstance::ReleaseInstance(std::shared_ptr<AllocatorBase> allocator, HalInstance* instance)
{
	if (instance)
		DeallocateDelete(*allocator, *instance);
}

}
