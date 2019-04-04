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

/// @file renderDevice.h
///       Render device interface

#include "Resource/resourceManager.h"
#include "halRenderDevice.h"
#include "engineTypes.h"
#include "frontend.h"

#include <map>

/** @addtogroup engine
*  @{
*		This module contains all code related to the engine
*/

namespace cave
{

/// forward declaration
class HalInstance;
class HalRenderDevice;
class RenderInstance;
class RenderVertexInput;
class RenderInputAssembly;
struct RenderLayerSectionInfo;
class RenderLayerSection;
class RenderRasterizerState;
class RenderDepthStencil;
class EngineLog;
class RenderMultisample;
class RenderColorBlend;
class RenderDynamicState;
class RenderPipelineLayout;
class RenderPass;
struct RenderGraphicsPipelineInfo;
class RenderGraphicsPipeline;
class RenderCommandPool;
class RenderCommandBuffer;
class RenderDescriptorPool;
class RenderDescriptorSet;
struct RenderDescriptorBufferInfo;
struct RenderWriteDescriptorSet;
struct RenderCmdRenderPassInfo;
class RenderVertexBuffer;
class RenderIndexBuffer;
class RenderUniformBuffer;
class RenderTexture;


/**
* Abstraction type of a device instance
*/
class CAVE_INTERFACE RenderDevice
{
public:
	/**
	* @brief Constructor
	*
	* @param[in] renderInstance	Pointer to render instance
	* @param[in] halInstance	Pointer to HAL render instance
	* @param[in] windowInfo	Window creation info
	*
	*/
	RenderDevice(RenderInstance* renderInstance, HalInstance* halInstance, FrontendWindowInfo& windowInfo);
	/** @brief Destructor */
	~RenderDevice();

	/**
	* @brief GetAllocator
	*
	* @return Pointer Engine allocator
	*/
	std::shared_ptr<AllocatorGlobal> GetEngineAllocator();

	/**
	* @brief GetEngineLog
	*
	* @return Pointer to engine logging
	*/
	EngineLog* GetEngineLog() const;

	/**
	* @brief Query API version number
	*
	* @param[out] major		Major API version number
	* @param[out] minor		Minor API version number
	* @param[out] patch		Patch API version number
	*
	*/
	void GetApiVersion(uint32_t& major, uint32_t& minor, uint32_t& patch);

	/**
	* @brief Query Driver version number
	*
	* @param[out] major		Major driver version number
	* @param[out] minor		Minor driver version number
	* @param[out] patch		Patch driver version number
	*
	*/
	void GetDriverVersion(uint32_t& major, uint32_t& minor, uint32_t& patch);

	/**
	* @brief Query device name
	*
	* return Pointer string
	*/
	const char* GetDeviceName();

	/**
	* @brief Hardcore function for everything has processed
	*	Use with caution
	*
	*/
	void WaitIdle();

	/**
	* @brief Create a swap chain
	*
	*/
	void CreateSwapChain();

	/**
	* @brief Create presentation framebuffers
	*		 This functions creates as much framebuffers as swap chain images.
	*
	*/
	bool CreateSwapChainFramebuffers();

	/**
	* @brief Get swap chain image format
	*
	* @return Image format
	*/
	const HalImageFormat GetSwapChainImageFormat();

	/**
	* @brief Get swap chain depth image format
	*
	* @return Image format
	*/
	const HalImageFormat GetSwapChainDepthImageFormat();

	/**
	* @brief Get swap chain image count
	*
	* @return Image count
	*/
	const uint32_t GetSwapChainImageCount();

	/**
	* @brief Get next available swap chain image
	*
	* @param[in] timeout		Timeout in nano seconds
	*
	* @return Image index
	*/
	const uint32_t AcquireNextSwapChainImage(uint64_t timeout);

	/**
	* @brief Get Resource manager
	*
	* @return ResourceManager Object
	*/
	ResourceManager& GetResourceManager()
	{
		return *_pResourceManager;
	}

	/**
	* @brief Get pointer to HAL render device
	*
	* @return Low level HAL device object
	*/
	HalRenderDevice* GetHalRenderDevice()
	{
		return _pHalRenderDevice;
	}

	/**
	* @brief Create a command pool object
	*
	* @param[in] commandPoolInfo	Command pool creation info
	*
	* @return RenderCommandPool object
	*/
	RenderCommandPool* CreateCommandPool(HalCommandPoolInfo& commandPoolInfo);

