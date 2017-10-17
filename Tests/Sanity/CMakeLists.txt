# The engine

# find pthread libs
IF(UNIX)
	find_package(Threads REQUIRED)
ENDIF()


# local pre-processor defines 
IF(WIN32)
	add_definitions(-D_CRT_SECURE_NO_WARNINGS)
ENDIF()

# disable warnings for test CAVE_SANITY_SOURCE
IF(UNIX)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-parameter")
ENDIF()

# librarie search path
link_directories(${CAVE_RUNTIME_LIB_DIR}) 

# shared libs
IF(UNIX)
link_directories(${CAVE_RUNTIME_BIN_DIR}) 
ENDIF()

# Add sources
list(APPEND CAVE_SANITY_SOURCE caveSanity.cpp) 

# Create named folders for the sources within the .vcproj
# Empty name lists them directly under the .vcproj
source_group("sanity\\Source" FILES ${CAVE_SANITY_SOURCE})

#Generate the executable from the sources
add_executable(CaveSanity ${CAVE_SANITY_SOURCE})

# additional include directories
target_include_directories(CaveSanity PRIVATE .)
target_include_directories(CaveSanity PRIVATE ${PROJECT_SOURCE_DIR}/Sdk/Source/Engine)
target_include_directories(CaveSanity PRIVATE ${PROJECT_SOURCE_DIR}/Sdk/Source/Os)
target_include_directories(CaveSanity PRIVATE ${PROJECT_SOURCE_DIR}/Sdk/Source/Backends)

# Properties->Linker->Input->Additional Dependencies
# For the GNU compiler link order matters
# NVRender depends on NVRenderBackend and must therefore be namend first
SET(LINK_LIBRARY optimized cave debug caved)
target_link_libraries (CaveSanity ${LINK_LIBRARY} 
								  ${CMAKE_DL_LIBS} ${CMAKE_THREAD_LIBS_INIT} 
								  ${X11_LIBRARIES} ${XKBCOMMON_LIBRARIES} )
 

# Creates folder "cave" and adds target project (cave.vcproj)
set_property(TARGET CaveSanity PROPERTY FOLDER "tests")


IF(WIN32)
	set_target_properties(CaveSanity PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CAVE_RUNTIME_BIN_DIR})
ELSE()  
	set_target_properties(CaveSanity PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CAVE_RUNTIME_BIN_DIR})
ENDIF()
