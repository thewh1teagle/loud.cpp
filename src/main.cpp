#include "CLI/CLI.hpp"
#include "config.h"
#include "diarization.h"
#include "download.h"
#include "segments.h"
#include "sherpa-onnx/c-api/c-api.h"
#include "spinner.h"
#include "transcribe.h"
#include "utils.h"
#include <CLI/CLI.hpp>
#include <iostream>
#include <nlohmann/json.hpp>
#include <termcolor/termcolor.hpp>
#include <whisper.h>

namespace fs = std::filesystem;

using spinner::Spinner;
using utils::contains;

int main(int argc, char *argv[]) {

  CLI::App app{"Loud.cpp\nSpeech to text with ONNX and Whisper\n"};

  std::string whisper_model_path = config::ggml_tiny_name;
  std::string audio_file;
  std::string json_path;
  std::string segmentation_model_path = config::segmentation_name;
  std::string embedding_model_path = config::embedding_name;
  std::string language = "en";
  int32_t num_speakers = 4;
  int32_t onnx_num_threads = 4;
  std::string onnx_provider = diarization::get_default_provider();
  bool debug = false;
  bool setup = false;
  bool show_version = false;

  // Audio required conditionally
  auto audio_flag =
      app.add_option("audio", audio_file, "Path to the audio file")
          ->check(CLI::ExistingFile);
  if (!contains(argc, argv, "--version") && !contains(argc, argv, "-v")) {
    audio_flag->required();
  }

  app.add_option("--language", language,
                 "Language to transcribe with (Default: en)");
  app.add_flag("--setup", setup,
               "Download models (pyannote segment, whisper tiny, nemo small "
               "en) and FFMPEG if not found");
  app.add_flag("--version,-v", show_version, "Show loud.cpp version and exit");
  app.add_option("--json", json_path, "Path to save the JSON output");
  app.add_option("--whisper-model", whisper_model_path, "Path to the model");
  app.add_option("--segmentation-model", segmentation_model_path,
                 "Path to the segmentation model");
  app.add_option("--embedding-model", embedding_model_path,
                 "Path to the embedding model");
  app.add_option("--num-speakers", num_speakers,
                 "Number of speakers in the file");
  app.add_option("--onnx-provider", onnx_provider, "Onnx execution provider");
  app.add_option("--onnx-num-threads", onnx_num_threads,
                 "Onnx number of threads (Default: 4)");
  app.add_flag("--debug", debug, "Enable debug output");

  try {
    app.parse(argc, argv);
  } catch (const CLI::ParseError &e) {
    return app.exit(e);
  }

  if (show_version) {
    std::cout << termcolor::green << "==== loud.cpp " << termcolor::blue
              << VERSION << termcolor::green << " ====" << termcolor::reset
              << std::endl
              << termcolor::reset << std::endl;
    return EXIT_SUCCESS;
  }

  // Download models
  if (setup) {
    download::download_resources_if_needed();
  }

  // Check if models exists
  if (!utils::check_resource_exists(embedding_model_path, argc, argv))
    return EXIT_FAILURE;
  if (!utils::check_resource_exists(segmentation_model_path, argc, argv))
    return EXIT_FAILURE;
  if (!utils::check_resource_exists(whisper_model_path, argc, argv))
    return EXIT_FAILURE;

  // Check if it's not wav file. then suggest download FFMPEG
  if (fs::path(audio_file).extension().string() != ".wav") {
    if (!utils::check_program_installed("ffmpeg", argc, argv)) {
      return EXIT_FAILURE;
    }
  }

  // Read wave file
  auto wave = diarization::prepare_audio_file(audio_file, argc, argv);
  if (!wave)
    return EXIT_FAILURE;

  // Start diarization
  Spinner spinner("Starting diarization...");
  auto *sd =
      diarization::create_sd(segmentation_model_path, embedding_model_path,
                             num_speakers, onnx_provider, onnx_num_threads);
  CHECK_NULL(sd);
  spinner.start();

  const auto *result = diarization::run_diarization(sd, wave, spinner);
  spinner.stop();
  CHECK_NULL(result);
  const auto num_segments =
      SherpaOnnxOfflineSpeakerDiarizationResultGetNumSegments(result);
  const auto *segments =
      SherpaOnnxOfflineSpeakerDiarizationResultSortByStartTime(result);

  // Start transcribe
  const auto params = transcribe::create_whisper_params(language, debug);
  const auto cparams = whisper_context_default_params();
  auto *ctx =
      whisper_init_from_file_with_params(whisper_model_path.c_str(), cparams);
  CHECK_NULL(ctx);

  auto json =
      segments::process_segments(segments, num_segments, wave, ctx, params);
  // Write JSON file
  if (!json_path.empty()) {
    utils::save_json(json_path, json);
  }

  // Cleanup
  SherpaOnnxOfflineSpeakerDiarizationDestroySegment(segments);
  SherpaOnnxOfflineSpeakerDiarizationDestroyResult(result);
  SherpaOnnxDestroyOfflineSpeakerDiarization(sd);
  SherpaOnnxFreeWave(wave);
  whisper_free(ctx);
  return 0;
}