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

/// @file renderTexture.h
///       Render data buffer interface

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


/**
* @brief Represents a texture interface
*/
class CAVE_INTERFACE RenderTexture : public CaveRefCount
{
public:

	/**
	* @brief Constructor
	*
	* @param[in] renderDevice		Pointer to render device
	* @param[in] imageInfo			HAL image create info
	* @param[in] texture			File name
	*
	*/
	RenderTexture(RenderDevice& renderDevice, HalImageInfo& imageInfo, const char* filename);
	/** @brief destructor */
	virtual ~RenderTexture();

	/**
	* @brief Backs the buffer with memory
	*
	*/
	virtual void Bind();

	/**
	* @brief Copy data to buffer
	*		 Throws an exception at failure
	*
	* @param[in] pData		Pointer to data
	*
	*/
	virtual void Update(const void* pData);

	/**
	* @brief Get file name
	*
	* @return const char string
	*/
	virtual const char* GetFileName()
	{
		return _filename.c_str();
	}

	/**
	* @brief Get low level HAL handle
	*
	* @return HalImage handle
	*/
	HalImage* GetHalHandle() { return _halImage; }

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

protected:
	RenderDevice & _renderDevice;	///< Render device object
	HalImage* _halImage;			///< Pointer to low level data image object
	class CAVE_INTERFACE caveString _filename;	///< texture file name
};

}

/** @}*/





