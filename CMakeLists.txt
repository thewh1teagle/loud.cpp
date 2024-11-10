cmake_minimum_required(VERSION 3.15)
project(main)

set(CMAKE_CXX_STANDARD 17)

# Avoid warning about DOWNLOAD_EXTRACT_TIMESTAMP in CMake 3.24:
if (CMAKE_VERSION VERSION_GREATER_EQUAL "3.24.0")
    cmake_policy(SET CMP0135 NEW)
endif()

# Export compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Options
option(FFMPEG_DOWNLOAD "Download and set up FFmpeg" OFF)
option(SHERPA_STATIC "Link sherpa libs statically" OFF)

if(SHERPA_STATIC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

# Prepare executable
file(GLOB SRC_FILES "src/*.cpp")
add_executable(main ${SRC_FILES})
target_include_directories(main PRIVATE "${CMAKE_SOURCE_DIR}/include")
set_target_properties(main PROPERTIES OUTPUT_NAME "loud")
set_target_properties(main PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

# -DVERSION="$(git rev-parse --short HEAD)"
add_compile_definitions(VERSION="${VERSION}")

# Used in __FILE__ for relative path
string(LENGTH "${PROJECT_SOURCE_DIR}/" SOURCE_PATH_SIZE)
add_definitions("-DSOURCE_PATH_SIZE=${SOURCE_PATH_SIZE}")
add_definitions("-DSOURCE_PATH=\"${PROJECT_SOURCE_DIR}\"")

# Define output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# External libraries
include(FetchContent)
include(cmake/cli11.cmake)
include(cmake/json.cmake)
include(cmake/termcolor.cmake)
include(cmake/whisper.cmake)
include(cmake/subprocess.cmake)
include(cmake/curl.cmake)
if (SHERPA_STATIC)
    include(cmake/sherpa-static.cmake)
else()
    include(cmake/sherpa.cmake)
endif()
if (FFMPEG_DOWNLOAD)
    include(cmake/static-ffmpeg.cmake)
endif()

if(APPLE)
    # Add additional rpath
    add_custom_command(TARGET main
        POST_BUILD COMMAND 
        ${CMAKE_INSTALL_NAME_TOOL} -add_rpath "@executable_path"
        $<TARGET_FILE:main>
    )
endif()