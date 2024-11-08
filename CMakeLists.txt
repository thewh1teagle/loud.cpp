cmake_minimum_required(VERSION 3.10)
project(main)

set(CMAKE_CXX_STANDARD 17)

# Avoid warning about DOWNLOAD_EXTRACT_TIMESTAMP in CMake 3.24:
if (CMAKE_VERSION VERSION_GREATER_EQUAL "3.24.0")
    cmake_policy(SET CMP0135 NEW)
endif()

# Export compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Prepare executable
file(GLOB SRC_FILES "src/*.cpp")
add_executable(main ${SRC_FILES})
target_include_directories(main PRIVATE "${CMAKE_SOURCE_DIR}/include")
set_target_properties(main PROPERTIES OUTPUT_NAME "loud")
set_target_properties(main PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

# -DCOMMIT_HASH="$(git rev-parse --short HEAD)"
add_compile_definitions(COMMIT_HASH="${COMMIT_HASH}")

# Used in __FILE__ for relative path
string(LENGTH "${PROJECT_SOURCE_DIR}/" SOURCE_PATH_SIZE)
add_definitions("-DSOURCE_PATH_SIZE=${SOURCE_PATH_SIZE}")
add_definitions("-DSOURCE_PATH=\"${PROJECT_SOURCE_DIR}\"")

# Define output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Options

option(FFMPEG_DOWNLOAD "Download and set up FFmpeg" OFF)

# External libraries
include(FetchContent)
include(CMake/cli11.cmake)
include(CMake/json.cmake)
include(CMake/sherpa.cmake)
include(CMake/whisper.cmake)
if (FFMPEG_DOWNLOAD)
    include(Cmake/static-ffmpeg.cmake)
endif()
