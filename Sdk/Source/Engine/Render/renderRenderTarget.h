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

/// @file renderRenderTarget.h
///       Render target interface

#include "Common/caveRefCount.h"
#include "Common/caveString.h"
#include "Memory/allocatorGlobal.h"
#include "halTypes.h"

#include <memory>

/** \addtogroup engine
*  @{
*		This module contains all code related to the engine
*/

namespace cave
{

/// forward declaration
class RenderDevice;
class HalImage;
class HalImageView;


/**
* @brief Represents a render target interface
*/
class CAVE_INTERFACE RenderTarget : public CaveRefCount
{
public:

    /**
    * @brief Constructor
    *
    * @param[in] renderDevice		Pointer to render device
    * @param[in] imageInfo			HAL image create info
    *
    */
    RenderTarget(RenderDevice& renderDevice, HalImageInfo& imageInfo);
    /** @brief destructor */
    virtual ~RenderTarget();

    /**
    * @brief Backs the buffer with memory
    *
    */
    virtual void Bind();

    /**
    * @brief Get low level HAL handle
    *
    * @return HalImage handle
    */
    HalImage* GetImageHandle() { return _halImage; }

    /**
    * @brief Get low level HAL handle
    *
    * @return HalImageView handle
    */
    HalImageView* GetImageViewHandle() { return _halImageView; }

    /**
    * @brief Query image format
    *
    * @retrun HalImageFormat enum
    *
    */
    HalImageFormat GetImageFormat();

protected:
    RenderDevice & _renderDevice;	///< Render device object
    HalImage* _halImage;			///< Pointer to low level data image object
    HalImageView* _halImageView;    ///< Pointer to low level image view object
};

}

/** @}*/
