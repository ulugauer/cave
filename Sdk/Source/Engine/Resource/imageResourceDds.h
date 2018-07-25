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

/// @file imageResourceDds.h
///       Handles material assets

#include "imageResource.h"
#include "halTypes.h"

/** \addtogroup engine
*  @{
*
*/

namespace cave
{

/** The maximum number of mipmap levels (per texture or per cubemap face) */
#define DDS_MAX_MIPMAPS       (16)

/** The number of cubemap faces that must exist in a cubemap-bearing DDS file */
#define DDS_NUM_CUBEMAP_FACES (6)



/** The master DDS structure for loading and saving */
struct DDSImageInfo
{
	/** Width of the overall texture in texels */
	uint32_t width;

	/** Height of the overall texture in texels */
	uint32_t height;

	/** Number of color/alpha components per texel 1-4 */
	uint32_t components;

	/** The GL type of each color component (noncompressed textures only) */
	HalComponentType componentFormat;

	/** The number of bytes per pixel (noncompressed textures only) */
	uint32_t bytesPerPixel;

	/** Nonzero if the format is DXT-compressed */
	uint32_t compressed;

	/** The number of levels in the mipmap pyramid (including the base level) */
	uint32_t numMipmaps;

	/** If nonzero, then the file contains 6 cubemap faces */
	bool cubemap;

	/** The texture format of the loaded texture data */
	HalImageFormat format;

	/** Nonzero if the texture data includes alpha */
	uint32_t alpha;

	/** Base of the allocated block of all texel data */
	int8_t *dataBlock;

	/** Pointers to the mipmap levels for the texture or each cubemap face */
	int8_t *data[DDS_MAX_MIPMAPS * DDS_NUM_CUBEMAP_FACES];

	/** Array of sizes of the mipmap levels for the texture or each cubemap face */
	uint32_t   size[DDS_MAX_MIPMAPS * DDS_NUM_CUBEMAP_FACES];

	/** Array of widths of the mipmap levels for the texture or each cubemap face */
	uint32_t mipwidth[DDS_MAX_MIPMAPS * DDS_NUM_CUBEMAP_FACES];

	/** Array of heights of the mipmap levels for the texture or each cubemap face */
	uint32_t mipheight[DDS_MAX_MIPMAPS * DDS_NUM_CUBEMAP_FACES];
};



/**
* Load image assets
*/
class CAVE_INTERFACE ImageResourceDds : public ImageResource
{
public:
	/**
	* @brief Constructor
	*
	* @param[in] rm	Pointer to resource manager
	*
	*/
	ImageResourceDds(ResourceManagerPrivate* rm);

	/** @brief Destructor */
	virtual ~ImageResourceDds();


	/*
	* @brief Query the image host data.
	*
	* @return a struct with information about the image and a pointer to the host data
	*/
	ImageData getImageData();

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
	ImageLevelData getLevelData(uint32_t face, uint32_t mipLevel);

	/*
	* @brief Relase all internal memory allocated
	*/
	void releaseImageData();

protected:
	/*
	* @brief Load an image from the specifed stream.
	*		 All classes derived from this must provide this function
	*
	* @param[in] flipVertical	Flipe image vertical
	* @param[in] dataStream		A valid file stream
	*
	* @return true if successfuly loaded
	*/
	bool decode(bool flipVertical, std::ifstream& dataStream);

private:
	DDSImageInfo m_imageInfo;	///< DDS image data and info
};

}

/** @}*/



