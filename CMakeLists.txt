cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

# Include more modules
SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/CMakeModules")

# Mac: Make indigo compatible with OS X as early as 10.6
#      has to be set before project is declared
set(CMAKE_OSX_DEPLOYMENT_TARGET 10.6)

# Binary name
set(PROJECT_TARGETBIN "indigo")
# Human-readable project name
set(PROJECT_NAME "indigo")
set(PROJECT_DESCRIPTION "indigo is an abstract puzzle game.")
# Identifier of project
set(PROJECT_IDENTIFIER "tech.ulti.indigo")
# Versioning format: http://semver.org
set(PROJECT_MAJOR_VERSION 1)
set(PROJECT_MINOR_VERSION 0)
set(PROJECT_PATCH_VERSION 0)
set(PROJECT_COMPANY "ultitech")
string(TIMESTAMP YEAR "%Y")
set(PROJECT_COPYRIGHT "Copyright © 2006-${YEAR}, ${PROJECT_COMPANY}. All rights reserved.")

find_package(Git)
if(GIT_FOUND)
    execute_process(
            COMMAND ${GIT_EXECUTABLE} rev-parse --short HEAD
            OUTPUT_VARIABLE PROJECT_REVISION
            OUTPUT_STRIP_TRAILING_WHITESPACE)
endif()

# Define project name and language
project(${PROJECT_TARGETBIN} C)

# Add all source and header files
file(GLOB SRC
    "*.h"
    "*.c"
)

# Locate required libraries
Find_Package ( OpenGL REQUIRED )
Find_Package ( SDL2 REQUIRED )

# Include headers
include_directories(${SDL2_INCLUDE_DIR})

# Link all libraries
link_libraries (
  ${OPENGL_LIBRARIES}
  ${SDL2_LIBRARY}
)

if(WIN32)
  # Hide console on Windows
  SET(GUI_TYPE WIN32)

  add_definitions(/D _CRT_SECURE_NO_WARNINGS)

  #set(PRODUCT_ICON ${PROJECT_SOURCE_DIR}/Win/indigo.ico)

  set (_VersionInfoFile ${CMAKE_CURRENT_BINARY_DIR}/VersionInfo.h)
  set (_VersionResourceFile ${CMAKE_CURRENT_BINARY_DIR}/VersionResource.rc)

  configure_file(
        ${PROJECT_SOURCE_DIR}/Win/VersionInfo.in
        ${_VersionInfoFile}
        @ONLY)
  configure_file(
      ${PROJECT_SOURCE_DIR}/Win/VersionResource.rc
      ${_VersionResourceFile}
      COPYONLY)
  list(APPEND SRC ${SRC} ${_VersionInfoFile} ${_VersionResourceFile})

  set(WIN_DLLS ${SDL2_DLL})
ENDIF(WIN32)

if(APPLE)
  # Create a .app bundle
  SET(GUI_TYPE MACOSX_BUNDLE)

  # Get framework/library location to copy into bundle
  #! Cmake adds -framework Cocoa behind SDL framework for Mac
  string(REPLACE "-framework Cocoa" "" SDL2_FRAMEWORK ${SDL2_LIBRARY})

  # Copy framework/library into bundle
  set(FRAMEWORKS ${SDL2_FRAMEWORK})
  set_source_files_properties(${FRAMEWORKS} PROPERTIES MACOSX_PACKAGE_LOCATION Frameworks)

  # Copy resources into bundle
  set(RESOURCES ${PROJECT_SOURCE_DIR}/levels
                ${PROJECT_SOURCE_DIR}/Xcode/English.lproj
                ${PROJECT_SOURCE_DIR}/Xcode/de.lproj)

  set_source_files_properties(${RESOURCES} PROPERTIES MACOSX_PACKAGE_LOCATION Resources)

  # Set Info.plist values

  set(MACOSX_BUNDLE_EXECUTABLE_NAME ${PROJECT_TARGETBIN})
  set(MACOSX_BUNDLE_BUNDLE_NAME ${PROJECT_NAME})
  set(MACOSX_BUNDLE_GUI_IDENTIFIER ${PROJECT_IDENTIFIER})
  set(MACOSX_BUNDLE_BUNDLE_VERSION "${PROJECT_MAJOR_VERSION}.${PROJECT_MINOR_VERSION}.${PROJECT_PATCH_VERSION} rev. ${PROJECT_REVISION}")
  set(MACOSX_BUNDLE_SHORT_VERSION_STRING "${PROJECT_MAJOR_VERSION}.${PROJECT_MINOR_VERSION}.${PROJECT_PATCH_VERSION}")
  set(MACOSX_BUNDLE_INFO_STRING "${PROJECT_NAME} version ${PROJECT_MAJOR_VERSION}.${PROJECT_MINOR_VERSION}, Copyright © 2006-${YEAR} ${PROJECT_COMPANY}.")
  #set(MACOSX_BUNDLE_ICON_FILE "indigo.icns")
  set(MACOSX_BUNDLE_COPYRIGHT ${PROJECT_COPYRIGHT})
endif(APPLE)

if(UNIX AND NOT APPLE)
  link_libraries(-lm)
endif()

# Create Executable
add_executable(${PROJECT_TARGETBIN} ${GUI_TYPE} ${SRC} ${RESOURCES} ${FRAMEWORKS})

if(APPLE)
  # Set search paths to look for framework inside the bundle
  set_target_properties(${PROJECT_TARGETBIN} PROPERTIES XCODE_ATTRIBUTE_LD_RUNPATH_SEARCH_PATHS "@loader_path @loader_path/../Frameworks")
endif(APPLE)

if(UNIX AND NOT APPLE)
  set_target_properties(${PROJECT_TARGETBIN} PROPERTIES RUNTIME_OUTPUT_DIRECTORY indigo)
endif(UNIX AND NOT APPLE)

if(UNIX AND NOT APPLE OR WIN32)
  add_custom_command(TARGET ${PROJECT_TARGETBIN} POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                       ${PROJECT_SOURCE_DIR}/levels $<TARGET_FILE_DIR:${PROJECT_TARGETBIN}>)
endif(UNIX AND NOT APPLE OR WIN32)

if(WIN32)
 foreach(DLL ${WIN_DLLS})
 	add_custom_command(TARGET ${PROJECT_TARGETBIN} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${DLL} $<TARGET_FILE_DIR:${PROJECT_TARGETBIN}>)
 endforeach(DLL)
endif(WIN32)