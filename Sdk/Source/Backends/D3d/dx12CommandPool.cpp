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

/// @file dx12CommandPool.cpp
///       Dx12 command pool handling

#include "dx12CommandPool.h"
#include "dx12RenderDevice.h"
#include "dxConversion.h"

#include<limits>

namespace cave
{

Dx12CommandPool::Dx12CommandPool(Dx12RenderDevice* renderDevice, HalCommandPoolInfo& commandPoolInfo)
    : HalCommandPool(renderDevice, commandPoolInfo)
    , _pDevice(renderDevice)
    , _dx12CommandPool(nullptr)
{
    ID3D12Device2* device = renderDevice->GetDeviceHandle();

    HRESULT res = device->CreateCommandAllocator(DxTypeConversion::ConvertCommandPoolUsageFlagsToDx(commandPoolInfo._flags), 
        IID_PPV_ARGS(&_dx12CommandPool));
    if (FAILED(res))
        throw BackendException("Failed to create command pool");
}

Dx12CommandPool::~Dx12CommandPool()
{
}

}
