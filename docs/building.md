# Building

### Prerequisites

[Clang](https://releases.llvm.org/download.html) | [Cmake](https://cmake.org/download/) | [Ninja](https://ninja-build.org/)

Windows

```console
winget install -e --id GnuWin32.Tar
winget install -e --id JernejSimoncic.Wget
winget install -e --id Kitware.CMake
winget install --id=Ninja-build.Ninja  -e
```

Build

```console
cmake -G Ninja -B build .
cmake --build build --target main
./build/bin/loud
```

Build release

```console
cmake -G Ninja -B build -DCMAKE_BUILD_TYPE=Release .
cmake --build build --config Release --target main
```

Run

```console
./build/bin/loud -h
```

Download and set up FFmpeg during compile automatically:

```console
cmake -G Ninja -B build . -DCMAKE_BUILD_TYPE=Release -DFFMPEG_DOWNLOAD=ON
```

Build with vulkan sdk on Windows

See https://vulkan.lunarg.com/sdk/home

```console
sudo apt-get install -y libvulkan1 mesa-vulkan-drivers
```

```console
$env:VULKAN_SDK = "C:\VulkanSDK\1.3.296.0"
cmake -G Ninja -B build . -DCMAKE_BUILD_TYPE=Release -DGGML_VULKAN=ON
cmake --build build --config Release
```

## Gotchas

OpenMP not found on macOS

```console
brew install libomp
export OpenMP_ROOT=$(brew --prefix)/opt/libomp
```

Windows doesn't produce `compile_commands.json` for clangd

Build with ninja backend

```console
cmake -G Ninja -B build . -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
```

## Create release and upload

```console
gh release create v0.1.0 --notes ''
gh release upload `git describe --tags --abbrev=0` loud.exe
```

## Failed to execute on macos

```console
xattr -d com.apple.quarantine *
chmod +x *
```

## Cross compile from macOS arm to x64

```console
cmake -G Ninja -B build -DCMAKE_BUILD_TYPE=Release -DCMAKE_OSX_ARCHITECTURES="x86_64" -DCMAKE_SYSTEM_NAME=Darwin .
cmake --build build --config Release
```

## Compare pyannote-diarization

https://huggingface.co/spaces/pyannote/pretrained-pipelines

## Try more embedding models

See [sherpa-onnx/releases/tag/speaker-recongition-models](https://github.com/k2-fsa/sherpa-onnx/releases/tag/speaker-recongition-models)

## Faster build with clang on Windows

```console
$env:CC = "clang"
$env:CXX = "clang++"
```

## Link sherpa static

```console
cmake -G "Ninja" -B build . -DCMAKE_BUILD_TYPE=Release -DGGML_VULKAN=ON -DSHERPA_STATIC=ON
cmake --build build --config Release
```