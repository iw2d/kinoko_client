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
    external/detours/src/detours.cpp
    external/detours/src/disasm.cpp
    external/detours/src/modules.cpp
)
target_include_directories(detours PUBLIC
    external/detours/src
)

set(StackWalker_DISABLE_TESTS ON CACHE BOOL "Disable StackWalker tests")
add_subdirectory(external/StackWalker)

add_subdirectory(src)
