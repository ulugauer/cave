# The engine

# find pthread libs
IF(UNIX)
	find_package(Threads REQUIRED)
	find_package(X11 REQUIRED) 
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
				  Render/renderDepthStencil.h Render/renderDepthStencil.cpp )

set(MEMORY_SOURCE Memory/allocatorBase.h 
				  Memory/allocatorGlobal.h
				  Memory/allocatorGlobal.cpp )

set(MATH_SOURCE Math/vector2.h
				Math/vector3.h
				Math/vector4.h )

set(COMMON_SOURCE Common/caveRefCount.h
				  Common/caveVector.h )

set(RESOURCE_SOURCE Resource/resourceManagerPrivate.h 
					Resource/resourceManagerPrivate.cpp
					Resource/resourceManager.h 
					Resource/resourceManager.cpp 
					Resource/materialResource.h 
					Resource/materialResource.cpp )

# Create named folders for the sources within the .vcproj
# Empty name lists them directly under the .vcproj
source_group("engine" FILES ${ENGINE_SOURCE})
source_group("engine\\math" FILES ${MATH_SOURCE})
source_group("engine\\memory" FILES ${MEMORY_SOURCE})
source_group("engine\\render" FILES ${RENDER_SOURCE})
source_group("engine\\resource" FILES ${RESOURCE_SOURCE})

#Generate the shared library from the sources
add_library(cave SHARED ${ENGINE_SOURCE} ${RENDER_SOURCE} ${RESOURCE_SOURCE} 
						${MEMORY_SOURCE} ${MATH_SOURCE} 
			$<TARGET_OBJECTS:os> 
			$<TARGET_OBJECTS:backends> 
			$<TARGET_OBJECTS:frontends> )

# additional include directories
target_include_directories(cave PRIVATE .)
target_include_directories(cave PRIVATE Render)
target_include_directories(cave PRIVATE Math)
target_include_directories(cave PRIVATE ${PROJECT_SOURCE_DIR}/Sdk/Source/Os)
target_include_directories(cave PRIVATE ${PROJECT_SOURCE_DIR}/Sdk/Source/Backends)
target_include_directories(cave PRIVATE ${PROJECT_SOURCE_DIR}/Sdk/Source/Frontends)
# 3rd party libs
target_include_directories(cave PRIVATE ${PROJECT_3RDPARTY_JSON_DIR})

# Creates folder "cave" and adds target project (cave.vcproj)
set_property(TARGET cave PROPERTY FOLDER "sdk")


IF(WIN32)
	set_target_properties(cave PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${CAVE_RUNTIME_LIB_DIR})
	set_target_properties(cave PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CAVE_RUNTIME_BIN_DIR})
ELSE()  
	set_target_properties(cave PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CAVE_RUNTIME_BIN_DIR})
ENDIF()
