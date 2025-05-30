cmake_minimum_required(VERSION 3.14)
project(client)

set(CMAKE_CXX_STANDARD 17)

set(CUSTOM_BINARY_DIR
    $ENV{CUSTOM_BINARY_DIR}
)
if(NOT CUSTOM_BINARY_DIR)
    set(CUSTOM_BINARY_DIR
        ${CMAKE_BINARY_DIR}/$<CONFIG>
    )
endif()

add_library(detours STATIC
    ${PROJECT_SOURCE_DIR}/external/detours/src/detours.cpp
    ${PROJECT_SOURCE_DIR}/external/detours/src/disasm.cpp
    ${PROJECT_SOURCE_DIR}/external/detours/src/modules.cpp
)
target_include_directories(detours PUBLIC
    ${PROJECT_SOURCE_DIR}/external/detours/src
)

add_subdirectory(${PROJECT_SOURCE_DIR}/external/StackWalker)

add_subdirectory(src)