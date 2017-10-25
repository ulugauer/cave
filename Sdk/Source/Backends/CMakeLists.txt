# The Backend

# local pre-processor defines 
IF(WIN32)
	add_definitions(-D_CRT_SECURE_NO_WARNINGS)
ENDIF()

add_definitions(-DCAVE_EXPORTS) 

# Add sources
set(BACKENDS_SOURCE halInstance.h halInstance.cpp 
					halRenderDevice.h halRenderDevice.cpp )
# vulkan source
set(BACKENDS_VULKAN_SOURCE Vulkan/vulkanApi.h 
					 	   Vulkan/vulkanInstance.h 
					 	   Vulkan/vulkanInstance.cpp 
					 	   Vulkan/vulkanPhysicalDevice.h 
					 	   Vulkan/vulkanPhysicalDevice.cpp
					 	   Vulkan/vulkanRenderDevice.h 
					 	   Vulkan/vulkanRenderDevice.cpp )


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


# Creates folder "os" and adds target project (os.vcproj)
set_property(TARGET backends PROPERTY FOLDER "sdk")
