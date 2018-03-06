cmake_minimum_required (VERSION 2.6)

project (system_abstraction)

include_directories(.)

add_subdirectory(Application/libs/nuklear/implementation)

add_library (system_abstraction STATIC
    system_abstraction.cpp
    system_paths.cpp
    demo.cpp
    )

target_link_libraries(system_abstraction
    nuklear_gles2
    )

