#include <string>

namespace config {
std::string ggml_tiny_url =
    "https://huggingface.co/ggerganov/whisper.cpp/resolve/main/ggml-tiny.bin";
std::string ggml_tiny_name = "ggml-tiny.bin";

std::string segmentation_url =
    "https://github.com/thewh1teagle/loud.cpp/releases/download/v0.1.0/"
    "pyannote-segmentation-3-0.onnx";
std::string segmentation_name = "pyannote-segmentation-3-0.onnx";

std::string embedding_url = "https://github.com/thewh1teagle/loud.cpp/releases/"
                            "download/v0.1.0/nemo_en_titanet_small.onnx";
std::string embedding_name = "nemo_en_titanet_small.onnx";

#ifdef _WIN32
std::string ffmpeg_name = "ffmpeg.exe";
std::string ffmpeg_url =
    "https://github.com/shaka-project/static-ffmpeg-binaries/releases/download/"
    "n7.1-1/ffmpeg-win-x64.exe";
#elif defined(__APPLE__)
#if defined(__aarch64__) || defined(__arm64__)
std::string ffmpeg_name = "ffmpeg";
std::string ffmpeg_url =
    "https://github.com/shaka-project/static-ffmpeg-binaries/releases/download/"
    "n7.1-1/ffmpeg-osx-arm64";
#else
std::string ffmpeg_name = "ffmpeg";
std::string ffmpeg_url =
    "https://github.com/shaka-project/static-ffmpeg-binaries/releases/download/"
    "n7.1-1/ffmpeg-osx-x64";
#endif
#elif defined(__linux__)
std::string ffmpeg_name = "ffmpeg";
std::string ffmpeg_url =
    "https://github.com/shaka-project/static-ffmpeg-binaries/releases/download/"
    "n7.1-1/ffmpeg-linux-x64";
#else
#error "Unsupported OS for FFmpeg download."
#endif
} // namespace config
