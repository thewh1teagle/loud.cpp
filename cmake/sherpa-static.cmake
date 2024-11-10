if(WIN32)
    # Windows platform
    set(SHERPA_URL "https://github.com/k2-fsa/sherpa-onnx/releases/download/v1.10.30/sherpa-onnx-v1.10.30-win-x64-static.tar.bz2")

elseif(UNIX AND NOT APPLE)
    # Linux platform
    set(SHERPA_URL "https://github.com/k2-fsa/sherpa-onnx/releases/download/v1.10.30/sherpa-onnx-v1.10.30-linux-x64-static.tar.bz2")

elseif(APPLE)
    # macOS platform
    set(SHERPA_URL "https://github.com/k2-fsa/sherpa-onnx/releases/download/v1.10.30/sherpa-onnx-v1.10.30-osx-universal2-static.tar.bz2")

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


if (_WIN32)
    # It doesn't support on static build
    set(SHERPA_ONNX_ENABLE_TTS OFF)
endif()

target_link_libraries(main PRIVATE 
    cargs
    espeak-ng
    kaldi-decoder-core
    kaldi-native-fbank-core
    onnxruntime
    piper_phonemize
    sherpa-onnx-c-api
    sherpa-onnx-core
    sherpa-onnx-fst
    sherpa-onnx-fstfar
    sherpa-onnx-kaldifst-core
    sherpa-onnx-portaudio_static
    ssentencepiece_core
    ucd
)