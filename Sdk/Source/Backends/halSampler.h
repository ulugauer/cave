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

/// @file halSampler.h
///       Hardware image sampler abstraction

#include "engineDefines.h"
#include "Memory/allocatorBase.h"
#include "halTypes.h"

#include <iostream>		// includes exception handling
#include <memory>

/** \addtogroup backend
*  @{
*
*/

namespace cave
{

///< forwards
class HalRenderDevice;

/**
* @brief Abstraction of device image views
*/
class HalSampler
{
public:
    /**
    * @brief Constructor
    *
    * @param[in] renderDevice   Pointer to render device object
    * @param[in] samplerInfo    Sampler create info
    */
    HalSampler(HalRenderDevice* renderDevice, const HalSamplerCreateInfo& samplerInfo);

    /** @brief Destructor */
    virtual ~HalSampler();

    /**
    * @brief Query if anisotropic filtering is supported
    *
    * @return true if available
    */
    bool AnisotropicFilteringSupported();

private:
    HalRenderDevice* _pDevice;	///< Pointer to device object
    HalSamplerCreateInfo _samplerInfo;	///< Sampler create info
};

}

/** @}*/

