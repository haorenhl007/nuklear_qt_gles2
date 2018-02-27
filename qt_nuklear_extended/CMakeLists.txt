cmake_minimum_required (VERSION 2.6)

project (nuklear_extended)

add_subdirectory(SystemAbstraction)

add_executable(extended main.cpp)

target_link_libraries(extended
system_abstraction
GLESv2
glfw
)