	/**
	* @brief Release a command pool object
	*
	* @param[in] commandPool	RenderCommandPool object to release
	*/
	void ReleaseCommandPool(RenderCommandPool* commandPool);

	/**
	* @brief Create a descriptor pool object
	*
	* @param[in] deescriptorPoolInfo	Descriptor pool creation info
	*
	* @return RenderDescriptorPool object
	*/
	RenderDescriptorPool* CreateDescriptorPool(HalDescriptorPoolInfo& deescriptorPoolInfo);

	/**
	* @brief Release a descriptor pool object
	*
	* @param[in] descriptorPool	RenderDescriptorPool object to release
	*/
	void ReleaseDescriptorPool(RenderDescriptorPool* descriptorPool);

	/**
	* @brief Create descriptor set layout objects
	*
	* @param[in] descriptorSetLayouts	Descriptor set layout array
	*
	* @return  RenderDescriptorSet object
	*/
	RenderDescriptorSet* CreateDescriptorSets(caveVector<HalDescriptorSetLayout>& descriptorSetLayouts);

	/**
	* @brief Release a descriptor set object
	*
	* @param[in] descriptorSet	RenderDescriptorSet object to release
	*/
	void ReleaseDescriptorSets(RenderDescriptorSet* descriptorSet);

	/**
	* @brief Update a descriptor set object
	* Note do not overwrite descriptor sets while used in command buffer recording
	* or rendering
	*
	* @param[in] descriptorWrites	Array of RenderWriteDescriptorSet
	*/
	void UpdateDescriptorSets(caveVector<RenderWriteDescriptorSet>& descriptorWrites);

	/**
	* @brief Create a vertex input object
	*
	* @param[in] vertexInputState	Vertex input setup struct
	*
	* @return Vertex input object
	*/
	RenderVertexInput* CreateVertexInput(HalVertexInputStateInfo& vertexInputState);

	/**
	* @brief Release a vertex input object
	*
	* @param[in] vertexInput	Vertex input object to release
	*/
	void ReleaseVertexInput(RenderVertexInput* vertexInput);

	/**
	* @brief Create an input assembly object
	*
	* @param[in] inputAssemblyState		Vertex input assembley setup
	*
	* @return Input assembly object
	*/
	RenderInputAssembly* CreateInputAssembly(HalInputAssemblyInfo& inputAssemblyState);

	/**
	* @brief Release an input assembly object
	*
	* @param[in] inputAssembly	 Input assembly object to release
	*/
	void ReleaseInputAssembly(RenderInputAssembly* inputAssembly);

	/**
	* @brief Create a layer section object
	*
	* @param[in] sectionInfo	 Section setup info
	*
	* @return RenderLayerSection object
	*/
	RenderLayerSection* CreateLayerSection(RenderLayerSectionInfo& sectionInfo);

	/**
	* @brief Release a layer section object
	*
	* @param[in] layerSection	 RenderLayerSectionInfo object to release
	*/
	void ReleaseLayerSection(RenderLayerSection* layerSection);

	/**
	* @brief Create a rasterizer state object
	*
	* @param[in] rasterizerInfo	 Rasterizer setup info
	*
	* @return RenderRasterizerState object
	*/
	RenderRasterizerState* CreateRasterizerState(HalRasterizerSetup& rasterizerInfo);

	/**
	* @brief Release a rasterizer object
	*
	* @param[in] rasterizerState	 RenderRasterizerState object to release
	*/
	void ReleaseRasterizerState(RenderRasterizerState* rasterizerState);

	/**
	* @brief Create a multisample state object
	*
	* @param[in] multisampleInfo	 Multisample setup info
	*
	* @return RenderMultisample object
	*/
	RenderMultisample* CreateMultisampleState(HalMultisampleState& multisampleInfo);

	/**
	* @brief Release a multisample object
	*
	* @param[in] multisampleState	 RenderMultisample object to release
	*/
	void ReleaseMultisampleState(RenderMultisample* multisampleState);

	/**
	* @brief Create a depth stencil state object
	*
	* @param[in] depthStencilInfo	 Depth stencil setup info
	*
	* @return RenderDepthStencil object
	*/
	RenderDepthStencil* CreateDepthStencilState(HalDepthStencilSetup& depthStencilInfo);

