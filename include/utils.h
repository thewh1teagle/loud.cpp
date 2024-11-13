#pragma once

#include <nlohmann/json.hpp>
#include <string>

#ifndef VERSION
#define VERSION ""
#endif

// https://stackoverflow.com/a/68523164
#define __FILENAME__ strstr(__FILE__, SOURCE_PATH) + SOURCE_PATH_SIZE

#ifndef CHECK_NULL
#define CHECK_NULL(ptr)                                                        \
  if (!ptr) {                                                                  \
    std::cerr << "Error: Null pointer detected (" << #ptr << ")"               \
              << " in " << __func__ << " at " << __FILENAME__ << ":"           \
              << __LINE__ << std::endl;                                        \
    if (VERSION[0] != '\0') {                                                  \
      std::cerr << "  Commit: " << VERSION << std::endl;                       \
      std::cerr << "  See the issue report here (including commit hash):"      \
                << std::endl;                                                  \
      std::cerr << "  "                                                        \
                   "https://github.com/thewh1teagle/loud.cpp/issues/"          \
                   "new?body=version="                                         \
                << VERSION << std::endl;                                       \
    } else {                                                                   \
      std::cerr << "Does this seem unexpected? Report the issue at: "          \
                << "https://github.com/thewh1teagle/loud.cpp/issues/new"       \
                << std::endl;                                                  \
    }                                                                          \
    return EXIT_FAILURE;                                                       \
  }
#endif

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
} // namespace utils