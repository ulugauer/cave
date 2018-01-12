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

/// @file vulkanRenderDevice.h
///       Vulkan render device

#include "halRenderDevice.h"
#include "osPlatformLib.h"
#include "Common/caveVector.h"

#include "vulkan.h"

/** \addtogroup backend 
*  @{
*		
*/

namespace cave
{

///< forwards
class VulkanInstance;
class VulkanPhysicalDevice;
class VulkanSwapChain;
class VulkanMemoryManager;

/**
* Vulkan render device
*/
class VulkanRenderDevice : public HalRenderDevice
{
public:
	/**
	* @brief Constructor
	*
	* @param[in] instance	Pointer to instance object
	* @param[in] physicalDevice	Pointer to physical device
	* @param[in] surface	Handle to presentation surface [optional]
	*
	*/
	VulkanRenderDevice(VulkanInstance* instance, VulkanPhysicalDevice* physicalDevice, VkSurfaceKHR surface);

	/** @brief Destructor */
	virtual ~VulkanRenderDevice();

	/**
	* @brief Get vulkan logical device handle
	*
	* @return Lowlevel vulkan handle
	*/
	VkDevice GetDeviceHandle() { return _vkDevice; }

	/**
	* @brief Get vulkan memory manager
	*
	* @return Device memory manager object
	*/
	VulkanMemoryManager* GetMemoryManager() { return _pMemoryManager; }

	/**
	* @brief Query physical device memory proeprties
	*
	* @param[in,out] deviceMemProperties	Pointer to VkPhysicalDeviceMemoryProperties
	*/
	void GetPhysicalDeviceMemoryProperties(VkPhysicalDeviceMemoryProperties& deviceMemProperties);

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
	void WaitIdle() override;

	/**
	* @brief Get vulkan surface handle
	*
	* @return Lowlevel vulkan handle
	*/
	VkSurfaceKHR GetPresentationSurfaceHandle() { return _presentationSurface; }

	/**
	* @brief Create a vulkan swap chain
	*
	* @param[in] swapChainInfo	Swap chain creation info
	*
	*/
	void CreateSwapChain(SwapChainInfo& swapChainInfo) override;

	/**
	* @brief Create a low level command pool object
	*
	* @param[in] commandPoolInfo	Command pool creation info
	*
	* @return HalCommandPool abstraction interface
	*/
	HalCommandPool* CreateCommandPool(HalCommandPoolInfo& commandPoolInfo) override;

	/**
	* @brief Create a low level shader
	*
	* @param[in] type		Shader type
	* @param[in] language	Shader language
	*
	* @return shader interface
	*/
	HalShader* CreateShader(ShaderType type, ShaderLanguage language) override;

	/**
	* @brief Create a low level vertex input state
	*
	* @param[in] vertexInputState	Vertex input setup struct
	*
	* @return Vertex input interface
	*/
	HalVertexInput* CreateVertexInput(HalVertexInputStateInfo& vertexInputState) override;

	/**
	* @brief Create a low level input assembly state
	*
	* @return Input assembly interface
	*/
	HalInputAssembly* CreateInputAssembly() override;

	/**
	* @brief Create a low level viewport, scissor state
	*
	* @param[in] viewport	Viewport setup
	* @param[in] scissor	Scissor setup
	*
	* @return  Viewport, scissor abstraction interface
	*/
	HalViewportAndScissor* CreateViewportAndScissor(HalViewport& viewport, HalScissor& scissor) override;

	/**
	* @brief Create a low level rasterizer state
	*
	* @param[in] rasterizerStateInfo	Rasterizer setup
	*
	* @return  rasterizer abstraction interface
	*/
	HalRasterizerState* CreateRasterizerState(HalRasterizerSetup& rasterizerStateInfo) override;

	/**
	* @brief Create a low level multisample state
	*
	* @param[in] multisampleStateInfo	Multisample setup info
	*
	* @return  Multisample abstraction interface
	*/
	HalMultisample* CreateMultisampleState(HalMultisampleState& multisampleStateInfo) override;

	/**
	* @brief Create a low level depth stencil state
	*
	* @param[in] depthStencilInfo	Depth Stencil setup info
	*
	* @return  DepthStencil abstraction interface
	*/
	HalDepthStencil* CreateDepthStencilState(HalDepthStencilSetup& depthStencilInfo) override;

