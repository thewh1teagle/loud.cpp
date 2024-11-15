#pragma once

#include "diarization.h"
#include "sherpa-onnx/c-api/c-api.h"
#include "whisper.h"
#include <nlohmann/json.hpp>

namespace segments {

// Function to process all segments and return a JSON result
nlohmann::ordered_json process_segments(
    std::vector<diarization::DiarizationSegment> segments,
    const SherpaOnnxWave *wave, // Assuming Wave is a defined struct or class
    whisper_context *ctx,       // Assuming Context is a defined struct or class
    whisper_full_params params  // Assuming Params is a defined struct or class
);

} // namespace segments
