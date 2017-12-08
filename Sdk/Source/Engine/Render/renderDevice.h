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
	* @brief Create a swap chain
	*
	*/
	void CreateSwapChain();

	/**
	* @brief Get swap chain image format
	*
	* @return Image format
	*/
	const HalImageFormat GetSwapChainImageFormat();

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
	* @brief Create a vertex input object
	*
	* @return Vertex input object
	*/
	RenderVertexInput* CreateVertexInput();

	/**
	* @brief Release a vertex input object
	*
	* @param[in] vertexInput	Vertex input object to release
	*/
	void ReleaseVertexInput(RenderVertexInput* vertexInput);

	/**
	* @brief Create an input assembly object
	*
	* @return Input assembly object
	*/
	RenderInputAssembly* CreateInputAssembly();

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
	* @param[in] descriptorSetLayouts	Pipeline set layouts array
	* @param[in] pushConstants			Pipeline push constant ranges array
	*
	* @return RenderPipelineLayout object
	*/
	RenderPipelineLayout* CreatePipelineLayout(caveVector<HalDescriptorSetLayout>& descriptorSetLayouts
		, caveVector<HalPushConstantRange>& pushConstants);

	/**
	* @brief Release a pipeline layout object
	*
	* @param[in] pipelineLayout	 RenderColorBlend object to release
	*/
	void ReleasePipelineLayout(RenderPipelineLayout* pipelineLayout);

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
	* @brief Release a graphics pipeline object
	*
	* @param[in] graphicsPipeline	 Graphics pipeline object to release
	*/
	void ReleaseGraphicsPipeline(RenderGraphicsPipeline* graphicsPipeline);

private:
	RenderInstance* _pRenderInstance;	///< Pointer to the render instance we belong to
	HalInstance* _pHalInstance;	///< Pointer to HAL Instance
	HalRenderDevice* _pHalRenderDevice;	///< Pointer to HAL render device
	ResourceManager* _pResourceManager;	///< Our device resource manager
	SwapChainInfo _swapChainInfo;	///< swap chain info
};

}

/** @}*/
