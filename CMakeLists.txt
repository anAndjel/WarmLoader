cmake_minimum_required(VERSION 3.16)

project(WarmLoader
  VERSION 0.1.0
  LANGUAGES CXX
)

add_executable(warmloader)

target_sources(warmloader
  PRIVATE
    src/main.cpp
)

target_include_directories(warmloader
  PRIVATE
    cxx_std_23
)

target_compile_options(warmloader
  PRIVATE
    -Wall
    -Wextra
    -Wpedantic
    -Wconversion
    -Wsign-conversion
)

set_target_properties(warmloader PROPERTIES
  CXX_EXTENSIONS OFF
)
