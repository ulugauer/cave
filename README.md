\mainpage
# cave
A multi-purpose render engine

A fully programmable render engine for any kind of visualization.
Initially supported backend is Vulkan.

# Prerequisites

    # Upgrade to CMake 3.4+; https://cmake.org/download/

    # For Windows :
    # Install Visual Studio 2015 / 17

    # For Linux (Ubuntu 16.04) :
    Minimum requirement gcc 4.9


# Building from Command Line

First:

    mkdir build
    cd build


## Windows :

	start cmake .. -G "Visual Studio 14 2015 Win64" 

## Linux (Ubuntu 16.04) :

    cmake .. -DCMAKE_BUILD_TYPE=Release	

# Sanity

To run CAVE engine sanity navigate to [projctRoot]/binary/[platform]  
Run CaveSanity -r ../../Tests/Sanity for low level engine tests.  
Run CaveSanity -h for more options.
