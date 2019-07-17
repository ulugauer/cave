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

/// @file d3dInstance.cpp
///       D3D instance

#include "d3dInstance.h"
#include "dx12RenderDevice.h"

using namespace Microsoft::WRL;

#include <iostream>
#include <vector>
#include <cstring>

namespace cave
{

    D3dInstance::D3dInstance(std::shared_ptr<AllocatorBase> allocator, BackendInstanceTypes type, const char* applicationName)
        : HalInstance(allocator, type)
        , _dxgiAdapter4(nullptr)
        , _dxgiFactory(nullptr)
        , _physicalDeviceCount(0)
        , _appName(applicationName)
        , _tearing(false)
    {
#if defined(_DEBUG)
        // Always enable the debug layer before doing anything DX12 related
        // so all possible errors generated while creating DX12 objects
        // are caught by the debug layer.
        ComPtr<ID3D12Debug> debugInterface;
        if (D3D12GetDebugInterface(IID_PPV_ARGS(&debugInterface)) < 0)
        {
            throw BackendException("Failed to create D3D12 debug interface");
        }
        debugInterface->EnableDebugLayer();
#endif

        UINT createFactoryFlags = 0;
#if defined(_DEBUG)
        createFactoryFlags = DXGI_CREATE_FACTORY_DEBUG;
#endif
        HRESULT res = CreateDXGIFactory2(createFactoryFlags, IID_PPV_ARGS(&_dxgiFactory));
        if (FAILED(res))
        {
            throw BackendException("Failed to create DXGI interface");
        }

        // iterate through all HW adapters. We don support SW adapters
        ComPtr<IDXGIAdapter1> dxgiAdapter1;
        SIZE_T maxDedicatedVideoMemory = 0;
        for (uint32_t i = 0; _dxgiFactory->EnumAdapters1(i, &dxgiAdapter1) != DXGI_ERROR_NOT_FOUND; ++i)
        {
            DXGI_ADAPTER_DESC1 dxgiAdapterDesc1;
            dxgiAdapter1->GetDesc1(&dxgiAdapterDesc1);

            // Check to see if the adapter can create a D3D12 device without actually 
            // creating it. The adapter with the largest dedicated video memory
            // is favored.
            if ((dxgiAdapterDesc1.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) == 0 &&
                SUCCEEDED(D3D12CreateDevice(dxgiAdapter1.Get(),
                    D3D_FEATURE_LEVEL_11_0, __uuidof(ID3D12Device), nullptr)) &&
                dxgiAdapterDesc1.DedicatedVideoMemory > maxDedicatedVideoMemory)
            {
                maxDedicatedVideoMemory = dxgiAdapterDesc1.DedicatedVideoMemory;
                if (dxgiAdapter1.As(&_dxgiAdapter4) < 0)
                {
                    throw BackendException("Failed to find suitable adapter");
                }
                _physicalDeviceCount++;
                if (dxgiAdapterDesc1.VendorId == 0x10DE)
                    _vendorName = "NVIDIA";
                else if (dxgiAdapterDesc1.VendorId == 0x1002)
                    _vendorName = "AMD";
                else if (dxgiAdapterDesc1.VendorId == 0x0300)
                    _vendorName = "INTEL";
            }
        }

        // check for vsync off feature
        ComPtr<IDXGIFactory4> factory4;
        if (SUCCEEDED(CreateDXGIFactory1(IID_PPV_ARGS(&factory4))))
        {
            ComPtr<IDXGIFactory5> factory5;
            if (SUCCEEDED(factory4.As(&factory5)))
            {
                res = factory5->CheckFeatureSupport(DXGI_FEATURE_PRESENT_ALLOW_TEARING, &_tearing, sizeof(_tearing));
                if (FAILED(res))
                {
                    _tearing = false;
                }
            }
        }
    }

    D3dInstance::~D3dInstance()
    {
    }

    bool D3dInstance::QueryPhysicalDevices()
    {
        return (_physicalDeviceCount > 0);
    }

    HalRenderDevice* D3dInstance::CreateRenderDevice(std::shared_ptr<AllocatorBase> allocator, SwapChainInfo& )
    {
        if (_dxgiAdapter4 == nullptr)
            throw BackendException("CreateRenderDevice: no suitable adapter found");

        // We have something suitable -> create device
        Dx12RenderDevice* renderDevice = AllocateObject<Dx12RenderDevice>(*allocator, this, _dxgiAdapter4.Get());

        return renderDevice;
    }

}
