#include "ggml.h"
#include <iostream>
#include <spdlog/spdlog.h>
#include <sstream>
#include <string>
#include <whisper.h>

namespace transcribe {
whisper_full_params create_whisper_params(std::string &language) {

  whisper_log_set(
      [](enum ggml_log_level level, const char *message, void *user_data) {
        // Use the debug flag from the context
        if (level == GGML_LOG_LEVEL_INFO) {
          level = GGML_LOG_LEVEL_DEBUG;
        }
        switch (level) {
        case GGML_LOG_LEVEL_DEBUG:
          SPDLOG_DEBUG("{}", message);
          break;
        case GGML_LOG_LEVEL_INFO:
          SPDLOG_INFO("{}", message);
          break;
        case GGML_LOG_LEVEL_WARN:
          SPDLOG_WARN("{}", message);
          break;
        case GGML_LOG_LEVEL_ERROR:
          SPDLOG_ERROR("{}", message);
          break;
        default:
          SPDLOG_ERROR("{}", message);
          break;
        }
      },
      nullptr); // Pass raw pointer for user_data

  // Set other whisper parameters as needed...

  whisper_full_params wparams =
      whisper_full_default_params(WHISPER_SAMPLING_GREEDY);
  // https://github.com/ggerganov/whisper.cpp/blob/master/examples/talk/talk.cpp
  wparams.new_segment_callback = NULL;
  wparams.language = language.c_str();
  wparams.print_realtime = false;
  wparams.debug_mode = false;
  wparams.no_timestamps = true;
  wparams.print_special = false;
  wparams.translate = false;
  wparams.single_segment = true;
  wparams.print_progress = false;
  wparams.no_context = true;

  return wparams;
}

std::string transcribe_audio_chunk(whisper_context *ctx,
                                   const whisper_full_params &params,
                                   const float *samples, int n_samples) {
  // Process the chunk with Whisper
  if (whisper_full(ctx, params, samples, n_samples) != 0) {
    std::cerr << "Failed to process audio chunk." << std::endl;
    return "";
  }

  // Get and return the chunk transcription
  const int n_segments = whisper_full_n_segments(ctx);
  SPDLOG_DEBUG("got {} segments", n_segments);
  std::ostringstream transcription;

  for (int j = 0; j < n_segments; j++) {
    const char *segment_text = whisper_full_get_segment_text(ctx, j);
    SPDLOG_DEBUG("segment[{}] = {}", j, segment_text);
    transcription << segment_text << " ";
  }

  return transcription.str();
}

} // namespace transcribe