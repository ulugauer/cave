/*
Copyright (c) <2019> <Udo Lugauer>
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

/// @file d3dSwapChain.h
///       D3D swap chain

#include "halRenderDevice.h"
#include "osPlatformLib.h"
#include "Common/caveVector.h"

// DirectX 12 specific headers.
#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>

#include <vector>
#include <array>

/** \addtogroup backend
*  @{
*
*/

namespace cave
{

///< forwards
class D3dInstance;
class Dx12RenderDevice;

/**
* Vulkan swap chain handling
*/
class D3dSwapChain
{
    static constexpr uint32_t kSwapChainImageCount = 3;
public:
    /**
    * @brief Constructor
    *
    * @param[in] instance	Pointer to instance
    * @param[in] renderDevice	Pointer to render device
    * @param[in] swapChainInfo	Pointer SwapChainInfo struct
    *
    */
    D3dSwapChain(D3dInstance* instance, Dx12RenderDevice* renderDevice, SwapChainInfo& swapChainInfo);

    /** @brief Destructor */
    virtual ~D3dSwapChain();

    /**
    * @brief Get swap image count
    *
    * @return swap image count
    */
    const uint32_t GetSwapChainImageCount() const { return kSwapChainImageCount; }

private:
    /**
    * @brief Create swap chain image views
    *
    */
    void CreateImageViews(Dx12RenderDevice* renderDevice);

private:
    D3dInstance* _pInstance;    ///< Pointer to instance
    Dx12RenderDevice* _pRenderDevice;   ///< Handle to render device
    Microsoft::WRL::ComPtr<IDXGISwapChain4> _swapChain4;    ///< Handle to a d3d swap chain
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> _descriptorPool;   ///< descripor allocation pool for swap surfaces
    Microsoft::WRL::ComPtr<ID3D12Fence> _swapFence; ///< sync object for swapchain GPU
    HANDLE _swapFenceEvent; ///< sync object for swapchain CPU
    std::array<Microsoft::WRL::ComPtr<ID3D12Resource>, kSwapChainImageCount> _swapChainImageVector;   ///< Array of swap chain images
};

}
