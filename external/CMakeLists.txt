# Build Detours
add_library(Detours STATIC
        Detours/src/creatwth.cpp
        Detours/src/modules.cpp
        Detours/src/detours.cpp
        Detours/src/disolx86.cpp
        Detours/src/disasm.cpp
        Detours/src/image.cpp
)
target_include_directories(Detours PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Detours/src)


# Build StackWalker
set(StackWalker_DISABLE_TESTS ON CACHE BOOL "Disable StackWalker tests")
add_subdirectory(StackWalker)
target_include_directories(StackWalker PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/StackWalker/Main/StackWalker)