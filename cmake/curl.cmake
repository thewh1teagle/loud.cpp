FetchContent_Declare(curl URL https://curl.se/download/curl-8.11.0.tar.gz)

set(BUILD_STATIC_LIBS ON)
set(HTTP_ONLY ON)
set(BUILD_SHARED_LIBS OFF)
set(BUILD_EXAMPLES OFF)
set(USE_LIBIDN2 OFF)
set(BUILD_CURL_EXE OFF)
set(CURL_USE_LIBPSL OFF)
set(CURL_USE_LIBSSH2 OFF)
set(BUILD_TESTING OFF)

if(WIN32)
    # Native https on Windows
    set(CURL_USE_SCHANNEL ON)
endif()

# Supress warnings
add_compile_options(-w)

FetchContent_MakeAvailable(curl)
target_link_libraries(main PUBLIC CURL::libcurl)