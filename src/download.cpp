#include "download.h"
#include "config.h"
#include "curl/curl.h"
#include "curl/system.h"
#include "spinner.h"
#include "utils.h"
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <termcolor/termcolor.hpp>

namespace fs = std::filesystem;

namespace download {

struct DownloadContext {
  spinner::Spinner spinner;
  std::string file_name;
};

static size_t write_callback(char *contents, size_t size, size_t nmemb,
                             std::ofstream *userp) {
  userp->write(contents, size * nmemb);
  return size * nmemb;
}

static int progress_callback(DownloadContext *ctx, curl_off_t dltotal,
                             curl_off_t dlnow, curl_off_t ultotal,
                             curl_off_t ulnow) {

  if (dltotal > 0) {
    // To avoid issues with extremely small or invalid values
    int progress = static_cast<int>((dlnow * 100) / dltotal);
    if (progress >= 0 && progress <= 100) {
      std::ostringstream message;
      message << "Download " << ctx->file_name << " " << std::fixed
              << std::setprecision(0) << progress << "%";
      ctx->spinner.updateMessage(message.str());
    }
  }
  return 0;
}

void download_file(const std::string url, const std::string path) {
  DownloadContext ctx;
  ctx.file_name = path;
  ctx.spinner.updateMessage("Download " + path + "...");
  ctx.spinner.start();

  CURLcode res;
  if (CURL *curl = curl_easy_init(); curl) {
    std::ofstream ofs(path, std::ios::binary);
    // curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(
        curl, CURLOPT_WRITEFUNCTION,
        +[](void *contents, size_t size, size_t nmemb, void *userp) {
          return write_callback(static_cast<char *>(contents), size, nmemb,
                                static_cast<std::ofstream *>(userp));
        });
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &ofs);
    curl_easy_setopt(curl, CURLOPT_XFERINFODATA, &ctx);
    curl_easy_setopt(curl, CURLOPT_XFERINFOFUNCTION, progress_callback);
    curl_easy_setopt(curl, CURLOPT_NOPROGRESS,
                     0L); // Enable progress tracking
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);
  }

  ctx.spinner.stop();
  if (res == CURLE_OK) {
    std::cout << termcolor::green << "✓" << termcolor::reset << " Download "
              << path << " complete!" << std::endl;
  } else {
    std::cerr << termcolor::red << "✗" << termcolor::reset << " Download of "
              << path << " from " << url
              << " failed: " << curl_easy_strerror(res) << std::endl;
  }
}
void download_resources_if_needed() {
  if (!utils::is_program_installed("ffmpeg")) {
    download_file(config::ffmpeg_url, config::ffmpeg_name);
    utils::set_executable(config::ffmpeg_name);
  }

  if (!fs::exists(config::segmentation_name)) {
    download_file(config::segmentation_url, config::segmentation_name);
  }
  if (!fs::exists(config::embedding_name)) {
    download_file(config::embedding_url, config::embedding_name);
  }
  if (!fs::exists(config::ggml_tiny_name)) {
    download_file(config::ggml_tiny_url, config::ggml_tiny_name);
  }
}

} // namespace download