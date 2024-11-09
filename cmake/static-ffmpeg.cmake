# Download static FFMPEG from https://github.com/shaka-project/static-ffmpeg-binaries
set(FFMPEG_BIN_DIR "${CMAKE_BINARY_DIR}/bin")
file(MAKE_DIRECTORY ${FFMPEG_BIN_DIR})

if (WIN32)
    # Windows
    set(FFMPEG_URL "https://github.com/shaka-project/static-ffmpeg-binaries/releases/download/n7.1-1/ffmpeg-win-x64.exe")
    set(FFMPEG_BIN_PATH "${FFMPEG_BIN_DIR}/ffmpeg.exe")
elseif (APPLE)
    # Darwin arm64
    if (CMAKE_SYSTEM_PROCESSOR MATCHES "arm64")
        set(FFMPEG_URL "https://github.com/shaka-project/static-ffmpeg-binaries/releases/download/n7.1-1/ffmpeg-osx-arm64")
    else()
    # Darwin x64
        set(FFMPEG_URL "https://github.com/shaka-project/static-ffmpeg-binaries/releases/download/n7.1-1/ffmpeg-osx-x64")
    endif()
    set(FFMPEG_BIN_PATH "${FFMPEG_BIN_DIR}/ffmpeg")
elseif (UNIX)
    # Linux x64
    set(FFMPEG_URL "https://github.com/shaka-project/static-ffmpeg-binaries/releases/download/n7.1-1/ffmpeg-linux-x64")
    set(FFMPEG_BIN_PATH "${FFMPEG_BIN_DIR}/ffmpeg")
else()
    message(FATAL_ERROR "Unsupported OS for FFmpeg download.")
endif()

if (NOT EXISTS ${FFMPEG_BIN_PATH})
    # Download
    file(DOWNLOAD ${FFMPEG_URL} ${FFMPEG_BIN_PATH} SHOW_PROGRESS)
    if (NOT WIN32)
        # Set permission
        execute_process(COMMAND chmod +x ${FFMPEG_BIN_PATH})
    endif()
endif()
