add_library(injector SHARED
    injector.cpp
    walker.cpp
    debug.cpp
    hook.cpp
    system.cpp
    resman.cpp
    bypass.cpp
    helper.cpp
    inlink.cpp
    stringpool.cpp
    resolution.cpp
    chatballoon.cpp
    portablechair.cpp
    temporarystat.cpp
)

target_precompile_headers(injector PRIVATE pch.h)

target_include_directories(injector PRIVATE
    ../external
)

target_link_libraries(injector PRIVATE
    winmm.lib
    ws2_32.lib
    ${PROJECT_SOURCE_DIR}/external/detours/detours.lib
    ${PROJECT_SOURCE_DIR}/external/StackWalker/StackWalker.lib
)

set_target_properties(injector PROPERTIES
    OUTPUT_NAME "Kinoko"
)

add_custom_command(
    TARGET injector POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:injector> ${CUSTOM_BINARY_DIR}
)



add_executable(launcher WIN32
    launcher.cpp
    debug.cpp
)

target_precompile_headers(launcher PRIVATE pch.h)

target_include_directories(launcher PRIVATE
    ../external
)

target_link_libraries(launcher PRIVATE
    ${PROJECT_SOURCE_DIR}/external/detours/detours.lib
)

set_target_properties(launcher PROPERTIES
    OUTPUT_NAME "Kinoko"
)

add_custom_command(
    TARGET launcher POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:launcher> ${CUSTOM_BINARY_DIR}
)