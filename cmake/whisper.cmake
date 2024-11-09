
# Add whisper lib
FetchContent_Declare(whisper URL https://github.com/ggerganov/whisper.cpp/archive/refs/tags/v1.7.1.tar.gz)
FetchContent_MakeAvailable(whisper)

set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build shared libraries" FORCE)
set(WHISPER_CCACHE OFF CACHE BOOL "")
set(GGML_CCACHE OFF CACHE BOOL "")
set(WHISPER_BUILD_TESTS OFF CACHE BOOL "")
set(WHISPER_BUILD_EXAMPLES OFF CACHE BOOL "")
set(WHISPER_BUILD_SERVER OFF CACHE BOOL "")

if(APPLE)
    set(WHISPER_COREML ON CACHE BOOL "")
    set(WHISPER_COREML_ALLOW_FALLBACK ON CACHE BOOL "")
    set(GGML_METAL ON CACHE BOOL "")
endif()

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



# Whisper produce the shared libraries in lib
file(GLOB SHARED_LIBS
    "${CMAKE_BINARY_DIR}/lib/*.so"
    "${CMAKE_BINARY_DIR}/lib/*.dylib"
    "${CMAKE_BINARY_DIR}/lib/*.dll"
)

# Copy whisper libs to bin
add_custom_command(TARGET main POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/bin
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${SHARED_LIBS} ${CMAKE_BINARY_DIR}/bin
)