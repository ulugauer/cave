# The Backend

# find DX12 package
IF(WIN32)
find_package(D3D12 REQUIRED)
ENDIF()

# local pre-processor defines 
IF(WIN32)
	add_definitions(-D_CRT_SECURE_NO_WARNINGS)
ENDIF()

add_definitions(-DCAVE_EXPORTS) 

# HAL sources
set(BACKENDS_SOURCE halTypes.h 
					halInstance.h halInstance.cpp 
					halRenderDevice.h halRenderDevice.cpp 
					halShader.h halShader.cpp 
					halVertexInput.h halVertexInput.cpp 
					halInputAssembly.h halInputAssembly.cpp 
					halViewportAndScissor.h halViewportAndScissor.cpp 
					halRasterizerState.h halRasterizerState.cpp 
					halMultisample.h halMultisample.cpp 
					halColorBlend.h halColorBlend.cpp 
					halDynamicState.h halDynamicState.cpp 
					halDepthStencil.h halDepthStencil.cpp 
					halPipelineLayout.h halPipelineLayout.cpp 
					halRenderPass.h halRenderPass.cpp 
					halGraphicsPipeline.h halGraphicsPipeline.cpp 
					halCommandPool.h halCommandPool.cpp 
					halCommandBuffer.h halCommandBuffer.cpp 
					halDescriptorPool.h halDescriptorPool.cpp 
					halDescriptorSet.h halDescriptorSet.cpp 
					halSemaphore.h halSemaphore.cpp 
                    halFence.h halFence.cpp
					halBuffer.h halBuffer.cpp 
					halImage.h halImage.cpp
                    halImageView.h halImageView.cpp 
                    halSampler.h halSampler.cpp 
                    halFrameBuffer.h halFrameBuffer.cpp )

# vulkan source
set(BACKENDS_VULKAN_SOURCE Vulkan/vulkanApi.h 
						   Vulkan/vulkanConversion.h Vulkan/vulkanConversion.cpp 
					 	   Vulkan/vulkanInstance.h Vulkan/vulkanInstance.cpp 
					 	   Vulkan/vulkanPhysicalDevice.h Vulkan/vulkanPhysicalDevice.cpp
					 	   Vulkan/vulkanRenderDevice.h Vulkan/vulkanRenderDevice.cpp 
						   Vulkan/vulkanMemoryManager.h Vulkan/vulkanMemoryManager.cpp
					 	   Vulkan/vulkanSwapChain.h Vulkan/vulkanSwapChain.cpp 
						   Vulkan/vulkanShader.h Vulkan/vulkanShader.cpp 
						   Vulkan/vulkanVertexInput.h Vulkan/vulkanVertexInput.cpp 
						   Vulkan/vulkanInputAssembly.h Vulkan/vulkanInputAssembly.cpp 
						   Vulkan/vulkanViewportAndScissor.h Vulkan/vulkanViewportAndScissor.cpp 
						   Vulkan/vulkanRasterizerState.h Vulkan/vulkanRasterizerState.cpp 
						   Vulkan/vulkanMultisample.h Vulkan/vulkanMultisample.cpp 
						   Vulkan/vulkanColorBlend.h Vulkan/vulkanColorBlend.cpp 
						   Vulkan/vulkanDynamicState.h Vulkan/vulkanDynamicState.cpp 
						   Vulkan/vulkanDepthStencil.h Vulkan/vulkanDepthStencil.cpp 
						   Vulkan/vulkanPipelineLayout.h Vulkan/vulkanPipelineLayout.cpp 
						   Vulkan/vulkanRenderPass.h Vulkan/vulkanRenderPass.cpp 
						   Vulkan/vulkanGraphicsPipeline.h Vulkan/vulkanGraphicsPipeline.cpp 
						   Vulkan/vulkanCommandPool.h Vulkan/vulkanCommandPool.cpp 
						   Vulkan/vulkanCommandBuffer.h Vulkan/vulkanCommandBuffer.cpp 
						   Vulkan/vulkanDescriptorPool.h Vulkan/vulkanDescriptorPool.cpp 
						   Vulkan/vulkanDescriptorSet.h Vulkan/vulkanDescriptorSet.cpp 
						   Vulkan/vulkanSemaphore.h Vulkan/vulkanSemaphore.cpp 
                           Vulkan/vulkanFence.h Vulkan/vulkanFence.cpp 
						   Vulkan/vulkanBuffer.h Vulkan/vulkanBuffer.cpp 
						   Vulkan/vulkanImage.h Vulkan/vulkanImage.cpp 
                           Vulkan/vulkanImageView.h Vulkan/vulkanImageView.cpp 
                           Vulkan/vulkanSampler.h Vulkan/vulkanSampler.cpp 
                           Vulkan/vulkanFrameBuffer.h Vulkan/vulkanFrameBuffer.cpp )


# Create named folders for the sources within the .vcproj
# Empty name lists them directly under the .vcproj
source_group("backends" FILES ${BACKENDS_SOURCE})
source_group("backends\\vulkan" FILES ${BACKENDS_VULKAN_SOURCE})

#Generate the static library from the sources
add_library(backends OBJECT ${BACKENDS_SOURCE} ${BACKENDS_VULKAN_SOURCE})

# additional include directories
target_include_directories(backends PRIVATE . )
target_include_directories(backends PRIVATE Vulkan )
target_include_directories(backends PRIVATE ${PROJECT_SOURCE_DIR}/Sdk/Source/Engine)
target_include_directories(backends PRIVATE ${PROJECT_SOURCE_DIR}/Sdk/Source/Os)

# 3rd party includes
target_include_directories(backends PRIVATE ${PROJECT_3RDPARTY_VULKANSDK_HEADER_DIR})

# Creates folder "os" and adds target project (os.vcproj)
set_property(TARGET backends PROPERTY FOLDER "sdk")
