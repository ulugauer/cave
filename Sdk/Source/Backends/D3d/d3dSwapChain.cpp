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

#include <limits>

using namespace Microsoft::WRL;

namespace cave
{

D3dSwapChain::D3dSwapChain(D3dInstance* instance, Dx12RenderDevice* renderDevice, SwapChainInfo& swapChainInfo)
    : _pInstance(instance)
    , _pRenderDevice(renderDevice)
    , _swapChain4(nullptr)
    , _descriptorPool(nullptr)
{
    DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};

    swapChainDesc.Width = swapChainInfo.surfaceWidth;
    swapChainDesc.Height = swapChainInfo.surfaceHeight;
    swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.Stereo = FALSE;
    swapChainDesc.SampleDesc = { 1, 0 };
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.BufferCount = 3;
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
    desc.NumDescriptors = 3;
    desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
    Microsoft::WRL::ComPtr<ID3D12Device2> device = renderDevice->GetDeviceHandle();
    res = device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&_descriptorPool));
    if (FAILED(res))
        throw BackendException("Failed to create swap resource descriptor heap");
}

D3dSwapChain::~D3dSwapChain()
{

}

}
