# The engine

# find pthread libs
IF(UNIX)
	find_package(Threads REQUIRED)
ENDIF()


# local pre-processor defines 
IF(WIN32)
	add_definitions(-D_CRT_SECURE_NO_WARNINGS)
ENDIF()

add_definitions(-DCAVE_EXPORTS) 

# Add sources
set(ENGINE_HEADERS engineDefines.h 
				   engineTypes.h )

set(RENDER_SOURCE Render/renderInstance.h 
				  Render/renderInstance.cpp )

# Create named folders for the sources within the .vcproj
# Empty name lists them directly under the .vcproj
source_group("engine\\include" FILES ${ENGINE_HEADERS})
source_group("engine\\source" FILES ${ENGINE_SOURCE})
source_group("engine\\render" FILES ${RENDER_SOURCE})

#Generate the shared library from the sources
add_library(cave SHARED ${ENGINE_SOURCE} ${ENGINE_HEADERS} ${RENDER_SOURCE} 
			$<TARGET_OBJECTS:os> 
			$<TARGET_OBJECTS:backends> )

# additional include directories
target_include_directories(cave PRIVATE .)
target_include_directories(cave PRIVATE ${PROJECT_SOURCE_DIR}/Sdk/Source/Os)
target_include_directories(cave PRIVATE ${PROJECT_SOURCE_DIR}/Sdk/Source/Backends)

# Creates folder "cave" and adds target project (cave.vcproj)
set_property(TARGET cave PROPERTY FOLDER "sdk")


IF(WIN32)
	set_target_properties(cave PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${CAVE_RUNTIME_LIB_DIR})
	set_target_properties(cave PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CAVE_RUNTIME_BIN_DIR})
ELSE()  
	set_target_properties(cave PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CAVE_RUNTIME_BIN_DIR})
ENDIF()
