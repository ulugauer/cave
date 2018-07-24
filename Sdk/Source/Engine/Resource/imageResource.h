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

/// @file imageResource.h
///       Handles image assets

#include "resourceManagerPrivate.h"
#include "halTypes.h"

/** \addtogroup engine
*  @{
*
*/

namespace cave
{

/**
* Contains image data for the texture
*/
struct ImageData
{
	uint32_t width;	///< mip 0 width
	uint32_t height;	///< mip 0 height
	uint32_t levels;	///< how many levels does this texture have (minimum 1)
	uint32_t arraySize;	///< arrary count of this texture (minimum 1)
	HalImageFormat format; ///< image format
	uint32_t componentCount;	///< amount of components (1-4)
	uint32_t componentSize; ///< size of a single component in bytes (1-4)
	uint8_t* data;	///< pointer to host data of that level
};

/**
* Contains image data for a single mip level
*/
struct ImageLevelData
{
	uint32_t width;	///< mip width
	uint32_t height;	///< mip height
	HalImageFormat format; ///< image format
	uint32_t componentCount;	///< amount of components (1-4)
	uint32_t componentSize; ///< size of a single component in bytes (1-4)
	uint8_t* data;	///< pointer to host data of that level
};

/**
* Load image assets
*/
class CAVE_INTERFACE ImageResource
{
public:
	/**
	* @brief Constructor
	*
	* @param[in] rm	Pointer to resource manager
	*
	*/
	ImageResource(ResourceManagerPrivate* rm);

	/** @brief Destructor */
	virtual ~ImageResource();

	/**
	* @brief Check if shader languge is supported
	*
	* @param[in] objectFinder	Helper class to find resource
	* @param ext	Filename extension (dds, png, ...)
	*
	* @return true if supported
	*/
	static bool IsImageFormatSupported(ResourceObjectFinder& objectFinder, const char* ext);

	/*
	* @brief Load an image from the specifed stream.
	*		 All classes derived from this must provide this function
	*
	* @param[in] flipVertical	Flipe image vertical
	* @param[in] dataStream		A valid file stream
	*
	* @return true if successfuly loaded
	*/
	virtual bool decode(bool flipVertical, std::ifstream& dataStream) = 0;

	/*
	* @brief Query the image host data.
	*
	* @return a struct with information about the image and a pointer to the host data
	*/
	virtual ImageData getImageData() = 0;

	/*
	* @brief Query the image host data at specified cube face and mip level.
	* If face > 0 it must be a cube map.
	* mipLevel must be smaller than the max mip level of the image.
	*
	* @param[in] face		Index of the cube face
	* @param[in] mipLevel	Mip level of the requested data
	*
	* @return Return a struct with information about the level and a pointer to host data
	*/
	virtual ImageLevelData getLevelData(uint32_t face, uint32_t mipLevel) = 0;

	/*
	* @brief Relase all internal memory allocated
	*/
	virtual void releaseImageData() = 0;


protected:
	ResourceManagerPrivate * _pResourceManagerPrivate;	///< Pointer to private resource manger
};

}

/** @}*/


