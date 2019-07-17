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

/// @file d3dSwapChain.cpp
///       D3D swap chain

#include "d3dSwapChain.h"
#include "d3dInstance.h"
#include "dx12RenderDevice.h"

#include "d3dx12.h"

#include <limits>

using namespace Microsoft::WRL;

namespace cave
{

D3dSwapChain::D3dSwapChain(D3dInstance* instance, Dx12RenderDevice* renderDevice, SwapChainInfo& swapChainInfo)
    : _pInstance(instance)
    , _pRenderDevice(renderDevice)
    , _swapChain4(nullptr)
    , _descriptorPool(nullptr)
    , _swapFence(nullptr)
    , _swapFenceEvent(nullptr)
{
    DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};

    swapChainDesc.Width = swapChainInfo.surfaceWidth;
    swapChainDesc.Height = swapChainInfo.surfaceHeight;
    swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.Stereo = FALSE;
    swapChainDesc.SampleDesc = { 1, 0 };
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.BufferCount = D3dSwapChain::kSwapChainImageCount;
    swapChainDesc.Scaling = DXGI_SCALING_STRETCH;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
    swapChainDesc.Flags = instance->GetTearingSupport() ? DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING : 0;

    IDXGIFactory4* dxgiFactory4 = instance->GetDxgiHandle();
    ComPtr<IDXGISwapChain1> swapChain1;
    HRESULT res = dxgiFactory4->CreateSwapChainForHwnd(
        renderDevice->GetGraphicsQueue(),
        swapChainInfo.hWindow,
        &swapChainDesc,
        nullptr,
        nullptr,
        &swapChain1);

    if (FAILED(res))
        throw BackendException("Failed to create DXGI swap chain");

    // Disable the Alt+Enter fullscreen toggle feature for now
    res = dxgiFactory4->MakeWindowAssociation(swapChainInfo.hWindow, DXGI_MWA_NO_ALT_ENTER);
    res = swapChain1.As(&_swapChain4);
    if (FAILED(res))
        throw BackendException("Failed to create DXGI swap chain");

    D3D12_DESCRIPTOR_HEAP_DESC desc = {};
    desc.NumDescriptors = D3dSwapChain::kSwapChainImageCount;
    desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
    ID3D12Device2* device = renderDevice->GetDeviceHandle();
    res = device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&_descriptorPool));
    if (FAILED(res))
        throw BackendException("Failed to create swap resource descriptor heap");

    CreateImageViews(renderDevice);

    res = device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&_swapFence));
    if (FAILED(res))
        throw BackendException("Failed to create swap fence");

    // CPU part of syncing
    _swapFenceEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
    if (!_swapFenceEvent)
        throw BackendException("Failed to create swap fence event");
}

D3dSwapChain::~D3dSwapChain()
{

}

void D3dSwapChain::CreateImageViews(Dx12RenderDevice* renderDevice)
{
    const Dx12DeviceCaps& caps = renderDevice->GetDeviceCaps();
    ID3D12Device2* device = renderDevice->GetDeviceHandle();

    CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(_descriptorPool->GetCPUDescriptorHandleForHeapStart());

    HRESULT res;
    for (uint32_t i = 0; i < D3dSwapChain::kSwapChainImageCount; ++i)
    {
        ComPtr<ID3D12Resource> backBuffer;
        res = _swapChain4->GetBuffer(i, IID_PPV_ARGS(&backBuffer));
        if (FAILED(res))
            throw BackendException("Failed to query back buffer");

        device->CreateRenderTargetView(backBuffer.Get(), nullptr, rtvHandle);
        _swapChainImageVector[i] = backBuffer;

        rtvHandle.Offset(caps.rtvDescriptorSize);
    }
}


}
