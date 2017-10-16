# The Backend

# local pre-processor defines 
IF(WIN32)
	add_definitions(-D_CRT_SECURE_NO_WARNINGS)
ENDIF()

# Add sources
set(OS_INCLUDE osPlatformLib.h )

IF(WIN32)
	list(APPEND OS_SOURCE osPlatformLibWin.cpp)
ELSE()
	list(APPEND OS_SOURCE osPlatformLibLinux.cpp)
ENDIF()


# Create named folders for the sources within the .vcproj
# Empty name lists them directly under the .vcproj
source_group("os\\Source" FILES ${OS_SOURCE})
source_group("os\\Include" FILES ${OS_INCLUDE})

#Generate the static library from the sources
add_library(os OBJECT ${OS_SOURCE} ${OS_INCLUDE})

# additional include directories
target_include_directories(os PRIVATE include)


# Creates folder "os" and adds target project (os.vcproj)
set_property(TARGET os PROPERTY FOLDER "sdk")
