# The engine

# find pthread libs
IF(UNIX)
	find_package(Threads REQUIRED)
	find_package(X11 REQUIRED) 
ENDIF()

# find DX12 package
IF(WIN32)
	find_package(D3D12 REQUIRED)
ENDIF()


# local pre-processor defines 
IF(WIN32)
	add_definitions(-D_CRT_SECURE_NO_WARNINGS)
ENDIF()

add_definitions(-DCAVE_EXPORTS) 

# Add sources
set(ENGINE_SOURCE engineDefines.h 
				  engineTypes.h 
				  engineInstancePrivate.h 
				  engineInstancePrivate.cpp 
				  engineInstance.h 
				  engineInstance.cpp 
				  engineError.h 
				  engineError.cpp
				  engineLog.h 
				  engineLog.cpp )

set(RENDER_SOURCE Render/renderInstance.h Render/renderInstance.cpp 
				  Render/renderDevice.h Render/renderDevice.cpp 
				  Render/renderMaterial.h Render/renderMaterial.cpp 
				  Render/renderShader.h Render/renderShader.cpp 
				  Render/renderVertexInput.h Render/renderVertexInput.cpp 
				  Render/renderInputAssembly.h Render/renderInputAssembly.cpp 
				  Render/renderViewportScissor.h Render/renderViewportScissor.cpp
				  Render/renderLayerSection.h Render/renderLayerSection.cpp 
				  Render/renderRasterizerState.h Render/renderRasterizerState.cpp 
				  Render/renderMultisample.h Render/renderMultisample.cpp 
				  Render/renderDepthStencil.h Render/renderDepthStencil.cpp 
				  Render/renderColorBlend.h Render/renderColorBlend.cpp 
				  Render/renderDynamicState.h Render/renderDynamicState.cpp 
				  Render/renderPipelineLayout.h Render/renderPipelineLayout.cpp 
				  Render/renderRenderPass.h Render/renderRenderPass.cpp 
				  Render/renderGraphicsPipeline.h Render/renderGraphicsPipeline.cpp 
				  Render/renderCommandPool.h Render/renderCommandPool.cpp 
				  Render/renderCommandBuffer.h Render/renderCommandBuffer.cpp 
				  Render/renderDescriptorPool.h Render/renderDescriptorPool.cpp 
				  Render/renderDescriptorSet.h Render/renderDescriptorSet.cpp 
				  Render/renderSemaphore.h Render/renderSemaphore.cpp 
                  Render/renderFence.h Render/renderFence.cpp 
				  Render/renderBuffer.h Render/renderBuffer.cpp 
				  Render/renderVertexBuffer.h Render/renderVertexBuffer.cpp 
				  Render/renderIndexBuffer.h Render/renderIndexBuffer.cpp 
				  Render/renderUniformBuffer.h Render/renderUniformBuffer.cpp 
				  Render/renderTexture.h Render/renderTexture.cpp 
                  Render/renderTextureView.h Render/renderTextureView.cpp 
                  Render/renderTextureSampler.h Render/renderTextureSampler.cpp 
                  Render/renderRenderTarget.h Render/renderRenderTarget.cpp 
                  Render/renderFrameBuffer.h Render/renderFrameBuffer.cpp)

set(MEMORY_SOURCE Memory/allocatorBase.h 
				  Memory/allocatorGlobal.h
				  Memory/allocatorGlobal.cpp )

set(MATH_SOURCE Math/vector2.h
				Math/vector3.h
				Math/vector4.h 
				Math/matrix4.h )

set(COMMON_SOURCE Common/caveRefCount.h
				  Common/caveVector.h )

set(RESOURCE_SOURCE Resource/resourceManagerPrivate.h 
					Resource/resourceManagerPrivate.cpp
					Resource/resourceManager.h 
					Resource/resourceManager.cpp 
					Resource/materialResource.h 
					Resource/materialResource.cpp 
					Resource/imageResource.h 
					Resource/imageResource.cpp 
					Resource/imageResourceDds.h 
					Resource/imageResourceDds.cpp )

set(COMPONENT_SOURCE Components/componentBase.h Components/componentBase.cpp )

set(SCENE_SOURCE Scene/sceneNode.h Scene/sceneNode.cpp )

# Create named folders for the sources within the .vcproj
# Empty name lists them directly under the .vcproj
source_group("engine" FILES ${ENGINE_SOURCE})
source_group("engine\\components" FILES ${COMPONENT_SOURCE})
source_group("engine\\math" FILES ${MATH_SOURCE})
source_group("engine\\memory" FILES ${MEMORY_SOURCE})
source_group("engine\\render" FILES ${RENDER_SOURCE})
source_group("engine\\resource" FILES ${RESOURCE_SOURCE})
source_group("engine\\scene" FILES ${SCENE_SOURCE})

#Generate the shared library from the sources
add_library(cave SHARED ${ENGINE_SOURCE} ${RENDER_SOURCE} ${RESOURCE_SOURCE} 
						${COMPONENT_SOURCE} ${SCENE_SOURCE} ${MEMORY_SOURCE} ${MATH_SOURCE} 
			$<TARGET_OBJECTS:os> 
			$<TARGET_OBJECTS:backends> 
			$<TARGET_OBJECTS:frontends> 
			$<TARGET_OBJECTS:input> )

# additional include directories
target_include_directories(cave PRIVATE .)
target_include_directories(cave PRIVATE Render)
target_include_directories(cave PRIVATE Math)
target_include_directories(cave PRIVATE ${PROJECT_SOURCE_DIR}/Sdk/Source/Os)
target_include_directories(cave PRIVATE ${PROJECT_SOURCE_DIR}/Sdk/Source/Backends)
target_include_directories(cave PRIVATE ${PROJECT_SOURCE_DIR}/Sdk/Source/Frontends)
target_include_directories(cave PRIVATE ${PROJECT_SOURCE_DIR}/Sdk/Source/Input)
# 3rd party libs
target_include_directories(cave PRIVATE ${PROJECT_3RDPARTY_JSON_DIR})
target_include_directories(cave PRIVATE ${PROJECT_3RDPARTY_VULKANSDK_HEADER_DIR})

# On windows link DX libs
IF(WIN32)
	target_link_libraries (cave ${D3D12_LIBRARIES})
ENDIF()

# Creates folder "cave" and adds target project (cave.vcproj)
set_property(TARGET cave PROPERTY FOLDER "sdk")


IF(WIN32)
	set_target_properties(cave PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${CAVE_RUNTIME_LIB_DIR})
	set_target_properties(cave PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CAVE_RUNTIME_BIN_DIR})
ELSE()  
	set_target_properties(cave PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CAVE_RUNTIME_BIN_DIR})
ENDIF()

# copy files we need to output folder
#add_custom_command(TARGET cave POST_BUILD
#                   COMMAND ${CMAKE_COMMAND} -E copy_directory
#                   ${PROJECT_3RDPARTY_VULKANSDK_LIB_DIR} $<TARGET_FILE_DIR:cave>)

