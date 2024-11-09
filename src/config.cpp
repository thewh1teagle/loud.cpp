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

#ifdef __APPLE__
std::string platform = "macos";
#elif _WIN32
std::string platform = "windows";
#elif __linux__
std::string platform = "linux";
#else
std::string platform = "unknown";
#endif

} // namespace config