	/**
	* @brief Release a multisample object
	*
	* @param[in] depthStencilState	 RenderDepthStencil object to release
	*/
	void ReleaseDepthStencilState(RenderDepthStencil* depthStencilState);

	/**
	* @brief Create a color blend state object
	*
	* @param[in] colorBlendInfo		Color blending state data
	* @param[in] blendAttachments	Color blend attachment state
	*
	* @return RenderColorBlend object
	*/
	RenderColorBlend* CreateColorBlendState(HalColorBlendState& colorBlendInfo
			, caveVector<HalColorBlendAttachment>& blendAttachments);

	/**
	* @brief Release a  color blend object
	*
	* @param[in] colorBlendState	 RenderColorBlend object to release
	*/
	void ReleaseColorBlendState(RenderColorBlend* colorBlendState);

	/**
	* @brief Create a dynamic state object
	*
	* @param[in] dynamicStates		Dynamic states array
	*
	* @return RenderDynamicState object
	*/
	RenderDynamicState* CreateDynamicState(caveVector<HalDynamicStates>& dynamicStates);

	/**
	* @brief Release a dynamic object
	*
	* @param[in] dynamicState	 RenderDynamicState object to release
	*/
	void ReleaseDynamicState(RenderDynamicState* dynamicState);

	/**
	* @brief Create a pipeline layout object
	*
	* @param[in] descriptorSet	Pointer to descriptor set layout object
	* @param[in] pushConstants	Pipeline push constant ranges array
	*
	* @return RenderPipelineLayout object
	*/
	RenderPipelineLayout* CreatePipelineLayout(RenderDescriptorSet* descriptorSet
		, caveVector<HalPushConstantRange>& pushConstants);

	/**
	* @brief Release a pipeline layout object
	*
	* @param[in] pipelineLayout	 RenderColorBlend object to release
	*/
	void ReleasePipelineLayout(RenderPipelineLayout* pipelineLayout);

	/**
	* @brief Create a vertex buffer object
	*
	* @param[in] bufferInfo		Buffer create info
	*
	* @return RenderVertexBuffer object
	*/
	RenderVertexBuffer* CreateVertexBuffer(HalBufferInfo& bufferInfo);

	/**
	* @brief Release a vertex buffer object
	*
	* @param[in] vertexBuffer	 RenderVertexBuffer object to release
	*/
	void ReleaseVertexBuffer(RenderVertexBuffer* vertexBuffer);

	/**
	* @brief Create a index buffer object
	*
	* @param[in] bufferInfo		Buffer create info
	* @param[in] indexType		Index tpye like [uint16 | uint32]
	*
	* @return RenderIndexBuffer object
	*/
	RenderIndexBuffer* CreateIndexBuffer(HalBufferInfo& bufferInfo, HalIndexType indexType);

	/**
	* @brief Release a index buffer object
	*
	* @param[in] indexBuffer	 RenderIndexBuffer object to release
	*/
	void ReleaseIndexBuffer(RenderIndexBuffer* indexBuffer);

	/**
	* @brief Create a uniform buffer object
	*
	* @param[in] bufferInfo		Buffer create info
	*
	* @return RenderIndexBuffer object
	*/
	RenderUniformBuffer* CreateUniformBuffer(HalBufferInfo& bufferInfo);

	/**
	* @brief Release a uniform buffer object
	*
	* @param[in] indexBuffer	 RenderIndexBuffer object to release
	*/
	void ReleaseUniformBuffer(RenderUniformBuffer* indexBuffer);

	/**
	* @brief Create a texture object
	* Note the image specified in file needs to be loaded before this call
	* using the resource manager.
	*
	* @param[in] file		Filename
	*
	* @return RenderIndexBuffer object
	*/
	RenderTexture* CreateTexture(const char* file);

	/**
	* @brief Release a texture object
	*
	* @param[in] texture	 RenderTexture object to release
	*/
	void ReleaseTexture(RenderTexture* texture);

	/**
	* @brief Create a render pass object
	*
	* @param[in] renderPassInfo		Render pass setup info
	*
	* @return RenderPass object
	*/
	RenderPass* CreateRenderPass(HalRenderPassInfo& renderPassInfo);

	/**
	* @brief Release a render pass object
	*
	* @param[in] renderPass	 RenderPass object to release
	*/
	void ReleaseRenderPass(RenderPass* renderPass);

