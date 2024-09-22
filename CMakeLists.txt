cmake_minimum_required(VERSION 3.14)
project(client)

set(CMAKE_CXX_STANDARD 17)

set(CUSTOM_BINARY_DIR
    ${CMAKE_BINARY_DIR}/$<CONFIG>
)

add_subdirectory(src)