	/**
	* @brief Create a low level depth stencil state
	*
	* @param[in] colorBlendInfo		Color blending state data
	* @param[in] blendAttachments	Color blend attachment state
	*
	* @return  DepthStencil abstraction interface
	*/
	HalColorBlend* CreateColorBlendState(HalColorBlendState& colorBlendInfo
		, caveVector<HalColorBlendAttachment>& blendAttachments) override;

	/**
	* @brief Create a low level dynamic state
	*
	* @param[in] dynamicStates		Dynamic states array
	*
	* @return  HalDynamicState abstraction interface
	*/
	HalDynamicState* CreateDynamicState(caveVector<HalDynamicStates>& dynamicStates) override;

	/**
	* @brief Create a pipeline layout object
	*
	* @param[in] descriptorSetLayouts	Pipeline set layouts array
	* @param[in] pushConstants			Pipeline push constant ranges array
	*
	* @return  HalPipelineLayout abstraction interface
	*/
	HalPipelineLayout* CreatePipelineLayout(caveVector<HalDescriptorSetLayout>& descriptorSetLayouts
		, caveVector<HalPushConstantRange>& pushConstants) override;

	/**
	* @brief Create a low level render pass object
	*
	* @param[in] renderPassInfo	Render pass setup info
	*
	* @return  HalRenderPass abstraction interface
	*/
	HalRenderPass* CreateRenderPass(HalRenderPassInfo& renderPassInfo) override;

	/**
	* @brief Create a low level graphics pipeline object
	*
	* @param[in] graphicsPipelineInfo	Graphics pipeline setup struct
	*
	* @return  HalGraphicsPipeline abstraction interface
	*/
	HalGraphicsPipeline* CreateGraphicsPipeline(HalGraphicsPipelineInfo& graphicsPipelineInfo) override;

	/**
	* @brief Create semaphore object
	*
	* @return  HalSemaphore abstraction interface
	*/
	HalSemaphore* CreateSemaphore() override;

	/**
	* @brief Create a low level buffer object
	*
	* @param[in] bufferInfo		Buffer create info
	*
	* @return  HalBuffer abstraction interface
	*/
	HalBuffer* CreateBuffer(HalBufferInfo& bufferInfo) override;

	/**
	* @brief Allocate command buffers
	*
	* @param[in] commandPool		Command pool object
	* @param[in] commandBufferInfo	Command buffer creation info
	* @param[out] commandBuffers	Allocated command buffers
	*
	* @return  true on succes
	*/
	bool AllocateCommandBuffers(HalCommandPool* commandPool
		, HalCommandBufferInfo& commandBufferInfo
		, caveVector<HalCommandBuffer*>& commandBuffers) override;

	/**
	* @brief Begin command buffer recording
	*
	* @param[in] commandBuffer				Command buffer we use for recording
	* @param[in] commandBufferBeginInfo		Command buffer begin setup
	*/
	void BeginCommandBuffer(HalCommandBuffer* commandBuffer, HalCommandBufferBeginInfo& commandBufferBeginInfo) override;

	/**
	* @brief End command buffer recording
	*
	* @param[in] commandBuffer	Command buffer we use for recording
	*/
	void EndCommandBuffer(HalCommandBuffer* commandBuffer) override;

	/**
	* @brief Begin render pass command
	*
	* @param[in] commandBuffer			Command buffer we use for recording
	* @param[in] RenderPassBeginInfo	Command info for render pass
	* @param[in] subpass				Subpass content in primary or secondary command buffers
	*/
	void CmdBeginRenderPass(HalCommandBuffer* commandBuffer, HalCmdRenderPassInfo& RenderPassBeginInfo, HalSubpassContents subpass) override;

	/**
	* @brief End command buffer recording
	*
	* @param[in] commandBuffer	Command buffer we use for recording
	*/
	void CmdEndRenderPass(HalCommandBuffer* commandBuffer) override;

	/**
	* @brief Bind graphics pipeline
	*
	* @param[in] commandBuffer			Command buffer we use for recording
	* @param[in] graphicsPipelineInfo	Graphics pipeline object
	*/
	void CmdBindGraphicsPipeline(HalCommandBuffer* commandBuffer, HalGraphicsPipeline* graphicsPipelineInfo) override;

