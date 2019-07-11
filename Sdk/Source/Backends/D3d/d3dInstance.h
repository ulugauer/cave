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
#pragma once

/// @file d3dInstance.h
///       D3D instance

#include "halInstance.h"
#include "osPlatformLib.h"

// DirectX 12 specific headers.
#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>

/** \addtogroup backend
*  @{
*
*/

namespace cave
{

    /**
    * Vulkan device instance
    */
    class D3dInstance : public HalInstance
    {
    public:
        /**
        * @brief Constructor
        *
        * @param allocator	Pointer to global allocator
        * @param type	Backend graphics API
        * @param applicationName	Name of application (optional)
        *
        */
        D3dInstance(std::shared_ptr<AllocatorBase> allocator, BackendInstanceTypes type, const char* applicationName);

        /** @brief Destructor */
        virtual ~D3dInstance();

        /**
        * @brief Create a list of physical devices supported by this instance
        *
        * @return True if a list was successfuly build
        */
        bool QueryPhysicalDevices();

        /**
        * @brief Create a hardware render device
        *
        * @param[in] allocator	Engine global allocator
        * @param[in] swapChainInfo	Swap chain creation info
        *
        * @return Pointer to a hardware render device
        */
        HalRenderDevice* CreateRenderDevice(std::shared_ptr<AllocatorBase> allocator, SwapChainInfo& swapChainInfo) override;

        /**
        * @brief Get D3D dxgi handle
        *
        * @return Lowlevel vulkan handle
        */
        IDXGIFactory4* GetDxgiHandle() { return _dxgiFactory.Get(); }

        /**
        * @brief Query if tearing is supported 
        *
        * @return true if tearing is supported
        */
        bool GetTearingSupport() { return _tearing; }

    private:
        /**
        * @brief Create a presentation surface
        *
        * @param[in] swapChainInfo	Swap chain creation info
        *
        * @return Vulkan handle to surface
        */
        //VkSurfaceKHR CreatePresentaionSurface(SwapChainInfo& swapChainInfo);

    private:
        Microsoft::WRL::ComPtr<IDXGIAdapter4> _dxgiAdapter4;    ///< com ptr to dxgi adapter
        Microsoft::WRL::ComPtr<IDXGIFactory4> _dxgiFactory;     ///< comp ptr to dxgi interface
        uint32_t _physicalDeviceCount; ///< Physical devices accessible by this instance
        std::string _appName; ///< application name
        bool _tearing; ///< true if tearing (vsync off) is supported
    };

}

/** @}*/

