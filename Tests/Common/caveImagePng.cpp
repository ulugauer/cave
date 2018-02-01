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

/// @file caveImagePng.cpp
///       Handles png images

#ifdef _WIN32
#include <windows.h>		// not exactly sure why
#endif

#include <stdio.h>

#include "caveImageUtil.h"


#ifdef _WIN32
#pragma warning ( push )
#pragma warning (disable: 4121)
#pragma warning (disable:4820)
#pragma warning (disable:4100)
#pragma warning (disable:4611)  // interaction between '_setjmp' and C++ object destruction is non-portable
#pragma warning (disable:4365)	// signed/unsigned mismatch
#endif

static void
pixel_at_inv(image_t * image, int x, int y,
	unsigned char *red, unsigned char *green,
	unsigned char *blue, unsigned char *alpha)
{
	unsigned int startY = (unsigned int)image->height - y - 1;
	unsigned char *pixel = image->pixels + ((unsigned int)image->pitch * startY) + x * (unsigned int)image->elementSize;

	*red = pixel[0];
	*green = pixel[1];
	*blue = pixel[2];
	if (image->writeAlpha)
		*alpha = pixel[3];
}

/**
* @brief write a png image to file
*
* @param[in] filename	Name of output file
* @param[in] pImage		Pointer to image struct
*
* @return true on success
*/
extern "C" bool
WritePng(const char* filename, image_t *pImage)
{
	FILE * fp;
	png_structp png_ptr = NULL;
	png_infop info_ptr = NULL;
	size_t x, y;
	png_byte ** row_pointers = NULL;
	bool status = false;

	int pixel_size = (pImage->writeAlpha) ? 4 : 3;
	int depth = 8;

	fp = fopen(filename, "wb");
	if (!fp) {
		printf("failed to open file: %s\n", filename);
		return false;
	}


	png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (png_ptr == NULL) {
		goto png_create_write_struct_failed;
	}

	info_ptr = png_create_info_struct(png_ptr);
	if (info_ptr == NULL) {
		goto png_create_info_struct_failed;
	}

	// error handling
	if (setjmp(png_jmpbuf(png_ptr))) {
		goto png_failure;
	}

	/* Set image attributes. */

	png_set_IHDR(png_ptr,
		info_ptr,
		(unsigned int)pImage->width,
		(unsigned int)pImage->height,
		depth,
		(pImage->writeAlpha) ? PNG_COLOR_TYPE_RGB_ALPHA : PNG_COLOR_TYPE_RGB,
		PNG_INTERLACE_NONE,
		PNG_COMPRESSION_TYPE_DEFAULT,
		PNG_FILTER_TYPE_DEFAULT);

	// alloc row pointer
	row_pointers = (png_byte **)png_malloc(png_ptr, (unsigned int)pImage->height * sizeof(png_byte *));
	for (y = 0; y < pImage->height; ++y) {
		png_byte *row = (png_byte *)png_malloc(png_ptr, sizeof(unsigned char) * (unsigned int)pImage->width * (unsigned int)pixel_size);
		row_pointers[y] = row;

		for (x = 0; x < pImage->width; ++x) {
			unsigned char red, green, blue, alpha;
			pixel_at_inv(pImage, (unsigned int)x, (unsigned int)y, &red, &green, &blue, &alpha);
			*row++ = red;
			*row++ = green;
			*row++ = blue;
			if (pImage->writeAlpha)
				*row++ = alpha;
		}
	}

	// Write the image data file
	png_init_io(png_ptr, fp);
	png_set_rows(png_ptr, info_ptr, row_pointers);
	png_write_png(png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY, NULL);

	// everyting went fine
	status = true;

	// delete all
	for (y = 0; y < pImage->height; y++) {
		png_free(png_ptr, row_pointers[y]);
	}
	png_free(png_ptr, row_pointers);

png_failure:
png_create_info_struct_failed:
	png_destroy_write_struct(&png_ptr, &info_ptr);
png_create_write_struct_failed:
	fclose(fp);

	return status;
}

#define PNGSIGSIZE 8

