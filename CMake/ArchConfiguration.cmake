# Copyright (c) 2016, NVIDIA CORPORATION.  All rights reserved.

#-------------------------------------------------------------------------------
# Platform selection
#-------------------------------------------------------------------------------


#-------------------------------------------------------------------------------
# Architecture selection
#-------------------------------------------------------------------------------
if(MSVC)
    # Windows
    if(CMAKE_CL_64)
        set(ARCH_DIR "Win64")
    else()
        set(ARCH_DIR "Win32")
        message(FATAL_ERROR "Only Win64 builds are supported. \
                Please make sure that you build the project with 64 bit compiler.")
    endif()
    if(MSVC_VERSION EQUAL 1800)
        set(ARCH_DIR "${ARCH_DIR}-vc12")
    elseif(MSVC_VERSION EQUAL 1900)
        set(ARCH_DIR "${ARCH_DIR}-vc14")
	elseif(NOT (MSVC_VERSION LESS 1910))
        set(ARCH_DIR "${ARCH_DIR}-vc15")
    else()
        message(FATAL_ERROR "Unsupported VisualStudio version.")
    endif()
else()
    # Linux (either VIBRANTE or PC)
    if(RUNTIME_TARGET_ARM)
        # Select device architecture
        if(CMAKE_SIZEOF_VOID_P EQUAL 8)
            set(ARCH_DIR "Linux-aarch64")
        else()
            smessage(FATAL_ERROR "Only 64 bit builds are supported. \
                     Please make sure that you build the project with 64 bit compiler.")
        endif()
    else()
        set(ARCH_DIR "Linux")
    endif()
endif()
unset(SDK_ARCH_DIR CACHE)
set(SDK_ARCH_DIR ${ARCH_DIR} CACHE INTERNAL "")
