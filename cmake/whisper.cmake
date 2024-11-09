
# Add whisper lib
FetchContent_Declare(whisper URL https://github.com/ggerganov/whisper.cpp/archive/refs/tags/v1.7.1.tar.gz)


set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build shared libraries" FORCE)
set(WHISPER_CCACHE OFF CACHE BOOL "")
set(GGML_CCACHE OFF CACHE BOOL "")
set(WHISPER_BUILD_TESTS OFF CACHE BOOL "")
set(WHISPER_BUILD_EXAMPLES OFF CACHE BOOL "")
set(WHISPER_BUILD_SERVER OFF CACHE BOOL "")
set(GGML_METAL_EMBED_LIBRARY ON CACHE BOOL "")

if(APPLE)
    set(WHISPER_COREML ON CACHE BOOL "")
    set(WHISPER_COREML_ALLOW_FALLBACK ON CACHE BOOL "")
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
