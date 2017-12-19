# The Frontends

# local pre-processor defines 
IF(WIN32)
	add_definitions(-D_CRT_SECURE_NO_WARNINGS)
ENDIF()

add_definitions(-DCAVE_EXPORTS) 

# Add sources
set(FRONTEND_SOURCE frontend.h frontend.cpp )

IF(WIN32)
	list(APPEND FRONTEND_SOURCE windowFrontend.h windowFrontend.cpp)
ELSE()
	list(APPEND FRONTEND_SOURCE x11Frontend.h x11Frontend.cpp)
ENDIF()


# Create named folders for the sources within the .vcproj
# Empty name lists them directly under the .vcproj
source_group("frontends" FILES ${FRONTEND_SOURCE})

#Generate the static library from the sources
add_library(frontends OBJECT ${FRONTEND_SOURCE})

# additional include directories
target_include_directories(frontends PRIVATE .)
target_include_directories(frontends PRIVATE ${PROJECT_SOURCE_DIR}/Sdk/Source/Engine)
target_include_directories(frontends PRIVATE ${PROJECT_SOURCE_DIR}/Sdk/Source/Input)

# Creates folder "os" and adds target project (os.vcproj)
set_property(TARGET frontends PROPERTY FOLDER "sdk")