static bool validate(FILE *filePtr)
{
	//Allocate a buffer of 8 bytes, where we can put the file signature.
	png_byte pngsig[PNGSIGSIZE];
	int is_png = 0;

	// read the png file signature
	size_t dataRead = fread(pngsig, PNGSIGSIZE, 1, filePtr);

	//Let LibPNG check the sig. If this function returns 0, everything is OK.
	if (dataRead)
		is_png = png_sig_cmp(pngsig, 0, PNGSIGSIZE);

	return (is_png == 0);
}

void readDataCallback(png_structp pngPtr, png_bytep data, png_size_t length)
{
	//Here we get our IO pointer back from the read struct.
	//This is the parameter we passed to the png_set_read_fn() function.
	//Our file pointer.
	FILE *filePtr = (FILE *)png_get_io_ptr(pngPtr);
	//read 'length' bytes into 'data'
	size_t dataRead = fread(data, 1, length, filePtr);
	if (dataRead == 0 && !feof(filePtr))
		printf("read beyond file end\n");
}

/**
* @brief read a png image to file
*
* @param[in] filename	Name of output file
* @param[in] pImage		Pointer to image struct
*
* @return true on success
*/
extern "C" bool ReadPng(const char* filename, image_t *pImage)
{
	FILE *filePtr;      // the file pointer

						// open filename in "read binary" mode
	filePtr = fopen(filename, "rb");
	if (!filePtr)
		return false;

	// check if this is really png file
	if (!validate(filePtr))
		return false;

	//Here we create the png read struct. The 3 NULL's at the end can be used
	//for your own custom error handling functions, but we'll just use the default.
	//if the function fails, NULL is returned. Always check the return values!
	png_structp pngPtr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!pngPtr)
	{
		fclose(filePtr);
		return false;
	}

	//Here we create the png info struct.
	//Note that this time, if this function fails, we have to clean up the read struct!
	png_infop infoPtr = png_create_info_struct(pngPtr);
	if (!infoPtr)
	{
		png_destroy_read_struct(&pngPtr, (png_infopp)0, (png_infopp)0);
		fclose(filePtr);
		return false;
	}

	//Here I've defined 2 pointers up front, so I can use them in error handling.
	png_bytep* rowPtrs = NULL;
	char* data = NULL;

	if (setjmp(png_jmpbuf(pngPtr))) {
		//An error occured, so clean up what we have allocated so far...
		png_destroy_read_struct(&pngPtr, &infoPtr, (png_infopp)0);
		if (rowPtrs != NULL) delete rowPtrs;
		if (data != NULL) delete data;

		//Make sure you return here. libPNG will jump to here if something
		//goes wrong, and if you continue with your normal code, you might
		//End up with an infinite loop.
		fclose(filePtr);
		return false; // Do your own error handling here.
	}

	// set read callback
	png_set_read_fn(pngPtr, (png_voidp)filePtr, readDataCallback);

	// read header and skip signature
	png_set_sig_bytes(pngPtr, PNGSIGSIZE);
	png_read_info(pngPtr, infoPtr);
	png_uint_32 imgWidth = png_get_image_width(pngPtr, infoPtr);
	png_uint_32 imgHeight = png_get_image_height(pngPtr, infoPtr);
	//bits per CHANNEL! note: not per pixel!
	png_uint_32 bitdepth = png_get_bit_depth(pngPtr, infoPtr);
	//Number of channels
	png_uint_32 channels = png_get_channels(pngPtr, infoPtr);
	//Color type. (RGB, RGBA, Luminance, luminance alpha... palette... etc)
	png_uint_32 color_type = png_get_color_type(pngPtr, infoPtr);

	switch (color_type) {
	case PNG_COLOR_TYPE_PALETTE:
		png_set_palette_to_rgb(pngPtr);
		//Don't forget to update the channel info.
		//It's used later to know how big a buffer we need for the image
		channels = 3;
		break;
	case PNG_COLOR_TYPE_GRAY:
		if (bitdepth < 8)
			png_set_expand_gray_1_2_4_to_8(pngPtr);
		//And the bitdepth info
		bitdepth = 8;
		break;
	}

	//if the image has a transperancy set.. convert it to a full Alpha channel..
	if (png_get_valid(pngPtr, infoPtr, PNG_INFO_tRNS))
	{
		png_set_tRNS_to_alpha(pngPtr);
		channels += 1;
	}

	//We don't support 16 bit precision.. so if the image Has 16 bits per channel
	//precision... round it down to 8.
	if (bitdepth == 16)
		png_set_strip_16(pngPtr);

	// setup header for queries
	pImage->width = imgWidth;
	pImage->height = imgHeight;
	pImage->elementSize = channels;
	pImage->pitch = imgWidth * (bitdepth * channels / 8);

	// read image
	//Here's one of the pointers we've defined in the error handler section:
	//Array of row pointers. One for every row.
	rowPtrs = new png_bytep[imgHeight];

	if (!rowPtrs)
	{
		png_destroy_read_struct(&pngPtr, (png_infopp)0, (png_infopp)0);
		fclose(filePtr);
		return false;
	}
	//Alocate a buffer with enough space.
	//(Don't use the stack, these blocks get big easilly)
	//This pointer was also defined in the error handling section, so we can clean it up on error.
	pImage->pixels = new unsigned char[imgWidth * imgHeight * (bitdepth * channels / 8)];

	if (!pImage->pixels)
	{
		png_destroy_read_struct(&pngPtr, (png_infopp)0, (png_infopp)0);
		fclose(filePtr);
		return false;
	}

	//This is the length in bytes, of one row.
	const unsigned int stride = imgWidth * (bitdepth * channels / 8);

	//A little for-loop here to set all the row pointers to the starting
	//Adresses for every row in the buffer
	for (size_t i = 0; i < imgHeight; i++) {
		//Set the pointer to the data pointer + i times the row stride.
		//Notice that the row order is reversed with q.
		//This is how at least OpenGL expects it,
		//and how many other image loaders present the data.
		png_uint_32 q = (imgHeight - (unsigned int)i - 1) * (unsigned int)stride;
		rowPtrs[i] = (png_bytep)pImage->pixels + q;
	}

	//And here it is! The actuall reading of the image!
	//Read the imagedata and write it to the adresses pointed to
	//by rowptrs (in other words: our image databuffer)
	png_read_image(pngPtr, rowPtrs);

	// clean up
	delete (png_bytep)rowPtrs;
	//And don't forget to clean up the read and info structs !
	png_destroy_read_struct(&pngPtr, &infoPtr, (png_infopp)0);

	fclose(filePtr);

	return true;
}

