#pragma once

#include <string>

namespace download {
void download_file(std::string url, std::string path);
void download_resources_if_needed();
} // namespace download