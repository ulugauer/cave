\mainpage
# cave
A multi-purpose render engine

A fully programmable render engine for any kind of visualization.
Initially supported backend is Vulkan.

# Prerequisites

    # Upgrade to CMake 3.4+; https://cmake.org/download/

    # For Windows :
    # Install Visual Studio 2015

    # For Linux (Ubuntu 16.04) :
    Install gcc 4.9


# Building from Command Line

First:

    mkdir build
    cd build


## Windows :

	start cmake ../.. -G "Visual Studio 14 2015 Win64" 

## Linux (Ubuntu 16.04) :

    cmake .. -DCMAKE_BUILD_TYPE=Release	
