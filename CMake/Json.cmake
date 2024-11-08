include(FetchContent)

# Add JSON lib
FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz)
FetchContent_MakeAvailable(json)
target_link_libraries(main PUBLIC nlohmann_json::nlohmann_json)