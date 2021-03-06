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

/// @file resourceManagerPrivate.cpp
///       Handle all kind of resources (images, shaders,...)

#include "resourceManagerPrivate.h"
#include "materialResource.h"
#include "imageResource.h"
#include "engineInstancePrivate.h"
#include "engineError.h"
#include "Render/renderMaterial.h"
#include "Render/renderShader.h"
#include "Render/renderTexture.h"

#include <fstream>

namespace cave
{

// our default relative locations
static const char* g_contentLocation = "/Content/";

//-----------------------------------------------------------------------------
// ResourceObjectFinder class
//-----------------------------------------------------------------------------
ResourceObjectFinder::ResourceObjectFinder(ResourceManagerPrivate& rm)
{
    std::string tmp = rm.GetProjectPath();
    if (!tmp.empty())
    {
        std::string projPath(tmp);
        projPath.append(g_contentLocation);
        _projectContentPath = projPath;
    }

    tmp = rm.GetApplicationPath();
    if (!tmp.empty())
    {
        std::string appPath(tmp);
        appPath.append(g_contentLocation);
        _appContentPath = appPath;
    }
}

bool ResourceObjectFinder::OpenFileAscii(const char* file, std::ifstream& inStream)
{
    // first search in project dir if available
    if (!_projectContentPath.empty())
    {
        for (size_t i = 0; i < _localSearchPath.size(); i++)
        {
            std::string projPath(_projectContentPath);
            projPath.append(_localSearchPath[i]);
            projPath.append(file);
            inStream.open(projPath.c_str(), std::ifstream::in);
            if (inStream.is_open())
            {
                return true;
            }
        }
    }

    if (!_appContentPath.empty())
    {
        for (size_t i = 0; i < _localSearchPath.size(); i++)
        {
            std::string appPath(_appContentPath);
            appPath.append(_localSearchPath[i]);
            appPath.append(file);
            inStream.open(appPath.c_str(), std::ifstream::in);
            if (inStream.is_open())
            {
                return true;
            }
        }
    }

    return false;
}

bool ResourceObjectFinder::OpenFileBinary(const char* file, std::ifstream& fileStream)
{
    // first search in project dir if available
    if (!_projectContentPath.empty())
    {
        for (size_t i = 0; i < _localSearchPath.size(); i++)
        {
            std::string projPath(_projectContentPath);
            projPath.append(_localSearchPath[i]);
            projPath.append(file);
            // we open the from the end to get the file content size
            fileStream.open(projPath.c_str(), std::ios::in | std::ios::binary);
            if (fileStream.is_open())
            {
                return true;
            }
        }
    }

    if (!_appContentPath.empty())
    {
        for (size_t i = 0; i < _localSearchPath.size(); i++)
        {
            std::string appPath(_appContentPath);
            appPath.append(_localSearchPath[i]);
            appPath.append(file);
            fileStream.open(appPath.c_str(), std::ios::in | std::ios::binary);
            if (fileStream.is_open())
            {
                return true;
            }
        }
    }

    return false;
}

std::string ResourceObjectFinder::GetFileName(const char* file)
{
    std::string input(file);
    size_t pos = input.find_last_of('/');

    if (pos != std::string::npos)
    {
        return input.substr(pos + 1, input.length());
    }

    return input;
}

std::string ResourceObjectFinder::GetDirectory(const char* file)
{
    std::string input(file);
    size_t pos = input.find_last_of('/');

    if (pos != std::string::npos)
    {
        return input.substr(0, pos + 1);
    }

    return std::string();
}

std::string ResourceObjectFinder::GetFileExt(const char* file)
{
    std::string fileString(file);
    size_t start = fileString.find_last_of(".");
    if (start != std::string::npos)
    {
        return fileString.substr(start + 1, fileString.length());
    }

    return std::string("");
}

bool ResourceObjectFinder::CaseInsensitiveStringCompare(const std::string& str1, const std::string& str2)
{
    // compare size first
    if (str1.size() != str2.size())
        return false;

    // compare char by char
    std::string::const_iterator s1 = str1.begin();
    std::string::const_iterator s2 = str2.begin();
    for (; s1 != str1.end(); ++s1, ++s2)
    {
        if (tolower(*s1) != tolower(*s2))
            return false;
    }

    return true;
}


//-----------------------------------------------------------------------------
// ResourceManagerPrivate class
//-----------------------------------------------------------------------------
ResourceManagerPrivate::ResourceManagerPrivate(RenderDevice* device
    , const char* applicationPath, const char* projectPath)
    : _pRenderDevice(device)
    , _appPath(applicationPath)
    , _projectPath(projectPath)
{

}

ResourceManagerPrivate::~ResourceManagerPrivate()
{
    // release shader
    TResourceShaderMap::iterator shaderIter;
    for (shaderIter = _shaderMap.begin(); shaderIter != _shaderMap.end(); ++shaderIter)
    {
        DeallocateDelete(*_pRenderDevice->GetEngineAllocator(), *shaderIter->second);
    }
    _shaderMap.clear();

    // release materials
    TResourceMaterialMap::iterator matIter;
    for (matIter = _materialMap.begin(); matIter != _materialMap.end(); ++matIter)
    {
        if (matIter->second)
        {
            DeallocateDelete(*_pRenderDevice->GetEngineAllocator(), *matIter->second);
        }
    }
    _materialMap.clear();

    // stop threads
    TResourceLoadingThreadMap::iterator threadIter;
    for (threadIter = _loadingThreadMap.begin(); threadIter != _loadingThreadMap.end(); ++threadIter)
    {
        if (threadIter->second.valid())
            threadIter->second.wait();
    }
    _loadingThreadMap.clear();

    // release image resources
    TResourceImageMap::iterator imageIter;
    for (imageIter = _imageMap.begin(); imageIter != _imageMap.end(); ++imageIter)
    {
        if (imageIter->second)
        {
            DeallocateDelete(*_pRenderDevice->GetEngineAllocator(), *imageIter->second);
        }
    }
    _imageMap.clear();

}

std::shared_ptr<AllocatorGlobal>
ResourceManagerPrivate::GetEngineAllocator()
{
    return _pRenderDevice->GetEngineAllocator();
}

RenderShader* ResourceManagerPrivate::FindRenderShaderResource(const char* fileName)
{
    std::string stringKey(fileName);
    TResourceShaderMap::const_iterator entry = _shaderMap.find(stringKey);
    if (entry != _shaderMap.end())
        return entry->second;

    return nullptr;
}

bool ResourceManagerPrivate::InsertRenderShaderResource(const char* fileName, RenderShader* shader)
{
    if (FindRenderShaderResource(fileName) || !shader || !fileName)
        return false;

    std::string stringKey(fileName);
    _shaderMap.insert(TResourceShaderMap::value_type(stringKey, shader));

    return true;
}

RenderMaterial* ResourceManagerPrivate::LoadMaterialAsset(const char* file)
{
    ResourceObjectFinder objectFinder(*this);
    MaterialResource mr(this);

    std::string stringKey(file);
    // check if material already exists
    TResourceMaterialMap::const_iterator entry = _materialMap.find(stringKey);
    if (entry != _materialMap.end())
        return entry->second;


    RenderMaterial* material = mr.LoadMaterialAsset(objectFinder, file);
    _materialMap.insert(TResourceMaterialMap::value_type(stringKey, material));

    return material;
}

void ResourceManagerPrivate::LoadImageAsset(const char* file)
{
    ResourceObjectFinder objectFinder(*this);

    std::string ext = objectFinder.GetFileExt(file);
    if (!ImageResource::IsImageFormatSupported(objectFinder, ext.c_str()) ||
        !ImageResource::ImageFileExists(objectFinder, file))
    {
        return;
    }

    std::string stringKey(file);
    // check if image already exists
    TResourceImageMap::const_iterator entry = _imageMap.find(stringKey);
    if (entry != _imageMap.end())
        return;

    // create a new object and start async loading
    ImageResource* image = ImageResource::CreateImageResource(this, objectFinder, file);
    if (image)
    {
        _imageMap.insert(TResourceImageMap::value_type(stringKey, image));
        _loadingThreadMap.insert(TResourceLoadingThreadMap::value_type(stringKey, std::async(&ResourceManagerPrivate::LoadImageFile, this, file, image)));
    }
}

RenderTexture* ResourceManagerPrivate::GetTexture(const char* file)
{
    std::string stringKey(file);
    // check if image already exists
    TResourceTextureMap::const_iterator entry = _textureMap.find(stringKey);
    if (entry != _textureMap.end())
        return entry->second;

    // find matching imageResource object
    ImageResource* image = GetImageResource(file);
    if (image == nullptr)
        return nullptr;

    // Get image data
    ImageData imageData = image->getImageData();
    // Fill in data
    HalImageInfo imageInfo;
    imageInfo._width = imageData.width;
    imageInfo._height = imageData.height;
    imageInfo._depth = 1;
    imageInfo._slices = 1;
    imageInfo._level = imageData.levels;
    imageInfo._format = imageData.format;
    imageInfo._componentCount = imageData.componentCount;
    imageInfo._componentSize = imageData.componentSize;
    imageInfo._usage = (static_cast<HalImageUsageFlags>(HalImageUsageFlagBits::TransferDst) | static_cast<HalImageUsageFlags>(HalImageUsageFlagBits::Sampled));

    RenderTexture* texture = nullptr;
    try
    {
        texture = AllocateObject<RenderTexture>(*GetEngineAllocator(), *_pRenderDevice, imageInfo, file);
        if (texture)
        {
            texture->AddRef();
            _textureMap.insert(TResourceTextureMap::value_type(stringKey, texture));
            // allocate memory
            texture->Bind();
            // upload data
            texture->Update(imageData.data);
        }
    }
    catch (std::exception&)
    {
        return texture;
    }

    return texture;
}

void ResourceManagerPrivate::ReleaseTexture(RenderTexture* texture)
{
    std::string stringKey(texture->GetFileName());
    // check if image already exists
    TResourceTextureMap::const_iterator entry = _textureMap.find(stringKey);
    if (entry == _textureMap.end())
        return;

    if (texture->GetRefCount() == 1)
    {
        // final release
        _textureMap.erase(stringKey);
        texture->Relase();
    };
}

ImageResource* ResourceManagerPrivate::GetImageResource(const char* file)
{
    std::string stringKey(file);

    // Make sure data is available
    TResourceLoadingThreadMap::const_iterator threadEntry = _loadingThreadMap.find(stringKey);
    if (threadEntry != _loadingThreadMap.end())
        threadEntry->second.wait();

    // return image resource
    TResourceImageMap::const_iterator imageEntry = _imageMap.find(stringKey);
    if (imageEntry != _imageMap.end())
        return imageEntry->second;

    // not found
    return nullptr;
}

/**
* Note this function is called in a thread
*/
void ResourceManagerPrivate::LoadImageFile(const char* file, ImageResource* image)
{
    ResourceObjectFinder objectFinder(*this);
    ImageResource::LoadImageResource(objectFinder, image, file);
}


}
