cmake_minimum_required(VERSION 3.22)
project(CHIP-8)

set(CMAKE_CXX_STANDARD 20)

find_package(SDL2 REQUIRED)

# Use `ccache` if it is installed in system's PATH.
find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
  set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CCACHE_PROGRAM}")
endif()

# Export compilation database in JSON format.
set( CMAKE_EXPORT_COMPILE_COMMANDS on )

# Ignore warnings.
add_definitions(-w)

add_executable(
  c8
  src/constructor.cpp
  src/hardware.cpp
  src/loadrom.cpp
  src/main.cpp
  src/opcodes.cpp
  )

target_include_directories(
  c8
  PUBLIC src/
)

target_compile_options(c8 PRIVATE -Wall)

target_include_directories(c8 PUBLIC ${SDL2_INCLUDE_DIR})
target_link_libraries(c8 PUBLIC SDL2::SDL2)
target_compile_definitions(c8 PUBLIC -DSDL_MAIN_HANDLED)