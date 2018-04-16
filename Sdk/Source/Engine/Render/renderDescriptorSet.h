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

/// @file renderDescriptorSet.h
///       Render descriptor set layout interface

#include "Common/caveRefCount.h"
#include "Memory/allocatorGlobal.h"
#include "Common/caveVector.h"
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
class HalDescriptorSet;

/**
* @brief Interface for descriptor set layout setup
*/
class CAVE_INTERFACE RenderDescriptorSet : public CaveRefCount
{
public:

	/**
	* @brief Constructor
	*
	* @param[in] renderDevice			Pointer to render device
	* @param[in] descriptorSetLayouts	Pipeline set layouts array
	*
	*/
	RenderDescriptorSet(RenderDevice& renderDevice
		, caveVector<HalDescriptorSetLayout>& descriptorSetLayouts);

	/** @brief destructor */
	virtual ~RenderDescriptorSet();

	/**
	* @brief Get low level HAL handle
	*
	* @return HalPipelineLayout handle
	*/
	HalDescriptorSet* GetHalHandle() { return _halDescriptorSet; }

private:
	RenderDevice& _renderDevice;			///< Render device object
	HalDescriptorSet* _halDescriptorSet;	///< Pointer to low level pipeline layout object
};

}
/** @}*/