	/**
	* @brief Create a graphics pipeline object
	*
	* @param[in] graphicsPipelineInfo	Graphics pipeline setup struct
	*
	* @return RenderGraphicsPipeline object
	*/
	RenderGraphicsPipeline* CreateGraphicsPipeline(RenderGraphicsPipelineInfo& graphicsPipelineInfo);

	/**
	* @brief Allocte command buffers for submits
	*
	* @param[in] graphicsPipeline	 RenderGraphicsPipeline to release
	*/
	void ReleaseGraphicsPipeline(RenderGraphicsPipeline* graphicsPipeline);

	/**************************************************************************
	*
	*  Command buffer handling
	*
	***************************************************************************/

	/**
	* @brief Allocte command buffers used for recordings
	*
	* @param[in] commandPool	 Command pool from where we allocate the buffers
	* @param[in] commandBufferInfo	Command buffer creation info
	* @param[out] commandBuffers	Array of RenderCommandBuffers where we store the pointers
	*/
	bool AllocateCommandBuffers(RenderCommandPool* commandPool
								, HalCommandBufferInfo& commandBufferInfo
								, caveVector<RenderCommandBuffer*>& commandBuffers);

	/**
	* @brief Relase command buffers 
	*
	* @param[in] commandBuffers		Array of RenderCommandBuffers we want to release
	*/
	void ReleaseCommandBuffers(caveVector<RenderCommandBuffer*>& commandBuffers);

	/**
	* @brief Begin command buffer recording
	*
	* @param[in] commandBuffer				RenderCommandBuffer we use for recording
	* @param[in] commandBufferBeginInfo		Command buffer begin setup
	*/
	void BeginCommandBuffer(RenderCommandBuffer* commandBuffer, HalCommandBufferBeginInfo& commandBufferBeginInfo);

	/**
	* @brief End command buffer recording
	*
	* @param[in] commandBuffer	RenderCommandBuffer we use for recording
	*/
	void EndCommandBuffer(RenderCommandBuffer* commandBuffer);

	/**
	* @brief Begin render pass command
	*
	* @param[in] commandBuffer			Command buffer we use for recording
	* @param[in] renderPassBeginInfo	Command info for render pass
	* @param[in] subpass				Subpass content in primary or secondary command buffers
	*/
	void CmdBeginRenderPass(RenderCommandBuffer* commandBuffer, RenderCmdRenderPassInfo& renderPassBeginInfo, HalSubpassContents subpass);

	/**
	* @brief End command buffer recording
	*
	* @param[in] commandBuffer	Command buffer we use for recording
	*/
	void CmdEndRenderPass(RenderCommandBuffer* commandBuffer);

	/**
	* @brief Bind graphics pipeline
	*
	* @param[in] commandBuffer			Command buffer we use for recording
	* @param[in] graphicsPipelineInfo	Graphics pipeline object
	*/
	void CmdBindGraphicsPipeline(RenderCommandBuffer* commandBuffer, RenderGraphicsPipeline* graphicsPipelineInfo);

	/**
	* @brief Bind vertex buffers to the command buffer
	*
	* @param[in] commandBuffer	Command buffer we use for recording
	* @param[in] firstBinding	First binding point
	* @param[in] bindingCount	Numbers of vertex buffers
	* @param[in] vertexBuffers	Array of RenderVertexBuffer
	* @param[in] offsetArray	Array of buffer offsets
	*/
	void CmdBindVertexBuffers(RenderCommandBuffer* commandBuffer, uint32_t firstBinding, uint32_t bindingCount
		, RenderVertexBuffer** vertexBuffers, const uint64_t* offsetArray);

	/**
	* @brief Bind index buffer to the command buffer
	*
	* @param[in] commandBuffer	Command buffer we use for recording
	* @param[in] indexBuffer	RenderIndexBuffer pointer
	* @param[in] offset			Offset into the buffer
	* @param[in] indexType		Index tpye like uint16
	*/
	void CmdBindIndexBuffer(RenderCommandBuffer* commandBuffer
		, RenderIndexBuffer* indexBuffer, const uint64_t offset, HalIndexType indexType);

