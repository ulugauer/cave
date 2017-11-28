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

/// @file halRenderPass.h
///       Hardware render pass abstraction

#include "engineDefines.h"
#include "halInstance.h"
#include "halTypes.h"
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
* @brief Renderpass attachment description
*/
struct HalRenderPassAttachment
{
	HalImageFormat _format;		///< Image format
	HalSampleCount _samples;	///< Sample count
	HalLoadOperation _loadOp;	///< Select load operation for color / depth
	HalStoreOperation _storeOp;	///< Select store operation for color / depth
	HalLoadOperation _loadStencilOp;	///< Select load operation for stencil
	HalStoreOperation _storeStencilOp;	///< Select store operation for stencil
	HalImageLayout _initialLayout;	///< Layout at render pass start
	HalImageLayout _finalLayout;	///< Layout at render pass end
};

/**
* @brief Renderpass attachment reference
*/
struct HalAttachmentReference 
{
	uint32_t _attachment;		///< Reference index into HalRenderPassAttachment array
	HalImageLayout _layout;		///< Layout at render pass time
};

/**
* @brief Render sub-pass description
*/
struct HalSubpassDescription 
{
	HalPipelineBindPoints _pipelineBindPoint;				///< Pipeline bind point
	uint32_t  _inputAttachmentCount;						///< Input attachment count
	const HalAttachmentReference*    _pInputAttachments;	///< Input attachemnt array
	uint32_t _colorAttachmentCount;							///< Color attachment count
	const HalAttachmentReference* _pColorAttachments;		///< Color attachemnt array
	const HalAttachmentReference* _pResolveAttachments;		///< Resolve attachemnt array
	const HalAttachmentReference* _pDepthStencilAttachment; ///< Depth attachemnt array
	uint32_t _preserveAttachmentCount;						///< Preserve attachment count
	const uint32_t* _pPreserveAttachments;					///< Preserve attachemnt array
};

/**
* @brief Render sub-pass dependency description
*/
struct HalSubpassDependency 
{
	uint32_t _srcSubpass;					///< Subpass index of first subpass
	uint32_t _dstSubpass;					///< Subpass index of second subpass
	HalPipelineStageBits _srcStageMask;		///< Source stage mask
	HalPipelineStageBits _dstStageMask;		///< Dest stage mask
	HalAccessFlags _srcAccessMask;			///< Source access mask
	HalAccessFlags _dstAccessMask;			///< Dest access mask
	HalDependencyBits  _dependencyFlags;	///< Dependency flags
};

/**
* @brief Render pass create info
*/
struct HalRenderPassInfo 
{
	uint32_t _attachmentCount;						///< Render pass attachment count
	const HalRenderPassAttachment* _pAttachments;	///< Render pass attachments
	uint32_t _subpassCount;							///< Render sub-pass attachment count
	const HalSubpassDescription* _pSubpasses;		///< Render sub-pass attachments
	uint32_t _dependencyCount;						///< Render pass dependency attachment count
	const HalSubpassDependency*  _pDependencies;	///< Render pass dependencies
};

/**
* @brief Describes a single render pass
*/
class HalRenderPass
{
public:
	/**
	* @brief Constructor
	*
	* @param[in] renderDevice			Pointer to render device object
	* @param[in] renderPassInfo	Color	Render pass create info
	*/
	HalRenderPass(HalRenderDevice* renderDevice, HalRenderPassInfo& renderPassInfo);

	/** @brief Destructor */
	virtual ~HalRenderPass();

private:
	HalRenderDevice* _pDevice;	///< Pointer to device object
	HalRenderPassInfo _renderPassInfo;	///< Hal render pass create info
};

}

/** @}*/

