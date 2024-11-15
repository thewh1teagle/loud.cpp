#pragma once

#include "curl/curl.h"
#include <fmt/core.h>
#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>
#include <string>
#include <termcolor/termcolor.hpp>

inline std::string url_encode(const std::string &decoded) {
  const auto encoded_value = curl_easy_escape(
      nullptr, decoded.c_str(), static_cast<int>(decoded.length()));
  std::string result(encoded_value);
  curl_free(encoded_value);
  return result;
}

// https://stackoverflow.com/a/68523164
#define __FILENAME__ strstr(__FILE__, SOURCE_PATH) + SOURCE_PATH_SIZE

#define CHECK_NULL(ptr)                                                        \
  if (!ptr) {                                                                  \
    std::string issue_body = fmt::format(                                      \
        "\n\n**Version**: `{}`\n**Commit**: `{}`\n**Location**: "              \
        "[`{}:{}`](https://github.com/thewh1teagle/loud.cpp/blob/"             \
        "{}/{}#L{})",                                                          \
        TAG, REV, __FILENAME__, __LINE__, REV, __FILENAME__, __LINE__);        \
    std::string issue_url =                                                    \
        fmt::format("https://github.com/thewh1teagle/loud.cpp/issues/"         \
                    "new?labels=bug&body={}",                                  \
                    url_encode(issue_body));                                   \
    SPDLOG_ERROR("Error: Null pointer detected. Unexpected? "                  \
                 "Report at: {}",                                              \
                 issue_url);                                                   \
    return EXIT_FAILURE;                                                       \
  }

namespace utils {

std::string get_relative_path(const std::string &absolute_path);
std::string get_random_string(int length);
std::string get_random_path(std::string suffix);
bool is_program_installed(std::string name);
std::string get_argv_line(int argc, char *argv[]);
void save_json(const std::string &json_path,
               const nlohmann::ordered_json &result_json);
bool check_resource_exists(const std::string &model_path, int argc,
                           char *argv[]);
bool contains(int argc, char *argv[], const std::string &arg);

void set_executable(const std::string &file_path);
bool check_program_installed(const std::string &program_path, int argc,
                             char *argv[]);
void log_version();
} // namespace utils