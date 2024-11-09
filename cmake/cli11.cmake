FetchContent_Declare(cli11 URL https://github.com/CLIUtils/CLI11/archive/refs/tags/v2.4.2.tar.gz)
FetchContent_MakeAvailable(cli11)
target_link_libraries(main PUBLIC CLI11::CLI11)