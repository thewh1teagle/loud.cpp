#include "config.h"
#include "subprocess/ProcessBuilder.hpp"
#include "subprocess/basic_types.hpp"
#include "subprocess/shell_utils.hpp"
#include <CLI/CLI.hpp>
#include <filesystem>
#include <iostream>
#include <nlohmann/json.hpp>
#include <random>
#include <sherpa-onnx/c-api/c-api.h>
#include <string>
#include <subprocess.hpp>
#include <whisper.h>

namespace fs = std::filesystem;

namespace ffmpeg {

bool is_ffmpeg_installed() {
  auto path = subprocess::find_program("ffmpeg");
  return !path.empty();
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

void normalize_audio(std::string input, std::string output) {
  std::cout << "Normalizing audio from " << input << " to " << output;
  using subprocess::CompletedProcess;
  using subprocess::PipeOption;
  using subprocess::RunBuilder;

  CompletedProcess proc = subprocess::run(
      {
          "ffmpeg",
          "-i",
          input,
          "-ar",
          "16000",
          "-ac",
          "1",
          "-c:a",
          "pcm_s16le",
          output,
      },
      RunBuilder().cout(PipeOption::cerr).cerr(PipeOption::pipe));
}

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

void show_ffmpeg_normalize_suggestion(const std::string &audio_path, int argc,
                                      char *argv[]) {

  std::cout << "It seems like you're on " << config::platform
            << ". Here's how you can normalize your audio using ffmpeg:\n";

  if (!is_ffmpeg_installed()) {
    std::cout << "**Install ffmpeg:**\n";
    if (config::platform == "macos") {
      std::cout << "brew install ffmpeg\n";
    } else if (config::platform == "windows") {
      std::cout << "winget install --id=Gyan.FFmpeg\n";
    } else if (config::platform == "linux") {
      std::cout << "sudo apt install ffmpeg\n";
    }
  }

  std::cout << "\n**Normalize the audio:**\n";
  std::cout << "ffmpeg -i " << audio_path
            << " -ar 16000 -ac 1 -c:a pcm_s16le output.wav\n";

  // Suggest running the command again on the output file, replacing audio_path
  // with output.wav
  std::cout << "\n**Once the normalization process is complete, you can run "
               "the command again on the output file:**\n";
  for (int i = 0; i < argc; ++i) {
    std::string arg = argv[i];
    if (i == 0) {
      std::string relative_path = get_relative_path(arg);
      std::cout << relative_path << " ";
    } else if (arg == audio_path) {
      std::cout << "output.wav" << " ";
    } else {
      std::cout << arg << " ";
    }
  }
  std::cout << "\n";
}

} // namespace ffmpeg