	/**
	* @brief Bind vertex buffers to the pipeline
	*
	* @param[in] commandBuffer	Command buffer we use for recording
	* @param[in] firstBinding	First binding point
	* @param[in] bindingCount	Numbers of vertex buffers
	* @param[in] vertexBuffers	Array of HalBuffer pointers
	* @param[in] offsetArray	Array of buffer offsets
	*/
	void CmdBindVertexBuffers(HalCommandBuffer* commandBuffer, uint32_t firstBinding, uint32_t bindingCount
		, HalBuffer** vertexBuffers, const uint64_t* offsetArray) override;

	/**
	* @brief Draw command for non indexed drawing
	*
	* @param[in] commandBuffer	Command buffer we use for recording
	* @param[in] vertexCount	Number of vertices to draw
	* @param[in] instanceCount	Number of instances to draw
	* @param[in] firstVertex	Index of the first vertex to draw
	* @param[in] firstInstance	Instance ID of the first instance to draw
	*/
	void CmdDraw(HalCommandBuffer* commandBuffer, uint32_t vertexCount, uint32_t instanceCount
		, uint32_t firstVertex, uint32_t firstInstance) override;

	/**
	* @brief A special submit for swap chain image presentation
	*
	* @param[in] commandBuffer			Command buffer we use for recording
	*/
	bool PresentQueueSubmit(HalCommandBuffer* commandBuffer) override;

	/**
	* @brief Present Image on scren
	*
	* @param[in] imageIndex		Image to present
	*
	* @return true if successful
	*/
	bool PresentQueue(uint32_t imageIndex) override;

	/**
	* @brief Get swap chain image format
	*
	* @return Image format
	*/
	const HalImageFormat GetSwapChainImageFormat() override;

	/**
	* @brief Get swap chain image count
	*
	* @return Image count
	*/
	const uint32_t GetSwapChainImageCount() override;

	/**
	* @brief Get swap chain image extend
	*
	* @return Image extend
	*/
	const HalExtent2D GetSwapChainExtend() override;

	/**
	* @brief Get next available swap chain image
	*
	* @param[in] timeout		Timeout in nano seconds
	*
	* @return Image index
	*/
	const uint32_t AcquireNextSwapChainImage(uint64_t timeout) override;

	/**
	* @brief Create presentation framebuffers
	*		 This functions creates as much framebuffers as swap chain images.
	*		 This functions throws an exception if soemthing goes wrong
	*
	* @param[in] renderPass		RenderPass object which we need to attach to the framebuffers
	*
	*/
	void CreateSwapChainFramebuffers(HalRenderPass* renderPass) override;

	/**
	* @brief Get graphics familiy index
	*
	* @return graphics familiy index
	*/
	const uint32_t GetGraphicsFamilyIndex() const {
		return _graphicsQueueFamilyIndex;
	}

	/**
	* @brief Get presentation familiy index
	*
	* @return presentation familiy index
	*/
	const uint32_t GetPresentationFamilyIndex() const {
		return _presentationQueueFamilyIndex;
	}

	/**
	* @brief Get graphicsQueue familiy index
	*
	* @return VkQueue handle
	*/
	const VkQueue GetGraphicsQueue() const {
		return _graphicsQueue;
	}

private:
	VulkanInstance* _pInstance;	///< Pointer to instance object
	VulkanPhysicalDevice* _pPhysicalDevice;	///< Pointer to physical device
	VulkanMemoryManager* _pMemoryManager;	///< Pointer to internal memory manager
	VkSurfaceKHR _presentationSurface; ///< Handle to presentation surface
	VkDevice _vkDevice;	///< Handle to vulkan device
	VkQueue _presentQueue;	///< Handle to vulkan queue used for presentations
	VkQueue _graphicsQueue;	///< Handle to vulkan queue used for graphics
	VulkanSwapChain* _pSwapChain;	///< Handle to a swap chain
	VkCommandPool _presentQueueCommandPool;	///< Command pool used for presentations
	VkCommandBuffer* _presentCommandBufferArray;	///< Presentation command buffer
	caveVector<VkFramebuffer> _presentationFramebuffers; ///< Array of framebuffers used for presentation
	uint32_t _presentationQueueFamilyIndex; ///< Index of present queue familiy
	uint32_t _graphicsQueueFamilyIndex; ///< Index of graphics queue familiy
};

}

/** @}*/
