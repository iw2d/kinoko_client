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

# Build Detours
add_library(detours STATIC
	external/Detours/src/creatwth.cpp
	external/Detours/src/modules.cpp
	external/Detours/src/detours.cpp
	external/Detours/src/disolx86.cpp
	external/Detours/src/disasm.cpp
	external/Detours/src/image.cpp
)
include_directories(
    external/Detours/src
)

# Build StackWalker
set(StackWalker_DISABLE_TESTS ON CACHE BOOL "Disable StackWalker tests")
add_subdirectory(external/StackWalker)
include_directories(
    external/StackWalker/Main/StackWalker
)

# Build Injector and Launcher
add_subdirectory(src)
