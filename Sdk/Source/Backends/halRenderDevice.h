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

/// @file halRenderDevice.h
///       Hardware render device abstraction

#include "engineDefines.h"
#include "halInstance.h"
#include "halCommandPool.h"
#include "halShader.h"
#include "halVertexInput.h"
#include "halInputAssembly.h"
#include "halViewportAndScissor.h"
#include "halRasterizerState.h"
#include "halMultisample.h"
#include "halDepthStencil.h"
#include "halColorBlend.h"
#include "halDynamicState.h"
#include "halPipelineLayout.h"
#include "halRenderPass.h"
#include "halGraphicsPipeline.h"
#include "halSemaphore.h"
#include "halCommandBuffer.h"
#include "halBuffer.h"
#include "Memory/allocatorBase.h"

#include <iostream>		// includes exception handling
#include <memory>

/** \addtogroup backend
*  @{
*
*/

namespace cave
{


/**
* Abstraction type of a device instance
*/
class HalRenderDevice
{
public:
	/**
	* @brief Constructor
	*
	*/
	HalRenderDevice(HalInstance* instance);

	/** @brief Destructor */
	virtual ~HalRenderDevice();

	/**
	* @brief GetAllocator
	*
	* @return Pointer Engine allocator
	*/
	std::shared_ptr<AllocatorBase> GetEngineAllocator() {
		return _pInstance->GetEngineAllocator();
	}

	/**
	* @brief Create a swap chain
	*
	* @param[in] swapChainInfo	Swap chain creation info
	*
	*/
	virtual void CreateSwapChain(SwapChainInfo& swapChainInfo) = 0;

	/**
	* @brief Create a low level shader
	*
	* @param[in] type		Shader type
	* @param[in] language	Shader language
	*
	* @return shader abstraction interface
	*/
	virtual HalShader* CreateShader(ShaderType type, ShaderLanguage language) = 0;

	/**
	* @brief Create a low level command pool object
	*
	* @param[in] commandPoolInfo	Command pool creation info
	*
	* @return HalCommandPool abstraction interface
	*/
	virtual HalCommandPool* CreateCommandPool(HalCommandPoolInfo& commandPoolInfo) = 0;

	/**
	* @brief Create a low level vertex input state
	*
	* @param[in] vertexInputState	Vertex input setup struct
	*
	* @return Vertex input abstraction interface
	*/
	virtual HalVertexInput* CreateVertexInput(HalVertexInputStateInfo& vertexInputState) = 0;

	/**
	* @brief Create a low level input assembly state
	*
	* @return Input assembly abstraction interface
	*/
	virtual HalInputAssembly* CreateInputAssembly() = 0;

	/**
	* @brief Create a low level viewport, scissor state
	*
	* @param[in] viewport	Viewport setup
	* @param[in] scissor	Scissor setup
	*
	* @return  Viewport, scissor abstraction interface
	*/
	virtual HalViewportAndScissor* CreateViewportAndScissor(HalViewport& viewport, HalScissor& scissor) = 0;

	/**
	* @brief Create a low level rasterizer state
	*
	* @param[in] rasterizerStateInfo	Rasterizer setup info
	*
	* @return  rasterizer abstraction interface
	*/
	virtual HalRasterizerState* CreateRasterizerState(HalRasterizerSetup& rasterizerStateInfo) = 0;

	/**
	* @brief Create a low level multisample state
	*
	* @param[in] multisampleStateInfo	Multisample setup info
	*
	* @return  Multisample abstraction interface
	*/
	virtual HalMultisample* CreateMultisampleState(HalMultisampleState& multisampleStateInfo) = 0;

	/**
	* @brief Create a low level depth stencil state
	*
	* @param[in] depthStencilInfo	Depth Stencil setup info
	*
	* @return  DepthStencil abstraction interface
	*/
	virtual HalDepthStencil* CreateDepthStencilState(HalDepthStencilSetup& depthStencilInfo) = 0;

	/**
	* @brief Create a low level depth stencil state
	*
	* @param[in] colorBlendInfo		Color blending state data
	* @param[in] blendAttachments	Color blend attachment state
	*
	* @return  DepthStencil abstraction interface
	*/
	virtual HalColorBlend* CreateColorBlendState(HalColorBlendState& colorBlendInfo
			, caveVector<HalColorBlendAttachment>& blendAttachments) = 0;

	/**
	* @brief Create a low level dynamic state
	*
	* @param[in] dynamicStates		Dynamic states array
	*
	* @return  HalDynamicState abstraction interface
	*/
	virtual HalDynamicState* CreateDynamicState(caveVector<HalDynamicStates>& dynamicStates) = 0;

	/**
	* @brief Create a pipeline layout object
	*
	* @param[in] descriptorSetLayouts	Pipeline set layouts array
	* @param[in] pushConstants			Pipeline push constant ranges array
	*
	* @return  HalPipelineLayout abstraction interface
	*/
	virtual HalPipelineLayout* CreatePipelineLayout(caveVector<HalDescriptorSetLayout>& descriptorSetLayouts
		, caveVector<HalPushConstantRange>& pushConstants) = 0;

	/**
	* @brief Create a low level render pass object
	*
	* @param[in] renderPassInfo		Render pass setup info
	*
	* @return  HalRenderPass abstraction interface
	*/
	virtual HalRenderPass* CreateRenderPass(HalRenderPassInfo& renderPassInfo) = 0;

