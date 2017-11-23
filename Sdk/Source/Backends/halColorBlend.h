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

/// @file halColorBlend.h
///       Hardware color blending state abstraction

#include "engineDefines.h"
#include "Memory/allocatorBase.h"
#include "Common/caveVector.h"

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
*  @brief A strongly typed enum class representing blending factors
*/
enum class BlendFactor
{
	Zero = 0,
	One,
	SrcColor,
	OneMinusSrcColor,
	DstColor,
	OneMinusDstColor,
	SrcAlpha,
	OneMinusSrcAlpha,
	DstAlpha,
	OneMinusDstAlpha,
	ConstantColor,
	OneMinusConstantColor,
	ConstantAlpha,
	OneMinusConstantAlpha,
	SrcAlphaSaturate,
	Src1Color,
	OneMinusSrc1Color,
	Src1Alpha,
	OneMinusSrc1Alpha
};

/**
*  @brief A strongly typed enum class representing blending operations
*/
enum class BlendOp
{
	Add = 0,
	Subtract,
	ReverseSubtract,
	Min,
	Max
};

/**
*  @brief A strongly typed enum class representing color write mask
*/
enum class ColorComponents
{
	Red = 1,
	Green = 2,
	Blue = 4,
	Alpha = 8
};

typedef uint32_t ColorComponentFlags;	///< Combined color component flags

/**
*  @brief A strongly typed enum class representing logic operations
*/
enum class LogicOp
{
	Clear = 0,
	And,
	AndReverse,
	Copy,
	AndInverted,
	NoOp,
	Xor,
	Or,
	Nor,
	Equivalent,
	Invert,
	OrReverse,
	CopyInverted,
	OrInverted,
	Nand,
	Set
};

/**
* @brief Color blending attachment data
*/
struct HalColorBlendAttachment
{
	bool _blendEnable;						///< Enable blending for this attachment
	BlendFactor _srcColorBlendFactor;		///< Select source blend factor
	BlendFactor _dstColorBlendFactor;		///< Select dest blend factor
	BlendOp _colorBlendOp;					///< Select blend operation applied to color values
	BlendFactor _srcAlphaBlendFactor;		///<  Select source alpha blend factor
	BlendFactor _dstAlphaBlendFactor;		///< Select cdst alpha blend factor
	BlendOp _alphaBlendOp;					///< Select blend operation applied to alpha values
	ColorComponentFlags _colorWriteMask;	///< Determines to which color compoent we are writing
};

/**
* @brief Color blending state
*/
struct HalColorBlendState
{
	bool _logicOpEnable;					///< Enable logical operations
	LogicOp _logicOp;						///< Select logical operation
	float _blendConstants[4];				///< RGBA constant values used for blending
};

/**
* @brief Describes the color blend state
*/
class HalColorBlend
{
public:
	/**
	* @brief Constructor
	*
	* @param[in] renderDevice		Pointer to render device object
	* @param[in] colorBlendState	Color blending state data
	* @param[in] blendAttachments	Color blend attachment state
	*/
	HalColorBlend(HalRenderDevice* renderDevice, HalColorBlendState& colorBlendState
				, caveVector<HalColorBlendAttachment>& blendAttachments);

	/** @brief Destructor */
	virtual ~HalColorBlend();

private:
	HalRenderDevice* _pDevice;	///< Pointer to device object
	HalColorBlendState _colorBlendState;	///< Color blend state state
	caveVector<HalColorBlendAttachment> _colorBlendAttachments;	///< Color blend attachment state
};

}

/** @}*/

