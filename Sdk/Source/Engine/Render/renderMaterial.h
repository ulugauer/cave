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

/// @file renderMaterial.h
///       Render material interface

#include "engineTypes.h"
#include "Memory/allocatorGlobal.h"
#include "Math/vector4.h"

#include <memory>

/** \addtogroup engine
*  @{
*		This module contains all code related to the engine
*/

namespace cave
{

/// forward declaration
class RenderDevice;

/**
* Material data
*/
struct CAVE_INTERFACE RenderMaterialDataStruct
{
	Vector4f _ambient;	///< Ambient material color
	Vector4f _diffuse;	///< Diffuse material color
	Vector4f _emissive;	///< Emissive material color
	float _opacity;		///< Material opacity
	float _padding[3];	///< structure padding

	RenderMaterialDataStruct() {}
	RenderMaterialDataStruct(const RenderMaterialDataStruct& rhs)
	{
		_ambient = rhs._ambient;
		_diffuse = rhs._diffuse;
		_emissive = rhs._emissive;
		_opacity = rhs._opacity;
	}

	RenderMaterialDataStruct& operator=(const RenderMaterialDataStruct& rhs)
	{
		_ambient = rhs._ambient;
		_diffuse = rhs._diffuse;
		_emissive = rhs._emissive;
		_opacity = rhs._opacity;

		return *this;
	}
};



/**
* Interface of renderer materials
*/
class CAVE_INTERFACE RenderMaterial
{
public:
	/**
	* @brief Constructor
	*
	* @param[in] renderDevice	Pointer to render device
	*
	*/
	RenderMaterial(RenderDevice& renderDevice);
	/** @brief Destructor */
	~RenderMaterial();

	/**
	* @brief Set ambient color
	*
	* @param[in] ambient	Ambient color vector
	*
	*/
	void SetAmbientColor(Vector4f ambient)
	{
		_materialData._ambient = ambient;
	}

	/**
	* @brief Set ambient color
	*
	* @param[in] ambient	Ambient color vector
	*
	*/
	void SetDiffuseColor(Vector4f diffuse)
	{
		_materialData._diffuse = diffuse;
	}

private:
	RenderDevice& _renderDevice;	///< Render device object
	RenderMaterialDataStruct _materialData;	///< Material values
};

}

/** @}*/

