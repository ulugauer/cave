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

#include "imageResource.h"
#include "imageResourceDds.h"
#include "engineError.h"
#include "Math/vector4.h"

#include <fstream>
#include <iostream>


namespace cave
{

// our default relative locations for materials and shaders
static const char* g_imageLocation = "Images/";

ImageResource::ImageResource(ResourceManagerPrivate* rm)
	: _pResourceManagerPrivate(rm)
{

}

ImageResource::~ImageResource()
{
}

bool ImageResource::IsImageFormatSupported(ResourceObjectFinder& objectFinder, const char* ext)
{
	// supported formats
	std::string dds("dds");
	std::string filext = ext;

	if (objectFinder.CaseInsensitiveStringCompare(filext, dds))
		return true;

	return false;
}

bool ImageResource::ImageFileExists(ResourceObjectFinder& objectFinder, const char* filename)
{
	std::string fileString = objectFinder.GetFileName(filename);
	std::string directory = objectFinder.GetDirectory(filename);

	// add local search path
	if (!directory.empty())
		objectFinder._localSearchPath.push_back(directory);

	// add default local serach path
	objectFinder._localSearchPath.push_back(g_imageLocation);

	std::ifstream fileStream;
	if (!objectFinder.OpenFileBinary(fileString.c_str(), fileStream))
	{
		return false;
	}

	fileStream.close();
	return true;
}

ImageResource* ImageResource::CreateImageResource(ResourceManagerPrivate* rm, ResourceObjectFinder& objectFinder, const char* filename)
{
	std::string ext = objectFinder.GetFileExt(filename);
	std::string dds("dds");
	std::string filext = ext;

	ImageResource* image = nullptr;
	if (objectFinder.CaseInsensitiveStringCompare(filext, dds))
	{
		image = AllocateObject<ImageResourceDds>(*rm->GetEngineAllocator(), rm);
	}

	return image;
}

void ImageResource::LoadImageResource(ResourceObjectFinder& objectFinder, ImageResource* image, const char* filename)
{
	std::string fileString = objectFinder.GetFileName(filename);
	std::string directory = objectFinder.GetDirectory(filename);

	// add local search path
	if (!directory.empty())
		objectFinder._localSearchPath.push_back(directory);

	// add default local serach path
	objectFinder._localSearchPath.push_back(g_imageLocation);

	std::ifstream fileStream;
	if (objectFinder.OpenFileBinary(fileString.c_str(), fileStream))
	{
		image->decode(false, fileStream);
		fileStream.close();
	}
}

}
