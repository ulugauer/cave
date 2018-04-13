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

# disable warnings for test CAVE_SANITY_SOURCE
IF(UNIX)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-parameter")
ENDIF()

# librarie search path
link_directories(${CAVE_RUNTIME_LIB_DIR}) 
link_directories(${PROJECT_3RDPARTY_PNG_LIB_DIR})
link_directories(${PROJECT_3RDPARTY_ZLIB_LIB_DIR})

# shared libs
IF(UNIX)
link_directories(${CAVE_RUNTIME_BIN_DIR}) 
ENDIF()

# Add sources
set(CAVE_SANITY_COMMON_SOURCE	../Common/caveImageUtil.h
								../Common/caveImagePng.cpp 
								../Common/caveHtmlLog.h
								../Common/caveHtmlLog.cpp ) 

set(CAVE_SANITY_SOURCE  caveSanity.cpp
						caveSanityTestBase.h ) 

set(CAVE_SANITY_BASE_SOURCE  Base/caveSanityTestDevice.h Base/caveSanityTestDevice.cpp 
							 Base/caveSanityTestMappedVbo.h Base/caveSanityTestMappedVbo.cpp 
							 Base/caveSanityTestDeviceVbo.h Base/caveSanityTestDeviceVbo.cpp 
							 Base/caveSanityTestIndexBuffer.h Base/caveSanityTestIndexBuffer.cpp 
							 Base/caveSanityTestUniformBuffer.h Base/caveSanityTestUniformBuffer.cpp) 

# Create named folders for the sources within the .vcproj
# Empty name lists them directly under the .vcproj
source_group("sanity" FILES ${CAVE_SANITY_SOURCE})
source_group("sanity\\base" FILES ${CAVE_SANITY_BASE_SOURCE})
source_group("sanity\\common" FILES ${CAVE_SANITY_COMMON_SOURCE})

#Generate the executable from the sources
add_executable(CaveSanity ${CAVE_SANITY_SOURCE} ${CAVE_SANITY_COMMON_SOURCE} 
				${CAVE_SANITY_BASE_SOURCE})

# additional include directories
target_include_directories(CaveSanity PRIVATE .)
target_include_directories(CaveSanity PRIVATE ${PROJECT_SOURCE_DIR}/Sdk/Source/Engine)
target_include_directories(CaveSanity PRIVATE ${PROJECT_SOURCE_DIR}/Sdk/Source/Os)
target_include_directories(CaveSanity PRIVATE ${PROJECT_SOURCE_DIR}/Sdk/Source/Backends)
target_include_directories(CaveSanity PRIVATE ${PROJECT_SOURCE_DIR}/Sdk/Source/Frontends)
target_include_directories(CaveSanity PRIVATE ${PROJECT_SOURCE_DIR}/Sdk/Source/Input)

# 3rd party includes
target_include_directories(CaveSanity PRIVATE ${PROJECT_3RDPARTY_PNG_HEADER_DIR})
target_include_directories(CaveSanity PRIVATE ${PROJECT_3RDPARTY_VULKANSDK_HEADER_DIR})

# Set OS related libs
IF(UNIX)
	set(OS_LIBRARIES 
		xcb
		${X11_LIBRARIES} )
ELSE()
	set(OS_LIBRARIES "")
ENDIF()

# Properties->Linker->Input->Additional Dependencies
# For the GNU compiler link order matters
SET(LINK_LIBRARY optimized cave debug caved
				 optimized png debug pngd 
				 optimized zlib debug zlibd )

target_link_libraries (CaveSanity ${LINK_LIBRARY} 
								  ${CMAKE_DL_LIBS} ${CMAKE_THREAD_LIBS_INIT} 
								  ${OS_LIBRARIES} )
 

# Creates folder "cave" and adds target project (cave.vcproj)
set_property(TARGET CaveSanity PROPERTY FOLDER "tests")


IF(WIN32)
	set_target_properties(CaveSanity PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CAVE_RUNTIME_BIN_DIR})
ELSE()  
	set_target_properties(CaveSanity PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CAVE_RUNTIME_BIN_DIR})
ENDIF()
