#include "download.h"
#include "curl/curl.h"
#include "curl/system.h"
#include "spinner.h"
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

Spinner spinner("");
std::string current_file = "";

size_t write_callback(void *contents, size_t size, size_t nmemb, void *userp) {
  std::ofstream *ofs = static_cast<std::ofstream *>(userp);
  ofs->write(static_cast<char *>(contents), size * nmemb);
  return size * nmemb;
}

static int progress_callback(void *ptr, curl_off_t dltotal, curl_off_t dlnow,
                             curl_off_t ultotal, curl_off_t ulnow) {

  if (dltotal > 0) {
    // To avoid issues with extremely small or invalid values
    int progress = static_cast<int>((dlnow * 100) / dltotal);
    if (progress >= 0 && progress <= 100) {
      std::ostringstream message;
      message << "Download " << current_file << " " << std::fixed
              << std::setprecision(0) << progress << "%";
      spinner.updateMessage(message.str());
    }
  }
  return 0;
}

void download_file(std::string url, std::string path) {
  std::ostringstream message;
  current_file = path;
  message << "Download " << path << "...";
  spinner.updateMessage(message.str());
  spinner.start();
  CURL *curl = curl_easy_init();
  if (curl) {
    std::ofstream ofs(path, std::ios::binary);
    // curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &ofs);
    curl_easy_setopt(curl, CURLOPT_XFERINFOFUNCTION, progress_callback);
    curl_easy_setopt(curl, CURLOPT_PROGRESSDATA,
                     nullptr);                      // No extra data passed
    curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0L); // Enable progress tracking
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_perform(curl);
    curl_easy_cleanup(curl);
  }
  spinner.stop();
  std::cout << "✓ Download " << path << " complete!" << std::endl;
}
