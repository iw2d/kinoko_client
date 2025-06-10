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
    itemeff.cpp
    stringpool.cpp
    resolution.cpp
    chatballoon.cpp
    portablechair.cpp
    temporarystat.cpp
    hyperteleport.cpp
    elementaldamage.cpp
)

target_precompile_headers(injector PRIVATE pch.h)

target_link_libraries(injector PRIVATE
    winmm.lib
    ws2_32.lib
    StackWalker
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

set_target_properties(launcher PROPERTIES
    OUTPUT_NAME "Kinoko"
)

add_custom_command(
    TARGET launcher POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:launcher> ${CUSTOM_BINARY_DIR}
)