add_library(injector SHARED
        injector.cpp
        hook.cpp
        system.cpp
        bypass.cpp
        debug.cpp
)

target_precompile_headers(injector PRIVATE pch.h)

target_include_directories(injector PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(injector PRIVATE
        winmm.lib
        ws2_32.lib
        detours
)

set_target_properties(injector PROPERTIES
        OUTPUT_NAME "Kinoko"
)


add_executable(launcher WIN32
        launcher.cpp
        debug.cpp
)

set_target_properties(launcher PROPERTIES
        OUTPUT_NAME "Kinoko"
)

target_link_libraries(launcher PRIVATE
        detours
)


if (CUSTOM_BINARY_DIR)
    add_custom_command(
            TARGET injector POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:injector> ${CUSTOM_BINARY_DIR}
    )
    add_custom_command(
            TARGET launcher POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:launcher> ${CUSTOM_BINARY_DIR}
    )
endif ()