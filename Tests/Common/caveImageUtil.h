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

/// @file caveImageUtil.h
///       Base heade for image utility functions

extern "C"
{
#undef FAR // to avaid warning
#define XMD_H 1 // To avoid INT32 redefiniton
#include "png.h"
#include "pngconf.h"
#include <setjmp.h>
}

#ifdef __cplusplus
extern "C"
{
	typedef struct SCREEN_PIXEL
	{
		int red;
		int green;
		int blue;
	}pixel;

	// png related structures
	typedef struct {
		unsigned char *pixels;
		size_t width;
		size_t height;
		size_t pitch;
		size_t elementSize;
		bool writeAlpha;
	} image_t;


	bool WritePng(const char* filename, image_t *pImage);
	bool ReadPng(const char* filename, image_t *pImage);
	bool CmpImage(image_t *pRefImage, image_t *pSrcImage, image_t *pDiffImage);
}
#endif


