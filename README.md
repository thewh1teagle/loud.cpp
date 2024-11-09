# loud.cpp

Whisper.cpp with diarization

## Features

- Diarize with onnxruntime
- Segment using pyannote-audio model
- Transcribe using OpenAI whisper
- Support macOS, Windows, Linux

## Install

Download and extract it from [releases](https://github.com/thewh1teagle/loud.cpp/releases/latest)

## Usage

```console
./loud ggml-tiny.bin single.wav --json transcript.json --download-models
```

## Building

See [BUILDING.md](docs/BUILDING.md)
