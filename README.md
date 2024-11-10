# loud.cpp

Whisper.cpp with diarization

## Features

- Diarize with onnxruntime
- Segment using pyannote-audio model
- Transcribe using OpenAI whisper
- Support macOS, Windows, Linux
- Comes with FFMPEG
- Support any audio or video format
- Download models automatically with `--download-models`
- Colored output for easy identification of speakers

## Install

Download and extract it from [releases](https://github.com/thewh1teagle/loud.cpp/releases/latest)

## Usage

```console
./loud audio.wav --json transcript.json --download-models
```

## Building

See [building.md](docs/building.md)
