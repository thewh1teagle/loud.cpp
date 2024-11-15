
# Add whisper lib
FetchContent_Declare(whisper URL https://github.com/ggerganov/whisper.cpp/archive/f02b40bcb4849139fd7de2558aee64c737ab678e.zip)

set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build shared libraries" FORCE)
set(WHISPER_BUILD_TESTS OFF CACHE BOOL "")
set(WHISPER_BUILD_EXAMPLES OFF CACHE BOOL "")
set(WHISPER_BUILD_SERVER OFF CACHE BOOL "")

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    # Hide debug logs from Vulkan
    add_definitions(-DNDEBUG)
endif()

if(LOUD_SCCACHE)
    set(WHISPER_CCACHE ON CACHE BOOL "")
    set(GGML_CCACHE ON CACHE BOOL "")
endif()

if(APPLE)
    set(WHISPER_COREML OFF CACHE BOOL "")
    set(GGML_METAL_EMBED_LIBRARY ON CACHE BOOL "")
    set(GGML_METAL ON CACHE BOOL "")
endif()

FetchContent_MakeAvailable(whisper)

target_include_directories(main PUBLIC ${whisper_SOURCE_DIR}/include)
target_include_directories(main PUBLIC ${whisper_SOURCE_DIR}/ggml/include)

# Link whisper
if(APPLE)
    target_link_libraries(main PRIVATE
        whisper
        ggml
        "-framework CoreFoundation"
        "-framework Metal"
        "-framework CoreML"
    )
else()
    target_link_libraries(main PRIVATE
        whisper
        ggml
    )
endif()
