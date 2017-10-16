# The Backend

# local pre-processor defines 
IF(WIN32)
	add_definitions(-D_CRT_SECURE_NO_WARNINGS)
ENDIF()

add_definitions(-DCAVE_EXPORTS) 

# Add sources
set(BACKENDS_INCLUDE halInstance.h )
set(BACKENDS_SOURCE halInstance.cpp )



# Create named folders for the sources within the .vcproj
# Empty name lists them directly under the .vcproj
source_group("backends" FILES ${BACKENDS_SOURCE})
source_group("backends" FILES ${BACKENDS_INCLUDE})

#Generate the static library from the sources
add_library(backends OBJECT ${BACKENDS_SOURCE} ${BACKENDS_INCLUDE})

# additional include directories
target_include_directories(backends PRIVATE ${PROJECT_SOURCE_DIR}/Sdk/Source/Engine)
target_include_directories(backends PRIVATE ${PROJECT_SOURCE_DIR}/Sdk/Source/Os)


# Creates folder "os" and adds target project (os.vcproj)
set_property(TARGET backends PROPERTY FOLDER "sdk")
