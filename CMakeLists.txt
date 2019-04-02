# project setup
cmake_minimum_required(VERSION 3.4.0 FATAL_ERROR)

project(CaveEngine)

# version number
set(CAVE_ENGINE_MAJOR_VERSION 0)
set(CAVE_ENGINE_MINOR_VERSION 1)
set(CAVE_ENGINE_PATCH_VERSION 0)
set(CAVE_ENGINE_VERSION
  ${CAVE_ENGINE_MAJOR_VERSION}.${CAVE_ENGINE_MINOR_VERSION}.${CAVE_ENGINE_PATCH_VERSION}) 

#-------------------------------------------------------------------------------
# System packages
#-------------------------------------------------------------------------------
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/CMake)
include(SetBuildType)

#-------------------------------------------------------------------------------
# Basic configuration
#-------------------------------------------------------------------------------
include(ArchConfiguration)
include(CommonConfiguration)

set(CMAKE_C_FLAGS_PROFILE             "${CMAKE_C_FLAGS_RELEASE}"             CACHE STRING "Flags used by the C++ compiler."  FORCE)
set(CMAKE_CXX_FLAGS_PROFILE           "${CMAKE_CXX_FLAGS_RELEASE}"           CACHE STRING "Flags used by the C++ compiler."  FORCE)

# Turn on the ability to create folders to organize projects (.vcproj)
# It creates "CMakePredefinedTargets" folder by default and adds CMake
# defined projects like INSTALL.vcproj and ZERO_CHECK.vcproj 
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

#-------------------------------------------------------------------------------
# Install location
#-------------------------------------------------------------------------------
IF(WIN32)
	set(CAVE_RUNTIME_BIN_DIR ${PROJECT_SOURCE_DIR}/binary/win)
	set(CAVE_RUNTIME_LIB_DIR ${PROJECT_SOURCE_DIR}/libs/win)
ELSE()
	set(CAVE_RUNTIME_BIN_DIR ${PROJECT_SOURCE_DIR}/binary/linux)
	set(CAVE_RUNTIME_LIB_DIR ${PROJECT_SOURCE_DIR}/libs/linux)
ENDIF()

#-------------------------------------------------------------------------------
# 3rdparty Dependencies
#-------------------------------------------------------------------------------
set(PROJECT_3RDPARTY_DIR ${PROJECT_SOURCE_DIR}/3rdparty)
set(PROJECT_3RDPARTY_SHARED_DIR ${PROJECT_SOURCE_DIR}/3rdparty/Shared)
set(PROJECT_3RDPARTY_JSON_DIR ${PROJECT_3RDPARTY_SHARED_DIR}/Json)
set(PROJECT_3RDPARTY_PNG_HEADER_DIR ${PROJECT_3RDPARTY_DIR}/Png-1.6.21)
set(PROJECT_3RDPARTY_PNG_LIB_DIR ${PROJECT_3RDPARTY_DIR}/Png-1.6.21/${ARCH_DIR})
set(PROJECT_3RDPARTY_ZLIB_HEADER_DIR ${PROJECT_3RDPARTY_DIR}/Zlib-1.2.8)
set(PROJECT_3RDPARTY_ZLIB_LIB_DIR ${PROJECT_3RDPARTY_DIR}/Zlib-1.2.8/${ARCH_DIR})
set(PROJECT_3RDPARTY_VULKANSDK_HEADER_DIR ${PROJECT_3RDPARTY_DIR}/VulkanSdk-1.1.92.1)
set(PROJECT_3RDPARTY_VULKANSDK_LIB_DIR ${PROJECT_3RDPARTY_DIR}/VulkanSdk-1.1.92.1/${ARCH_DIR})

#-------------------------------------------------------------------------------
# Platform defines
#-------------------------------------------------------------------------------
IF(CMAKE_SYSTEM_NAME STREQUAL "Windows")
	add_definitions(-DVK_USE_PLATFORM_WIN32_KHR)
ELSEIF(CMAKE_SYSTEM_NAME STREQUAL "Linux")
	add_definitions(-DVK_USE_PLATFORM_XCB_KHR) 
ENDIF()

IF(MSVC)
	add_definitions(-DUNICODE -D_UNICODE)
ENDIF()

# add postfix for debug builds
SET(CMAKE_DEBUG_POSTFIX d)

#-------------------------------------------------------------------------------
# Common
#-------------------------------------------------------------------------------
add_subdirectory(Sdk)

#-------------------------------------------------------------------------------
# Tests
#-------------------------------------------------------------------------------
add_subdirectory(Tests)

#-------------------------------------------------------------------------------
# Applications
#-------------------------------------------------------------------------------


