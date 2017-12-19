# The Input

# local pre-processor defines 
IF(WIN32)
	add_definitions(-D_CRT_SECURE_NO_WARNINGS)
ENDIF()

add_definitions(-DCAVE_EXPORTS) 

# Add sources
set(INPUT_ENGINE_SOURCE inputEngine.h inputEngine.cpp )


# Create named folders for the sources within the .vcproj
# Empty name lists them directly under the .vcproj
source_group("input" FILES ${INPUT_ENGINE_SOURCE})

#Generate the static library from the sources
add_library(input OBJECT ${INPUT_ENGINE_SOURCE})

# additional include directories
target_include_directories(input PRIVATE .)
target_include_directories(input PRIVATE ${PROJECT_SOURCE_DIR}/Sdk/Source/Engine)

# Creates folder "os" and adds target project (os.vcproj)
set_property(TARGET input PROPERTY FOLDER "sdk")