/**
* @brief compare two  png images
*
* @param[in] pRefImage		Pointer to reference image
* @param[in] pSrcImage		Pointer to source image
* @param[out] pDiffImage	Pointer to difference image
*
* @return true on success
*/
extern "C" bool CmpImage(image_t *pRefImage, image_t *pSrcImage, image_t *pDiffImage)
{
	// first sanity checkes
	if (!pRefImage || !pRefImage->pixels ||
		!pSrcImage || !pSrcImage->pixels)
	{
		return false;
	}

	// check dimensions
	if (pRefImage->width != pSrcImage->width ||
		pRefImage->height != pSrcImage->height)
	{
		return false;
	}

	// we always assume component count of 4
	if (pRefImage->elementSize != 4 || pSrcImage->elementSize != 4)
	{
		return false;
	}

	if (pDiffImage)
		pDiffImage->pixels = new unsigned char[pRefImage->height * pRefImage->pitch];

	unsigned int *pRef = (unsigned int *)pRefImage->pixels;
	unsigned int *pSrc = (unsigned int *)pSrcImage->pixels;
	unsigned int *pDiff = (pDiffImage && pDiffImage->pixels) ? (unsigned int *)pSrcImage->pixels : NULL;

	size_t failed = 0;
	// we allow a maximum of one percent pixel errors
	size_t maxError = (pRefImage->height * pRefImage->width) / 100;

	for (size_t h = 0; h < pRefImage->height; ++h)
	{
		for (size_t w = 0; w < pRefImage->width; ++w)
		{
			if (*pRef != *pSrc)
			{
				failed++;
			}

			// we write current image
			if (pDiff)
				*pDiff = *pSrc;

			pRef++;
			pSrc++;
		}
	}

	return failed <= maxError;
}

#ifdef _WIN32
#pragma warning ( pop )
#endif

