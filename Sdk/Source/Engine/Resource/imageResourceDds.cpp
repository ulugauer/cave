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

/// @file materialResource.cpp
///        Handles material assets

#include "imageResourceDds.h"
#include "engineError.h"

#include <fstream>
#include <iostream>
#include <cstring>

namespace cave
{

//-----------------------------------------------------------------------------
// DXT data-layout structure definitions.
//-----------------------------------------------------------------------------
typedef struct
{
	uint16_t col0; // 16-bit 565 interpolant endpoints
	uint16_t col1;
	uint8_t row[4]; // 4x4 * 2bpp color-index == 4 bytes.
} DXTColBlock;

typedef struct
{
	uint16_t row[4]; // 4x4 * 4bpp alpha == 8 bytes.  (pure 4-bit alpha values)
} DXT3AlphaBlock;

typedef struct
{
	uint8_t alpha0; // 8-bit alpha interpolant endpoints
	uint8_t alpha1;
	uint8_t row[6]; // 4x4 * 3bpp alpha-index == 48bits == 6 bytes.
} DXT5AlphaBlock;

typedef struct
{
	uint8_t red;
	uint8_t green;
	uint8_t blue;
	uint8_t alpha;
} Color8888;

//-----------------------------------------------------------------------------
// Various DDS file defines
// from Microsoft
//-----------------------------------------------------------------------------
#ifndef MAKEFOURCC
#define MAKEFOURCC(ch0, ch1, ch2, ch3)                              \
                ((uint32_t)(uint8_t)(ch0) | ((uint32_t)(uint8_t)(ch1) << 8) |       \
                ((uint32_t)(uint8_t)(ch2) << 16) | ((uint32_t)(uint8_t)(ch3) << 24 ))
#endif /* defined(MAKEFOURCC) */ 

#define DDSD_CAPS             0x00000001l
#define DDSD_HEIGHT           0x00000002l
#define DDSD_WIDTH            0x00000004l
#define DDSD_PIXELFORMAT      0x00001000l
#define DDS_ALPHAPIXELS       0x00000001l
#define DDS_FOURCC            0x00000004l
#define DDS_PITCH             0x00000008l
#define DDS_COMPLEX           0x00000008l
#define DDS_RGB               0x00000040l
#define DDS_TEXTURE           0x00001000l
#define DDS_MIPMAPCOUNT       0x00020000l
#define DDS_LINEARSIZE        0x00080000l
#define DDS_VOLUME            0x00200000l
#define DDS_MIPMAP            0x00400000l
#define DDS_DEPTH             0x00800000l

#define DDS_CUBEMAP           0x00000200L
#define DDS_CUBEMAP_POSITIVEX 0x00000400L
#define DDS_CUBEMAP_NEGATIVEX 0x00000800L
#define DDS_CUBEMAP_POSITIVEY 0x00001000L
#define DDS_CUBEMAP_NEGATIVEY 0x00002000L
#define DDS_CUBEMAP_POSITIVEZ 0x00004000L
#define DDS_CUBEMAP_NEGATIVEZ 0x00008000L

#define FOURCC_DXT1 MAKEFOURCC('D','X','T','1')
#define FOURCC_DXT2 MAKEFOURCC('D','X','T','2')
#define FOURCC_DXT3 MAKEFOURCC('D','X','T','3')
#define FOURCC_DXT4 MAKEFOURCC('D','X','T','4')
#define FOURCC_DXT5 MAKEFOURCC('D','X','T','5')
#define FOURCC_DX10 MAKEFOURCC('D','X','1','0')
#define FOURCC_DXGI_FORMAT_R16G16B16A16_FLOAT 0x0000071L
#define FOURCC_DXGI_FORMAT_R32G32B32A32_FLOAT 0x0000074L

#define DDS_MAGIC_FLIPPED     0x0F7166ED 

//-----------------------------------------------------------------------------
// DXGI texture formats for DX10 DDS files
//-----------------------------------------------------------------------------

typedef enum DXGI_FORMAT
{
	DXGI_FORMAT_UNKNOWN = 0,
	DXGI_FORMAT_R32G32B32A32_TYPELESS = 1,
	DXGI_FORMAT_R32G32B32A32_FLOAT = 2,
	DXGI_FORMAT_R32G32B32A32_UINT = 3,
	DXGI_FORMAT_R32G32B32A32_SINT = 4,
	DXGI_FORMAT_R32G32B32_TYPELESS = 5,
	DXGI_FORMAT_R32G32B32_FLOAT = 6,
	DXGI_FORMAT_R32G32B32_UINT = 7,
	DXGI_FORMAT_R32G32B32_SINT = 8,
	DXGI_FORMAT_R16G16B16A16_TYPELESS = 9,
	DXGI_FORMAT_R16G16B16A16_FLOAT = 10,
	DXGI_FORMAT_R16G16B16A16_UNORM = 11,
	DXGI_FORMAT_R16G16B16A16_UINT = 12,
	DXGI_FORMAT_R16G16B16A16_SNORM = 13,
	DXGI_FORMAT_R16G16B16A16_SINT = 14,
	DXGI_FORMAT_R32G32_TYPELESS = 15,
	DXGI_FORMAT_R32G32_FLOAT = 16,
	DXGI_FORMAT_R32G32_UINT = 17,
	DXGI_FORMAT_R32G32_SINT = 18,
	DXGI_FORMAT_R32G8X24_TYPELESS = 19,
	DXGI_FORMAT_D32_FLOAT_S8X24_UINT = 20,
	DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS = 21,
	DXGI_FORMAT_X32_TYPELESS_G8X24_UINT = 22,
	DXGI_FORMAT_R10G10B10A2_TYPELESS = 23,
	DXGI_FORMAT_R10G10B10A2_UNORM = 24,
	DXGI_FORMAT_R10G10B10A2_UINT = 25,
	DXGI_FORMAT_R11G11B10_FLOAT = 26,
	DXGI_FORMAT_R8G8B8A8_TYPELESS = 27,
	DXGI_FORMAT_R8G8B8A8_UNORM = 28,
	DXGI_FORMAT_R8G8B8A8_UNORM_SRGB = 29,
	DXGI_FORMAT_R8G8B8A8_UINT = 30,
	DXGI_FORMAT_R8G8B8A8_SNORM = 31,
	DXGI_FORMAT_R8G8B8A8_SINT = 32,
	DXGI_FORMAT_R16G16_TYPELESS = 33,
	DXGI_FORMAT_R16G16_FLOAT = 34,
	DXGI_FORMAT_R16G16_UNORM = 35,
	DXGI_FORMAT_R16G16_UINT = 36,
	DXGI_FORMAT_R16G16_SNORM = 37,
	DXGI_FORMAT_R16G16_SINT = 38,
	DXGI_FORMAT_R32_TYPELESS = 39,
	DXGI_FORMAT_D32_FLOAT = 40,
	DXGI_FORMAT_R32_FLOAT = 41,
	DXGI_FORMAT_R32_UINT = 42,
	DXGI_FORMAT_R32_SINT = 43,
	DXGI_FORMAT_R24G8_TYPELESS = 44,
	DXGI_FORMAT_D24_UNORM_S8_UINT = 45,
	DXGI_FORMAT_R24_UNORM_X8_TYPELESS = 46,
	DXGI_FORMAT_X24_TYPELESS_G8_UINT = 47,
	DXGI_FORMAT_R8G8_TYPELESS = 48,
	DXGI_FORMAT_R8G8_UNORM = 49,
	DXGI_FORMAT_R8G8_UINT = 50,
	DXGI_FORMAT_R8G8_SNORM = 51,
	DXGI_FORMAT_R8G8_SINT = 52,
	DXGI_FORMAT_R16_TYPELESS = 53,
	DXGI_FORMAT_R16_FLOAT = 54,
	DXGI_FORMAT_D16_UNORM = 55,
	DXGI_FORMAT_R16_UNORM = 56,
	DXGI_FORMAT_R16_UINT = 57,
	DXGI_FORMAT_R16_SNORM = 58,
	DXGI_FORMAT_R16_SINT = 59,
	DXGI_FORMAT_R8_TYPELESS = 60,
	DXGI_FORMAT_R8_UNORM = 61,
	DXGI_FORMAT_R8_UINT = 62,
	DXGI_FORMAT_R8_SNORM = 63,
	DXGI_FORMAT_R8_SINT = 64,
	DXGI_FORMAT_A8_UNORM = 65,
	DXGI_FORMAT_R1_UNORM = 66,
	DXGI_FORMAT_R9G9B9E5_SHAREDEXP = 67,
	DXGI_FORMAT_R8G8_B8G8_UNORM = 68,
	DXGI_FORMAT_G8R8_G8B8_UNORM = 69,
	DXGI_FORMAT_BC1_TYPELESS = 70,
	DXGI_FORMAT_BC1_UNORM = 71,
	DXGI_FORMAT_BC1_UNORM_SRGB = 72,
	DXGI_FORMAT_BC2_TYPELESS = 73,
	DXGI_FORMAT_BC2_UNORM = 74,
	DXGI_FORMAT_BC2_UNORM_SRGB = 75,
	DXGI_FORMAT_BC3_TYPELESS = 76,
	DXGI_FORMAT_BC3_UNORM = 77,
	DXGI_FORMAT_BC3_UNORM_SRGB = 78,
	DXGI_FORMAT_BC4_TYPELESS = 79,
	DXGI_FORMAT_BC4_UNORM = 80,
	DXGI_FORMAT_BC4_SNORM = 81,
	DXGI_FORMAT_BC5_TYPELESS = 82,
	DXGI_FORMAT_BC5_UNORM = 83,
	DXGI_FORMAT_BC5_SNORM = 84,
	DXGI_FORMAT_B5G6R5_UNORM = 85,
	DXGI_FORMAT_B5G5R5A1_UNORM = 86,
	DXGI_FORMAT_B8G8R8A8_UNORM = 87,
	DXGI_FORMAT_B8G8R8X8_UNORM = 88,
	DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM = 89,
	DXGI_FORMAT_B8G8R8A8_TYPELESS = 90,
	DXGI_FORMAT_B8G8R8A8_UNORM_SRGB = 91,
	DXGI_FORMAT_B8G8R8X8_TYPELESS = 92,
	DXGI_FORMAT_B8G8R8X8_UNORM_SRGB = 93,
	DXGI_FORMAT_BC6H_TYPELESS = 94,
	DXGI_FORMAT_BC6H_UF16 = 95,
	DXGI_FORMAT_BC6H_SF16 = 96,
	DXGI_FORMAT_BC7_TYPELESS = 97,
	DXGI_FORMAT_BC7_UNORM = 98,
	DXGI_FORMAT_BC7_UNORM_SRGB = 99,
	DXGI_FORMAT_AYUV = 100,
	DXGI_FORMAT_Y410 = 101,
	DXGI_FORMAT_Y416 = 102,
	DXGI_FORMAT_NV12 = 103,
	DXGI_FORMAT_P010 = 104,
	DXGI_FORMAT_P016 = 105,
	DXGI_FORMAT_420_OPAQUE = 106,
	DXGI_FORMAT_YUY2 = 107,
	DXGI_FORMAT_Y210 = 108,
	DXGI_FORMAT_Y216 = 109,
	DXGI_FORMAT_NV11 = 110,
	DXGI_FORMAT_AI44 = 111,
	DXGI_FORMAT_IA44 = 112,
	DXGI_FORMAT_P8 = 113,
	DXGI_FORMAT_A8P8 = 114,
	DXGI_FORMAT_B4G4R4A4_UNORM = 115,
	DXGI_FORMAT_P208 = 130,
	DXGI_FORMAT_V208 = 131,
	DXGI_FORMAT_V408 = 132,
	DXGI_FORMAT_FORCE_UINT = 0xffffffff
} DXGI_FORMAT;

typedef enum D3D10_RESOURCE_DIMENSION
{
	D3D10_RESOURCE_DIMENSION_UNKNOWN = 0,
	D3D10_RESOURCE_DIMENSION_BUFFER = 1,
	D3D10_RESOURCE_DIMENSION_TEXTURE1D = 2,
	D3D10_RESOURCE_DIMENSION_TEXTURE2D = 3,
	D3D10_RESOURCE_DIMENSION_TEXTURE3D = 4
} D3D10_RESOURCE_DIMENSION;

//-----------------------------------------------------------------------------
// DDS file format structures.
//-----------------------------------------------------------------------------
typedef struct _DDS_PIXELFORMAT
{
	uint32_t dwSize;
	uint32_t dwFlags;
	uint32_t dwFourCC;
	uint32_t dwRGBBitCount;
	uint32_t dwRBitMask;
	uint32_t dwGBitMask;
	uint32_t dwBBitMask;
	uint32_t dwABitMask;
} DDS_PIXELFORMAT;

typedef struct _DDS_HEADER
{
	uint32_t dwSize;
	uint32_t dwFlags;
	uint32_t dwHeight;
	uint32_t dwWidth;
	uint32_t dwPitchOrLinearSize;
	uint32_t dwDepth;
	uint32_t dwMipMapCount;
	uint32_t dwReserved1[11];
	DDS_PIXELFORMAT ddspf;
	uint32_t dwCaps1;
	uint32_t dwCaps2;
	uint32_t dwReserved2[3];
} DDS_HEADER;

typedef struct
{
	DXGI_FORMAT dxgiFormat;
	D3D10_RESOURCE_DIMENSION resourceDimension;
	uint32_t miscFlag;
	uint32_t arraySize;
	uint32_t miscFlags2;
} DDS_HEADER_DXT10;

//-----------------------------------------------------------------------------
// helper functions
//-----------------------------------------------------------------------------

// helper macros.
static inline void swapChar(uint8_t& a, uint8_t& b)
{
	uint8_t tmp;
	tmp = a;
	a = b;
	b = tmp;
}

static inline void swapShort(uint16_t& a, uint16_t& b)
{
	uint16_t tmp;
	tmp = a;
	a = b;
	b = tmp;
}

static void flip_blocks_dxtc1(DXTColBlock *line, int32_t numBlocks)
{
	DXTColBlock *curblock = line;
	int32_t i;

	for (i = 0; i < numBlocks; i++)
	{
		swapChar(curblock->row[0], curblock->row[3]);
		swapChar(curblock->row[1], curblock->row[2]);
		curblock++;
	}
}

static void flip_blocks_dxtc3(DXTColBlock *line, int32_t numBlocks)
{
	DXTColBlock *curblock = line;
	DXT3AlphaBlock *alphablock;
	int32_t i;

	for (i = 0; i < numBlocks; i++)
	{
		alphablock = (DXT3AlphaBlock*)curblock;

		swapShort(alphablock->row[0], alphablock->row[3]);
		swapShort(alphablock->row[1], alphablock->row[2]);
		curblock++;

		swapChar(curblock->row[0], curblock->row[3]);
		swapChar(curblock->row[1], curblock->row[2]);
		curblock++;
	}
}

static void flip_dxt5_alpha(DXT5AlphaBlock *block)
{
	int8_t gBits[4][4];

	const uint32_t mask = 0x00000007;          // bits = 00 00 01 11
	uint32_t bits = 0;
	std::memcpy(&bits, &block->row[0], sizeof(int8_t) * 3);

	gBits[0][0] = static_cast<int8_t>(bits & mask);
	bits >>= 3;
	gBits[0][1] = static_cast<int8_t>(bits & mask);
	bits >>= 3;
	gBits[0][2] = static_cast<int8_t>(bits & mask);
	bits >>= 3;
	gBits[0][3] = static_cast<int8_t>(bits & mask);
	bits >>= 3;
	gBits[1][0] = static_cast<int8_t>(bits & mask);
	bits >>= 3;
	gBits[1][1] = static_cast<int8_t>(bits & mask);
	bits >>= 3;
	gBits[1][2] = static_cast<int8_t>(bits & mask);
	bits >>= 3;
	gBits[1][3] = static_cast<int8_t>(bits & mask);

	bits = 0;
	std::memcpy(&bits, &block->row[3], sizeof(int8_t) * 3);

	gBits[2][0] = static_cast<int8_t>(bits & mask);
	bits >>= 3;
	gBits[2][1] = static_cast<int8_t>(bits & mask);
	bits >>= 3;
	gBits[2][2] = static_cast<int8_t>(bits & mask);
	bits >>= 3;
	gBits[2][3] = static_cast<int8_t>(bits & mask);
	bits >>= 3;
	gBits[3][0] = static_cast<int8_t>(bits & mask);
	bits >>= 3;
	gBits[3][1] = static_cast<int8_t>(bits & mask);
	bits >>= 3;
	gBits[3][2] = static_cast<int8_t>(bits & mask);
	bits >>= 3;
	gBits[3][3] = static_cast<int8_t>(bits & mask);

	bits = (gBits[3][0] << 0) | (gBits[3][1] << 3) | (gBits[3][2] << 6) |
		(gBits[3][3] << 9) | (gBits[2][0] << 12) | (gBits[2][1] << 15) |
		(gBits[2][2] << 18) | (gBits[2][3] << 21);

	std::memcpy(&block->row[0], &bits, 3);

	bits = (gBits[1][0] << 0) | (gBits[1][1] << 3) | (gBits[1][2] << 6) |
		(gBits[1][3] << 9) | (gBits[0][0] << 12) | (gBits[0][1] << 15) |
		(gBits[0][2] << 18) | (gBits[0][3] << 21);

	std::memcpy(&block->row[3], &bits, 3);
}

static void flip_blocks_dxtc5(DXTColBlock *line, int32_t numBlocks)
{
	DXTColBlock *curblock = line;
	DXT5AlphaBlock *alphablock;
	int32_t i;

	for (i = 0; i < numBlocks; i++)
	{
		alphablock = reinterpret_cast<DXT5AlphaBlock*>(curblock);

		flip_dxt5_alpha(alphablock);
		curblock++;

		swapChar(curblock->row[0], curblock->row[3]);
		swapChar(curblock->row[1], curblock->row[2]);
		curblock++;
	}
}

static void flip_data_vertical(int8_t *image, int32_t width, int32_t height, DDSImageInfo* info, AllocatorBase& allocator)
{
	if (info->compressed)
	{
		int32_t linesize, j;
		DXTColBlock *top;
		DXTColBlock *bottom;
		int8_t *tmp;
		void(*flipblocks)(DXTColBlock*, int32_t) = NULL;
		int32_t xblocks = width / 4;
		int32_t yblocks = height / 4;
		int32_t blocksize;

		switch (info->format)
		{
		case HalImageFormat::BC1RgbUnorm:
			blocksize = 8;
			flipblocks = &flip_blocks_dxtc1;
			break;
		case HalImageFormat::BC2Unorm:
			blocksize = 16;
			flipblocks = &flip_blocks_dxtc3;
			break;
		case HalImageFormat::BC3Unorm:
			blocksize = 16;
			flipblocks = &flip_blocks_dxtc5;
			break;
		default:
			return;
		}

		linesize = xblocks * blocksize;
		tmp = AllocateArray<int8_t>(allocator, linesize);
		if (!tmp)
			return;

		for (j = 0; j < (yblocks >> 1); j++)
		{
			top = (DXTColBlock*)(void*)(image + j * linesize);
			bottom = (DXTColBlock*)(void*)(image + (((yblocks - j) - 1) * linesize));

			(*flipblocks)(top, xblocks);
			(*flipblocks)(bottom, xblocks);

			std::memcpy(tmp, bottom, linesize);
			std::memcpy(bottom, top, linesize);
			std::memcpy(top, tmp, linesize);
		}

		// Catch the middle row of blocks if there is one
		// The loop above will skip the middle row
		if (yblocks & 0x01)
		{
			DXTColBlock *middle
				= (DXTColBlock*)(void*)(image + (yblocks >> 1) * linesize);
			(*flipblocks)(middle, xblocks);
		}

		DeallocateArray<int8_t>(allocator, tmp);
	}
	else
	{
		int32_t linesize = width * info->bytesPerPixel;
		int32_t j;
		int8_t *top;
		int8_t *bottom;
		int8_t *tmp;

		// much simpler - just compute the line length and swap each row
		tmp = AllocateArray<int8_t>(allocator, linesize);
		if (!tmp)
			return;

		for (j = 0; j < (height >> 1); j++)
		{
			top = static_cast<int8_t*>(image + j * linesize);
			bottom = static_cast<int8_t*>(image + (((height - j) - 1) * linesize));

			std::memcpy(tmp, bottom, linesize);
			std::memcpy(bottom, top, linesize);
			std::memcpy(top, tmp, linesize);
		}

		DeallocateArray<int8_t>(allocator, tmp);
	}
}

static int32_t size_image(int32_t width, int32_t height, const DDSImageInfo* image)
{
	if (image->compressed)
	{
		return ((width + 3) / 4)*((height + 3) / 4)
			* (image->format == HalImageFormat::BC1RgbaUnorm ? 8 : 16);
	}
	else
	{
		return width * height * image->bytesPerPixel;
	}
}

static int32_t total_image_data_size(DDSImageInfo* image)
{
	int32_t index = 0, size = 0, w, h;
	int32_t cubeCount = image->cubemap ? DDS_NUM_CUBEMAP_FACES : 1;

	for (int32_t j = 0; j < cubeCount; j++)
	{
		w = image->width;
		h = image->height;

		for (uint32_t i = 0; i < image->numMipmaps; i++) // account for base plus each mip
		{
			image->size[index] = size_image(w, h, image);
			image->mipwidth[index] = w;
			image->mipheight[index] = h;
			size += image->size[index];
			if (w != 1)
			{
				w >>= 1;
			}
			if (h != 1)
			{
				h >>= 1;
			}

			index++;
		}
	}

	return(size);
}

void* DDSAllocDataBlock(DDSImageInfo* image, AllocatorBase& allocator)
{
	if (image)
	{
		int32_t size = total_image_data_size(image);
		image->dataBlock = AllocateArray<int8_t>(allocator, size);
		if (image->dataBlock == nullptr)
		{
			return nullptr;
		}

		image->data[0] = image->dataBlock;

		uint32_t planes = image->numMipmaps * (image->cubemap ? DDS_NUM_CUBEMAP_FACES : 1);

		for (uint32_t i = 1; i < planes; i++) // account for base plus each mip
		{
			image->data[i] = (int8_t*)(((size_t)(image->data[i - 1])) + (size_t)image->size[i - 1]);
		}

		return(image->dataBlock); // in case caller wants to sanity check...
	}

	return(nullptr);
}


void getImageFormatInfo(DDSImageInfo& imageInfo, DXGI_FORMAT format)
{
	imageInfo.format = HalImageFormat::Undefined;

	switch (format)
	{
	case cave::DXGI_FORMAT_UNKNOWN:
		imageInfo.format = HalImageFormat::Undefined;
		break;
	case cave::DXGI_FORMAT_R32G32B32A32_TYPELESS:
		break;
	case cave::DXGI_FORMAT_R32G32B32A32_FLOAT:
		break;
	case cave::DXGI_FORMAT_R32G32B32A32_UINT:
		break;
	case cave::DXGI_FORMAT_R32G32B32A32_SINT:
		break;
	case cave::DXGI_FORMAT_R32G32B32_TYPELESS:
		break;
	case cave::DXGI_FORMAT_R32G32B32_FLOAT:
		break;
	case cave::DXGI_FORMAT_R32G32B32_UINT:
		break;
	case cave::DXGI_FORMAT_R32G32B32_SINT:
		break;
	case cave::DXGI_FORMAT_R16G16B16A16_TYPELESS:
		break;
	case cave::DXGI_FORMAT_R16G16B16A16_FLOAT:
		break;
	case cave::DXGI_FORMAT_R16G16B16A16_UNORM:
		break;
	case cave::DXGI_FORMAT_R16G16B16A16_UINT:
		break;
	case cave::DXGI_FORMAT_R16G16B16A16_SNORM:
		break;
	case cave::DXGI_FORMAT_R16G16B16A16_SINT:
		break;
	case cave::DXGI_FORMAT_R32G32_TYPELESS:
		break;
	case cave::DXGI_FORMAT_R32G32_FLOAT:
		break;
	case cave::DXGI_FORMAT_R32G32_UINT:
		break;
	case cave::DXGI_FORMAT_R32G32_SINT:
		break;
	case cave::DXGI_FORMAT_R32G8X24_TYPELESS:
		break;
	case cave::DXGI_FORMAT_D32_FLOAT_S8X24_UINT:
		break;
	case cave::DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS:
		break;
	case cave::DXGI_FORMAT_X32_TYPELESS_G8X24_UINT:
		break;
	case cave::DXGI_FORMAT_R10G10B10A2_TYPELESS:
		break;
	case cave::DXGI_FORMAT_R10G10B10A2_UNORM:
		break;
	case cave::DXGI_FORMAT_R10G10B10A2_UINT:
		break;
	case cave::DXGI_FORMAT_R11G11B10_FLOAT:
		break;
	case cave::DXGI_FORMAT_R8G8B8A8_TYPELESS:
		break;
	case cave::DXGI_FORMAT_R8G8B8A8_UNORM:
		imageInfo.format = HalImageFormat::R8G8B8A8UNorm;
		imageInfo.components = 4;
		imageInfo.componentFormat = HalComponentType::UInt8;
		imageInfo.bytesPerPixel = 4;
		imageInfo.alpha = 1;
		imageInfo.compressed = 0;
		break;
	case cave::DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
		break;
	case cave::DXGI_FORMAT_R8G8B8A8_UINT:
		break;
	case cave::DXGI_FORMAT_R8G8B8A8_SNORM:
		break;
	case cave::DXGI_FORMAT_R8G8B8A8_SINT:
		break;
	case cave::DXGI_FORMAT_R16G16_TYPELESS:
		break;
	case cave::DXGI_FORMAT_R16G16_FLOAT:
		break;
	case cave::DXGI_FORMAT_R16G16_UNORM:
		break;
	case cave::DXGI_FORMAT_R16G16_UINT:
		break;
	case cave::DXGI_FORMAT_R16G16_SNORM:
		break;
	case cave::DXGI_FORMAT_R16G16_SINT:
		break;
	case cave::DXGI_FORMAT_R32_TYPELESS:
		break;
	case cave::DXGI_FORMAT_D32_FLOAT:
		break;
	case cave::DXGI_FORMAT_R32_FLOAT:
		break;
	case cave::DXGI_FORMAT_R32_UINT:
		break;
	case cave::DXGI_FORMAT_R32_SINT:
		break;
	case cave::DXGI_FORMAT_R24G8_TYPELESS:
		break;
	case cave::DXGI_FORMAT_D24_UNORM_S8_UINT:
		break;
	case cave::DXGI_FORMAT_R24_UNORM_X8_TYPELESS:
		break;
	case cave::DXGI_FORMAT_X24_TYPELESS_G8_UINT:
		break;
	case cave::DXGI_FORMAT_R8G8_TYPELESS:
		break;
	case cave::DXGI_FORMAT_R8G8_UNORM:
		break;
	case cave::DXGI_FORMAT_R8G8_UINT:
		break;
	case cave::DXGI_FORMAT_R8G8_SNORM:
		break;
	case cave::DXGI_FORMAT_R8G8_SINT:
		break;
	case cave::DXGI_FORMAT_R16_TYPELESS:
		break;
	case cave::DXGI_FORMAT_R16_FLOAT:
		break;
	case cave::DXGI_FORMAT_D16_UNORM:
		break;
	case cave::DXGI_FORMAT_R16_UNORM:
		break;
	case cave::DXGI_FORMAT_R16_UINT:
		break;
	case cave::DXGI_FORMAT_R16_SNORM:
		break;
	case cave::DXGI_FORMAT_R16_SINT:
		break;
	case cave::DXGI_FORMAT_R8_TYPELESS:
		break;
	case cave::DXGI_FORMAT_R8_UNORM:
		break;
	case cave::DXGI_FORMAT_R8_UINT:
		break;
	case cave::DXGI_FORMAT_R8_SNORM:
		break;
	case cave::DXGI_FORMAT_R8_SINT:
		break;
	case cave::DXGI_FORMAT_A8_UNORM:
		break;
	case cave::DXGI_FORMAT_R1_UNORM:
		break;
	case cave::DXGI_FORMAT_R9G9B9E5_SHAREDEXP:
		break;
	case cave::DXGI_FORMAT_R8G8_B8G8_UNORM:
		break;
	case cave::DXGI_FORMAT_G8R8_G8B8_UNORM:
		break;
	case cave::DXGI_FORMAT_BC1_TYPELESS:
		break;
	case cave::DXGI_FORMAT_BC1_UNORM:
		imageInfo.format = HalImageFormat::BC1RgbaUnorm;
		imageInfo.components = 4;
		imageInfo.bytesPerPixel = 16;
		imageInfo.compressed = 1;
		imageInfo.alpha = 1;
		break;
	case cave::DXGI_FORMAT_BC1_UNORM_SRGB:
		break;
	case cave::DXGI_FORMAT_BC2_TYPELESS:
		break;
	case cave::DXGI_FORMAT_BC2_UNORM:
		imageInfo.format = HalImageFormat::BC2Unorm;
		imageInfo.components = 4;
		imageInfo.bytesPerPixel = 16;
		imageInfo.compressed = 1;
		imageInfo.alpha = 1;
		break;
	case cave::DXGI_FORMAT_BC2_UNORM_SRGB:
		imageInfo.format = HalImageFormat::BC2Unorm;
		imageInfo.components = 4;
		imageInfo.bytesPerPixel = 16;
		imageInfo.compressed = 1;
		imageInfo.alpha = 1;
		break;
	case cave::DXGI_FORMAT_BC3_TYPELESS:
		break;
	case cave::DXGI_FORMAT_BC3_UNORM:
		imageInfo.format = HalImageFormat::BC3Unorm;
		imageInfo.components = 4;
		imageInfo.bytesPerPixel = 16;
		imageInfo.compressed = 1;
		imageInfo.alpha = 1;
		break;
	case cave::DXGI_FORMAT_BC3_UNORM_SRGB:
		imageInfo.format = HalImageFormat::BC3Unorm;
		imageInfo.components = 4;
		imageInfo.bytesPerPixel = 16;
		imageInfo.compressed = 1;
		imageInfo.alpha = 1;
		break;
	case cave::DXGI_FORMAT_BC4_TYPELESS:
		break;
	case cave::DXGI_FORMAT_BC4_UNORM:
		break;
	case cave::DXGI_FORMAT_BC4_SNORM:
		break;
	case cave::DXGI_FORMAT_BC5_TYPELESS:
		break;
	case cave::DXGI_FORMAT_BC5_UNORM:
		break;
	case cave::DXGI_FORMAT_BC5_SNORM:
		break;
	case cave::DXGI_FORMAT_B5G6R5_UNORM:
		break;
	case cave::DXGI_FORMAT_B5G5R5A1_UNORM:
		break;
	case cave::DXGI_FORMAT_B8G8R8A8_UNORM:
		break;
	case cave::DXGI_FORMAT_B8G8R8X8_UNORM:
		break;
	case cave::DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM:
		break;
	case cave::DXGI_FORMAT_B8G8R8A8_TYPELESS:
		break;
	case cave::DXGI_FORMAT_B8G8R8A8_UNORM_SRGB:
		break;
	case cave::DXGI_FORMAT_B8G8R8X8_TYPELESS:
		break;
	case cave::DXGI_FORMAT_B8G8R8X8_UNORM_SRGB:
		break;
	case cave::DXGI_FORMAT_BC6H_TYPELESS:
		break;
	case cave::DXGI_FORMAT_BC6H_UF16:
		break;
	case cave::DXGI_FORMAT_BC6H_SF16:
		break;
	case cave::DXGI_FORMAT_BC7_TYPELESS:
		break;
	case cave::DXGI_FORMAT_BC7_UNORM:
		imageInfo.format = HalImageFormat::BC7Unorm;
		imageInfo.components = 4;
		imageInfo.bytesPerPixel = 16;
		imageInfo.compressed = 1;
		imageInfo.alpha = 1;
		break;
	case cave::DXGI_FORMAT_BC7_UNORM_SRGB:
		imageInfo.format = HalImageFormat::BC7Srgb;
		imageInfo.components = 4;
		imageInfo.bytesPerPixel = 16;
		imageInfo.compressed = 1;
		imageInfo.alpha = 1;
		break;
	case cave::DXGI_FORMAT_AYUV:
		break;
	case cave::DXGI_FORMAT_Y410:
		break;
	case cave::DXGI_FORMAT_Y416:
		break;
	case cave::DXGI_FORMAT_NV12:
		break;
	case cave::DXGI_FORMAT_P010:
		break;
	case cave::DXGI_FORMAT_P016:
		break;
	case cave::DXGI_FORMAT_420_OPAQUE:
		break;
	case cave::DXGI_FORMAT_YUY2:
		break;
	case cave::DXGI_FORMAT_Y210:
		break;
	case cave::DXGI_FORMAT_Y216:
		break;
	case cave::DXGI_FORMAT_NV11:
		break;
	case cave::DXGI_FORMAT_AI44:
		break;
	case cave::DXGI_FORMAT_IA44:
		break;
	case cave::DXGI_FORMAT_P8:
		break;
	case cave::DXGI_FORMAT_A8P8:
		break;
	case cave::DXGI_FORMAT_B4G4R4A4_UNORM:
		break;
	case cave::DXGI_FORMAT_P208:
		break;
	case cave::DXGI_FORMAT_V208:
		break;
	case cave::DXGI_FORMAT_V408:
		break;
	case cave::DXGI_FORMAT_FORCE_UINT:
		break;
	default:
		break;
	}

	assert(imageInfo.format != HalImageFormat::Undefined);
}

//-----------------------------------------------------------------------------
// class functions
//-----------------------------------------------------------------------------

ImageResourceDds::ImageResourceDds(ResourceManagerPrivate* rm)
	: ImageResource(rm)
{}

ImageResourceDds::~ImageResourceDds()
{
	releaseImageData();
}

bool ImageResourceDds::decode(bool flipVertical, std::ifstream& dataStream)
{
	DDS_HEADER ddsh;
	DDS_HEADER_DXT10 ddsdx10;   // extended header for DX 10 formats
	char filecode[4];
	bool needsBGRASwap = false;
	bool isAllreadyFlipped = false;

	// check file code
	dataStream.read(filecode, 4);
	if (std::memcmp(filecode, "DDS ", 4))
	{
		return false;
	}

	std::memset(&m_imageInfo, 0, sizeof(DDSImageInfo));

	// read in DDS header
	dataStream.read(reinterpret_cast<char*>(&ddsh), sizeof(DDS_HEADER));
	// check if image is a cubempap
	if (ddsh.dwCaps2 & DDS_CUBEMAP)
	{
		const int32_t allFaces =
			DDS_CUBEMAP_POSITIVEX |
			DDS_CUBEMAP_POSITIVEY |
			DDS_CUBEMAP_POSITIVEZ |
			DDS_CUBEMAP_NEGATIVEX |
			DDS_CUBEMAP_NEGATIVEY |
			DDS_CUBEMAP_NEGATIVEZ;

		if ((ddsh.dwCaps2 & allFaces) != allFaces)
		{
			return false;
		}

		m_imageInfo.cubemap = true;
	}
	else
	{
		m_imageInfo.cubemap = false;
	}

	// check if image is a volume texture
	if ((ddsh.dwCaps2 & DDS_VOLUME) && (ddsh.dwDepth > 0))
	{
		// not supported
		return false;
	}

	bool isDX10 = false;

	// figure out what the image format is
	if (ddsh.ddspf.dwFlags & DDS_FOURCC)
	{
		switch (ddsh.ddspf.dwFourCC)
		{
		case FOURCC_DXT1:
			m_imageInfo.format = HalImageFormat::BC1RgbaUnorm;
			m_imageInfo.components = 4;
			m_imageInfo.bytesPerPixel = 16;
			m_imageInfo.compressed = 1;
			m_imageInfo.alpha = 1;
			break;
		case FOURCC_DXT3:
			m_imageInfo.format = HalImageFormat::BC2Unorm;
			m_imageInfo.components = 4;
			m_imageInfo.bytesPerPixel = 16;
			m_imageInfo.compressed = 1;
			m_imageInfo.alpha = 1;
			break;
		case FOURCC_DXT5:
			m_imageInfo.format = HalImageFormat::BC3Unorm;
			m_imageInfo.components = 4;
			m_imageInfo.bytesPerPixel = 16;
			m_imageInfo.compressed = 1;
			m_imageInfo.alpha = 1;
			break;
		case FOURCC_DX10:
			isDX10 = true; // We need to read the extended header to get format data
			break;
		case FOURCC_DXGI_FORMAT_R16G16B16A16_FLOAT:
			m_imageInfo.format = HalImageFormat::R16G16B16A16SFloat;
			m_imageInfo.components = 4;
			m_imageInfo.componentFormat = HalComponentType::Float16;
			m_imageInfo.bytesPerPixel = 8;
			m_imageInfo.alpha = 1;
			m_imageInfo.compressed = 0;
			break;
		case FOURCC_DXGI_FORMAT_R32G32B32A32_FLOAT:
			m_imageInfo.format = HalImageFormat::R32G32B32A32SFloat;
			m_imageInfo.components = 4;
			m_imageInfo.componentFormat = HalComponentType::Float32;
			m_imageInfo.bytesPerPixel = 16;
			m_imageInfo.alpha = 1;
			m_imageInfo.compressed = 0;
			break;
		default:
			assert(false);
		}
	}
	else
	{
		// Check for a supported pixel format
		if ((ddsh.ddspf.dwRGBBitCount == 32) &&
			(ddsh.ddspf.dwRBitMask == 0x000000FF) &&
			(ddsh.ddspf.dwGBitMask == 0x0000FF00) &&
			(ddsh.ddspf.dwBBitMask == 0x00FF0000) &&
			(ddsh.ddspf.dwABitMask == 0xFF000000))
		{
			// We support D3D's A8B8G8R8, which is actually RGBA in linear
			// memory, equivalent to GL's RGBA
			m_imageInfo.format = HalImageFormat::R8G8B8A8UNorm;
			m_imageInfo.components = 4;
			m_imageInfo.componentFormat = HalComponentType::UInt8;
			m_imageInfo.bytesPerPixel = 4;
			m_imageInfo.alpha = 1;
			m_imageInfo.compressed = 0;
		}
		else if ((ddsh.ddspf.dwRGBBitCount == 32) &&
			(ddsh.ddspf.dwRBitMask == 0x00FF0000) &&
			(ddsh.ddspf.dwGBitMask == 0x0000FF00) &&
			(ddsh.ddspf.dwBBitMask == 0x000000FF) &&
			(ddsh.ddspf.dwABitMask == 0xFF000000))
		{
			// We support D3D's A8R8G8B8, which is actually BGRA in linear
			// memory, need to be 
			m_imageInfo.format = HalImageFormat::B8G8R8A8UNorm;
			m_imageInfo.components = 4;
			m_imageInfo.componentFormat = HalComponentType::UInt8;
			m_imageInfo.bytesPerPixel = 4;
			m_imageInfo.alpha = 1;
			m_imageInfo.compressed = 0;
			needsBGRASwap = true;
		}
		else
		{
			assert(false);
			return false;
		}
	}

	if (isDX10)
	{
		// read in DDS DX10 extended header
		dataStream.read(reinterpret_cast<char*>(&ddsdx10), sizeof(DDS_HEADER_DXT10));
		getImageFormatInfo(m_imageInfo, ddsdx10.dxgiFormat);
	}

	// detect flagging to indicate this texture was stored in a y-inverted fashion
	if (!(ddsh.dwFlags & DDS_LINEARSIZE))
	{
		if (ddsh.dwPitchOrLinearSize == DDS_MAGIC_FLIPPED)
		{
			isAllreadyFlipped = true;
		}
	}

	flipVertical = (isAllreadyFlipped != (flipVertical ? true : false)) ? 1 : 0;

	// store primary surface width/height/numMipmaps
	m_imageInfo.width = ddsh.dwWidth;
	m_imageInfo.height = ddsh.dwHeight;
	m_imageInfo.numMipmaps = ddsh.dwFlags & DDS_MIPMAPCOUNT ? ddsh.dwMipMapCount : 1;

	if (m_imageInfo.numMipmaps > DDS_MAX_MIPMAPS)
	{
		return false;
	}

	// allocate the meta datablock for all mip storage.
	DDSAllocDataBlock(&m_imageInfo, *_pResourceManagerPrivate->GetEngineAllocator());
	if (m_imageInfo.dataBlock == nullptr)
	{
		return false;
	}

	uint32_t faces = m_imageInfo.cubemap ? 6 : 1;

	int32_t index = 0;
	for (uint32_t j = 0; j < faces; j++)
	{
		// load all surfaces for the image
		uint32_t width = m_imageInfo.width;
		uint32_t height = m_imageInfo.height;

		for (uint32_t i = 0; i < m_imageInfo.numMipmaps; i++)
		{
			// Get the size, read in the data.
			dataStream.read(reinterpret_cast<char*>(m_imageInfo.data[index]), m_imageInfo.size[index]);

			// Flip in Y for OpenGL if needed
			if (flipVertical)
				flip_data_vertical(static_cast<int8_t*>(m_imageInfo.data[index]), width, height, &m_imageInfo, *_pResourceManagerPrivate->GetEngineAllocator());

			// shrink to next power of 2
			width >>= 1;
			height >>= 1;

			if (!width)
				width = 1;

			if (!height)
				height = 1;

			// make sure DXT isn't <4 on a side...
			if (m_imageInfo.compressed)
			{
				if (width < 4)
					width = 4;
				if (height < 4)
					height = 4;
			}

			index++;
		}
	}

	if (needsBGRASwap)
	{
		index = 0;

		for (uint32_t k = 0; k < faces; k++)
		{
			uint32_t width = m_imageInfo.width;
			uint32_t height = m_imageInfo.height;

			for (uint32_t i = 0; i < m_imageInfo.numMipmaps; i++)
			{
				int8_t* data = static_cast<int8_t*>(m_imageInfo.data[index]);
				uint32_t pixels = width * height;

				for (uint32_t j = 0; j < pixels; j++)
				{
					int8_t temp = data[0];
					data[0] = data[2];
					data[2] = temp;

					data += 4;
				}

				// shrink to next power of 2
				width >>= 1;
				height >>= 1;

				if (!width)
					width = 1;

				if (!height)
					height = 1;

				index++;
			}
		}
	}

	return true;
}

ImageData ImageResourceDds::getImageData()
{
	ImageData imageData = {};
	imageData.width = m_imageInfo.width;
	imageData.height = m_imageInfo.height;
	imageData.componentCount = m_imageInfo.components;
	imageData.componentSize = m_imageInfo.bytesPerPixel;
	imageData.format = m_imageInfo.format;
	imageData.arraySize = (m_imageInfo.cubemap) ? 6 : 1;
	imageData.levels = m_imageInfo.numMipmaps;
	imageData.data = reinterpret_cast<uint8_t*>(m_imageInfo.dataBlock);

	return imageData;
}

ImageLevelData ImageResourceDds::getLevelData(uint32_t face, uint32_t mipLevel)
{
	if ((face > 0 && !m_imageInfo.cubemap) || mipLevel > m_imageInfo.numMipmaps)
		return ImageLevelData();

	ImageLevelData imageData = {};

	if (face > 0)
		imageData.width = m_imageInfo.mipwidth[mipLevel];
	imageData.height = m_imageInfo.mipheight[mipLevel];
	imageData.componentCount = m_imageInfo.components;
	imageData.componentSize = m_imageInfo.bytesPerPixel;
	imageData.format = m_imageInfo.format;
	imageData.data = reinterpret_cast<uint8_t*>(m_imageInfo.data[mipLevel]);
	return imageData;
}

void ImageResourceDds::releaseImageData()
{
	if (m_imageInfo.dataBlock)
	{
		DeallocateArray<int8_t>(*_pResourceManagerPrivate->GetEngineAllocator(), m_imageInfo.dataBlock);
		m_imageInfo.dataBlock = nullptr;
	}
}



}
