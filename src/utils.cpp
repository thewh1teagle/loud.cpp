#include <filesystem>
#include <fstream>
#include <nlohmann/json.hpp>
#include <random>
#include <sstream>
#include <string>
#include <subprocess.hpp>

namespace fs = std::filesystem;

namespace utils {
std::string get_relative_path(const std::string &absolute_path) {
  fs::path cwd = fs::current_path();
  fs::path file_path(absolute_path);

  // Make paths absolute to ensure correct relative path calculation
  cwd = fs::absolute(cwd);
  file_path = fs::absolute(file_path);

  // Calculate the relative path
  fs::path relative_path = fs::relative(file_path, cwd);

  return relative_path.string();
}

std::string get_random_string(int length) {
  std::string str(
      "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");

  std::random_device rd;
  std::mt19937 generator(rd());

  std::shuffle(str.begin(), str.end(), generator);

  // Ensure the length does not exceed the size of the string
  if (length > str.size()) {
    length = str.size();
  }

  return str.substr(0, length);
}

std::string get_random_path(std::string suffix) {
  fs::path tmp_dir = fs::temp_directory_path();
  std::string random_part = get_random_string(5);
  fs::path random_path = tmp_dir / (random_part + suffix);
  return random_path.string();
}

bool is_program_installed(std::string name) {
  auto path = subprocess::find_program(name);
  return !path.empty();
}

std::string get_argv_line(int argc, char *argv[]) {
  std::ostringstream line;
#ifdef _WIN32
  line << ".\\";
#else
  line << "./";
#endif

  for (int i = 0; i < argc; i++) {
    if (i == 0 || i == 1) {
      line << get_relative_path(argv[i]) << " ";
    } else {
      line << argv[i] << " ";
    }
  }
  return line.str();
}

void save_json(const std::string &json_path,
               const nlohmann::ordered_json &result_json) {
  std::ofstream json_output(json_path);
  if (json_output.is_open()) {
    json_output << result_json.dump(4); // Pretty print
  } else {
    std::cerr << "Error: Could not open file for writing: " << json_path
              << std::endl;
  }
}

bool check_model_exists(const std::string &model_path, int argc, char *argv[]) {
  if (!fs::exists(model_path)) {
    std::cout << "Model " << " not found at " << model_path << std::endl
              << std::endl
              << "Please execute the following command to download models "
                 "automatically:"
              << std::endl
              << utils::get_argv_line(argc, argv) << " --download-models"
              << std::endl;
    return false;
  }
  return true;
}

} // namespace utils