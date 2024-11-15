#include "diarization.h"
#include "sherpa-onnx/c-api/c-api.h"
#include "transcribe.h"
#include <CLI/CLI.hpp>
#include <nlohmann/json.hpp>
#include <vector>
#include <whisper.h>

namespace segments {
static void
handle_segment(whisper_context *ctx, whisper_full_params params,
               nlohmann::ordered_json *json,
               const std::vector<diarization::DiarizationSegment> &segments,
               std::vector<float> segment_data, int32_t index) {
  auto text = transcribe::transcribe_audio_chunk(
      ctx, params, segment_data.data(), segment_data.size());

  if (text.empty()) {
    return;
  }

  json->push_back({{"text", text},
                   {"start", segments[index].start},
                   {"end", segments[index].end},
                   {"speaker", segments[index].speaker}});

  diarization::print_segment(segments[index], text);
}

nlohmann::ordered_json process_segments(
    std::vector<diarization::DiarizationSegment> segments,
    const SherpaOnnxWave *wave, // Assuming Wave is a defined struct or class
    whisper_context *ctx,       // Assuming Context is a defined struct or class
    whisper_full_params params  // Assuming Params is a defined struct or class
) {
  nlohmann::ordered_json json = nlohmann::json::array();

  // Iterate diarize segments

  for (int32_t i = 0; i != segments.size(); ++i) {

    // Calculate start and end samples for the segment
    int32_t start_sample = static_cast<int32_t>(segments[i].start * 16000);
    int32_t end_sample = static_cast<int32_t>(segments[i].end * 16000);

    // skip segments that are less than 0.5s
    if ((end_sample - start_sample) < 16000 / 2) {
      continue;
    }

    // Ensure start and end are within bounds
    if (start_sample < 0) {
      start_sample = 0;
    }
    if (end_sample > wave->num_samples) {
      end_sample = wave->num_samples;
    }

    int32_t segment_length = end_sample - start_sample;
    int32_t chunk_size = 16000 * 30; // 30 seconds in samples

    // Process longer segments in chunks (no more than 30 seconds each)
    if (segment_length > chunk_size) {
      for (int32_t chunk_start = start_sample; chunk_start < end_sample;
           chunk_start += chunk_size) {
        int32_t chunk_end = std::min(chunk_start + chunk_size, end_sample);

        // Prepare buffer for the chunk
        std::vector<float> chunk_data(wave->samples + chunk_start,
                                      wave->samples + chunk_end);

        // Fill with zeros up to 30 seconds if the chunk is smaller than 30
        // seconds
        if (chunk_data.size() < chunk_size) {
          chunk_data.resize(chunk_size, 0.0f);
        }
        handle_segment(ctx, params, &json, segments, chunk_data, i);
      }
    } else {
      // Process the segment if it's <= 30 seconds
      std::vector<float> segment_data(wave->samples + start_sample,
                                      wave->samples + end_sample);

      // Fill with zeros up to 30 seconds if the segment is smaller than 30
      // seconds
      if (segment_data.size() < chunk_size) {
        segment_data.resize(chunk_size, 0.0f);
      }

      handle_segment(ctx, params, &json, segments, segment_data, i);
    }
  }
  return json;
}
} // namespace segments