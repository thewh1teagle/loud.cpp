#pragma once

#include <string>

#if defined(unix) || defined(__unix__) || defined(__unix__) ||                 \
    defined(__APPLE__)
#define PLATFORM_UNIX
#endif

namespace config {
extern std::string ggml_tiny_url;
extern std::string ggml_tiny_name;

extern std::string segmentation_url;
extern std::string segmentation_name;

extern std::string embedding_url;
extern std::string embedding_name;

extern std::string ffmpeg_url;
extern std::string ffmpeg_name;

} // namespace config