	/**
	* @brief Create a graphics pipeline object
	*
	* @param[in] graphicsPipelineInfo	Graphics pipeline setup struct
	*
	* @return  HalGraphicsPipeline abstraction interface
	*/
	virtual HalGraphicsPipeline* CreateGraphicsPipeline(HalGraphicsPipelineInfo& graphicsPipelineInfo) = 0;

	/**
	* @brief Create semaphore object
	*
	* @return  HalSemaphore abstraction interface
	*/
	virtual HalSemaphore* CreateSemaphore() = 0;

	/**
	* @brief Create a buffer object
	*
	* @param[in] bufferInfo		Buffer create info
	*
	* @return  HalBuffer abstraction interface
	*/
	virtual HalBuffer* CreateBuffer(HalBufferInfo& bufferInfo) = 0;

	/**
	* @brief Allocate command buffers
	*
	* @param[in] commandPool		Command pool object
	* @param[in] commandBufferInfo	Command buffer creation info
	* @param[out] commandBuffers	Allocated command buffers
	*
	* @return True on success
	*/
	virtual bool AllocateCommandBuffers(HalCommandPool* commandPool
									 , HalCommandBufferInfo& commandBufferInfo
									 , caveVector<HalCommandBuffer*>& commandBuffers) = 0;

	/**
	* @brief Begin command buffer recording
	*
	* @param[in] commandBuffer				Command buffer we use for recording
	* @param[in] commandBufferBeginInfo		Command buffer begin setup
	*/
	virtual void BeginCommandBuffer(HalCommandBuffer* commandBuffer, HalCommandBufferBeginInfo& commandBufferBeginInfo) = 0;

	/**
	* @brief End command buffer recording
	*
	* @param[in] commandBuffer	Command buffer we use for recording
	*/
	virtual void EndCommandBuffer(HalCommandBuffer* commandBuffer) = 0;

	/**
	* @brief Begin render pass command
	*
	* @param[in] commandBuffer			Command buffer we use for recording
	* @param[in] RenderPassBeginInfo	Command info for render pass
	* @param[in] subpass				Subpass content in primary or secondary command buffers
	*/
	virtual void CmdBeginRenderPass(HalCommandBuffer* commandBuffer, HalCmdRenderPassInfo& RenderPassBeginInfo, HalSubpassContents subpass) = 0;

	/**
	* @brief End command buffer recording
	*
	* @param[in] commandBuffer	Command buffer we use for recording
	*/
	virtual void CmdEndRenderPass(HalCommandBuffer* commandBuffer) = 0;

	/**
	* @brief Bind graphics pipeline
	*
	* @param[in] commandBuffer			Command buffer we use for recording
	* @param[in] graphicsPipelineInfo	Graphics pipeline object
	*/
	virtual void CmdBindGraphicsPipeline(HalCommandBuffer* commandBuffer, HalGraphicsPipeline* graphicsPipelineInfo) = 0;

	/**
	* @brief Draw command for non indexed drawing
	*
	* @param[in] commandBuffer	Command buffer we use for recording
	* @param[in] vertexCount	Number of vertices to draw
	* @param[in] instanceCount	Number of instances to draw
	* @param[in] firstVertex	Index of the first vertex to draw
	* @param[in] firstInstance	Instance ID of the first instance to draw
	*/
	virtual void CmdDraw(HalCommandBuffer* commandBuffer, uint32_t vertexCount, uint32_t instanceCount
		, uint32_t firstVertex, uint32_t firstInstance) = 0;

	/**
	* @brief A special submit for swap chain image presentation
	*
	* @param[in] commandBuffer			Command buffer we use for recording
	*
	* @return true if successful
	*/
	virtual bool PresentQueueSubmit(HalCommandBuffer* commandBuffer) = 0;

	/**
	* @brief Present Image on scren
	*
	* @param[in] imageIndex		Image to present
	*
	* @return true if successful
	*/
	virtual bool PresentQueue(uint32_t imageIndex) = 0;

	/**
	* @brief Get swap chain image format
	*
	* @return Image format
	*/
	virtual const HalImageFormat GetSwapChainImageFormat() = 0;

	/**
	* @brief Get swap chain image count
	*
	* @return Image count
	*/
	virtual const uint32_t GetSwapChainImageCount() = 0;

	/**
	* @brief Get swap chain image extend
	*
	* @return Image extend
	*/
	virtual const HalExtent2D GetSwapChainExtend() = 0;

	/**
	* @brief Get next available swap chain image
	*
	* @param[in] timeout		Timeout in nano seconds
	*
	* @return Image index
	*/
	virtual const uint32_t AcquireNextSwapChainImage(uint64_t timeout) = 0;

	/**
	* @brief Create presentation framebuffers
	*		 This functions creates as much framebuffers as swap chain images.
	*		 This functions throws an exception if soemthing goes wrong
	*
	* @param[in] renderPass		RenderPass object which we need to attach to the framebuffers
	*
	*/
	virtual void CreateSwapChainFramebuffers(HalRenderPass* renderPass) = 0;

private:
	HalInstance* _pInstance;	///< Pointer to instance object
};

}

/** @}*/