	/**
	* @brief Binds descriptor sets to a command buffer
	*
	* @param[in] commandBuffer		Command buffer we use for recording
	* @param[in] pipelineBindPoint	Bound to graphics or compute pipeline
	* @param[in] layout				Layout is a RenderPipelineLayout object used to program the bindings
	* @param[in] firstSet			The set number of the first descriptor set to be bound.
	* @param[in] descriptorSetCount	The number of elements in the pDescriptorSets array.
	* @param[in] descriptorSets		Array of handles to RenderDescriptorSet objects describing the descriptor sets to write to.
	* @param[in] dynamicOffsetCount	The number of dynamic offsets in the pDynamicOffsets array.
	* @param[in] dynamicOffsets 	Pointer to an array of uint32_t values specifying dynamic offsets.
	*/
	void CmdBindDescriptorSets(RenderCommandBuffer* commandBuffer, HalPipelineBindPoints pipelineBindPoint
		, RenderPipelineLayout* layout, uint32_t firstSet, uint32_t descriptorSetCount
		, RenderDescriptorSet** descriptorSets, uint32_t dynamicOffsetCount, const uint32_t* dynamicOffsets);

	/**
	* @brief Update the values of push constants
	*
	* @param[in] commandBuffer		Command buffer we use for recording
	* @param[in] layout				Layout is a RenderPipelineLayout object used to program the bindings
	* @param[in] shaderStagesFlags	Bitmask of HalShaderStagesFlags specifying the shader stages that will use the push constants in the updated range.
	* @param[in] offset				The start offset of the push constant range to update, in units of bytes.
	* @param[in] size				The size of the push constant range to update, in units of bytes.
	* @param[in] pData				An array of size bytes containing the new push constant values.
	*/
	void CmdPushConstants(RenderCommandBuffer* commandBuffer, RenderPipelineLayout* layout
		, HalShaderStagesFlags shaderStagesFlags, uint32_t offset, uint32_t size, const void* pData);

	/**
	* @brief Draw command for non indexed drawing
	*
	* @param[in] commandBuffer	Command buffer we use for recording
	* @param[in] vertexCount	Number of vertices to draw
	* @param[in] instanceCount	Number of instances to draw
	* @param[in] firstVertex	Index of the first vertex to draw
	* @param[in] firstInstance	Instance ID of the first instance to draw
	*/
	void CmdDraw(RenderCommandBuffer* commandBuffer, uint32_t vertexCount, uint32_t instanceCount
		, uint32_t firstVertex, uint32_t firstInstance);

	/**
	* @brief Draw command for indexed drawing
	*
	* @param[in] commandBuffer	Command buffer we use for recording
	* @param[in] indexCount		Number of indices to draw
	* @param[in] instanceCount	Number of instances to draw
	* @param[in] firstIndex		Index of the first index to draw
	* @param[in] vertexOffset	Value added to the vertex index before indexing into the vertex buffer.
	* @param[in] firstInstance	Instance ID of the first instance to draw
	*/
	void CmdDrawIndexed(RenderCommandBuffer* commandBuffer, uint32_t indexCount, uint32_t instanceCount
		, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance);

	/**
	* @brief Submit scheduled copies
	* Note that this call just submits scheduled copies but we do not wait for it.
	*
	*/
	void FlushCopies();

	/**
	* @brief A special submit for swap chain image presentation
	*
	* @param[in] commandBuffer	Command buffer we use for recording
	*/
	bool PresentQueueSubmit(RenderCommandBuffer* commandBuffer);

	/**
	* @brief Present Image on screen
	*
	* @param[in] imageIndex		Image to present
	*
	* @return true if successful
	*/
	bool PresentQueue(uint32_t imageIndex);

	/**************************************************************************
	*
	*  Misc tools functions
	*
	***************************************************************************/
	
	/**
	* @brief Read pixels from the last used swap chain image
	*		 The returned pixels are always in RGBA format.
	*		 data must point to a memory which has sufficient space.
	*		 WIDTH * HEIGHT * 4 bytes
	*
	* @param[out] data	Pixel values are returned here
	*
	*/
	void ReadPixels(void* data);

private:
	RenderInstance* _pRenderInstance;	///< Pointer to the render instance we belong to
	HalInstance* _pHalInstance;	///< Pointer to HAL Instance
	HalRenderDevice* _pHalRenderDevice;	///< Pointer to HAL render device
	ResourceManager* _pResourceManager;	///< Our device resource manager
	SwapChainInfo _swapChainInfo;	///< swap chain info
};

}

/** @}*/
