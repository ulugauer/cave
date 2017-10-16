# Copyright (c) 2016, NVIDIA CORPORATION.  All rights reserved.

#-------------------------------------------------------------------------------
# Enable C++11
#-------------------------------------------------------------------------------
if(CMAKE_VERSION VERSION_GREATER 3.1)
    set(CMAKE_CXX_STANDARD 11)
else()
    if(LINUX)
        CHECK_CXX_COMPILER_FLAG(-std=c++11 COMPILER_SUPPORTS_CXX11)
        CHECK_CXX_COMPILER_FLAG(-std=c++0x COMPILER_SUPPORTS_CXX0X)
        if(COMPILER_SUPPORTS_CXX11)
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
        elseif(COMPILER_SUPPORTS_CXX0X)
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
        else()
            message(ERROR "Compiler ${CMAKE_CXX_COMPILER} has no C++11 support")
        endif()
    endif()
endif()

#-------------------------------------------------------------------------------
# Build flags
#-------------------------------------------------------------------------------

if(MSVC)
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4") 
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
elseif(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -Wunused -Wunused-value -Wunused-parameter")

endif()



