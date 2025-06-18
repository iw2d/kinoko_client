cmake_minimum_required(VERSION 3.14)
project(client)

set(CMAKE_CXX_STANDARD 17)

set(CUSTOM_BINARY_DIR
    $ENV{CUSTOM_BINARY_DIR}
)

# Build External Dependencies
add_subdirectory(external)

# Build Injector and Launcher
add_subdirectory(src)
