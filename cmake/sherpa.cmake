if(WIN32)
    # Windows platform
    set(SHERPA_URL "https://github.com/k2-fsa/sherpa-onnx/releases/download/v1.10.30/sherpa-onnx-v1.10.30-win-x64-shared.tar.bz2")

elseif(UNIX AND NOT APPLE)
    # Linux platform
    set(SHERPA_URL "https://github.com/k2-fsa/sherpa-onnx/releases/download/v1.10.30/sherpa-onnx-v1.10.30-linux-x64-shared.tar.bz2")

elseif(APPLE)
    # macOS platform
    set(SHERPA_URL "https://github.com/k2-fsa/sherpa-onnx/releases/download/v1.10.30/sherpa-onnx-v1.10.30-osx-universal2-shared.tar.bz2")

else()
    message(FATAL_ERROR "Unsupported platform!")
endif()
FetchContent_Declare(
    sherpa
    URL ${SHERPA_URL}
)
FetchContent_MakeAvailable(sherpa)
target_include_directories(main PUBLIC ${sherpa_SOURCE_DIR}/include)

# Link sherpa
target_link_directories(main PRIVATE ${sherpa_SOURCE_DIR}/lib)
target_link_libraries(main PRIVATE
    sherpa-onnx-c-api
    cargs
    $<$<NOT:$<PLATFORM_ID:Windows>>:onnxruntime>
)
# Copy sherpa lib/ and bin/ to build/bin
file(GLOB SHARED_LIBS
    "${sherpa_SOURCE_DIR}/lib/*.so"
    "${sherpa_SOURCE_DIR}/lib/*.dylib"
    "${sherpa_SOURCE_DIR}/lib/*.dll"

    "${sherpa_SOURCE_DIR}/bin/*.so"
    "${sherpa_SOURCE_DIR}/bin/*.dylib"
    "${sherpa_SOURCE_DIR}/bin/*.dll"
)
add_custom_command(TARGET main POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:main>
    # Loop through each shared library and copy it to the target directory
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${SHARED_LIBS} $<TARGET_FILE_DIR:main>
)