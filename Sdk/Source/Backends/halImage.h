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

/// @file halBuffer.h
///       Hardware data buffer abstraction

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
* @brief Abstraction of device images
*/
class HalImage
{
public:
	/**
	* @brief Constructor
	*
	* @param[in] renderDevice	Pointer to render device object
	* @param[in] imageInfo		Image create info
	*/
	HalImage(HalRenderDevice* renderDevice, HalImageInfo& imageInfo);

	/** @brief Destructor */
	virtual ~HalImage();

	/**
	* @brief Bind Binds a image and allocates memory
	*
	*/
	virtual void Bind() = 0;

	/**
	* @brief Copy data to device memory
	*
	* @param[in] pData		Pointer to image data
	*
	*/
	virtual void Update(const void* pData) = 0;

    /**
    * @brief Query image format
    *
    * @retrun HalImageFormat enum
    *
    */
    HalImageFormat GetImageFormat();

    /**
    * @brief Query image level count
    *
    * @retrun amount mip level
    *
    */
    uint32_t GetLevelCount();

    /**
    * @brief Query image color/depth type
    *
    * @retrun true if image has a depth format
    *
    */
    bool IsDepthFormat();

    /**
    * @brief Query if image is stencil
    *
    * @retrun true if image has a stencil format
    *
    */
    bool IsStencilFormat();

private:
	HalRenderDevice * _pDevice;	///< Pointer to device object
	HalImageInfo _imageInfo;	///< Buffer create info
};

}

/** @}*/




