cmake_minimum_required(VERSION 3.16)

project(Matrix LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(Matrix
    src/main.cpp
)

target_include_directories(Matrix PRIVATE
    include
)

enable_testing()
add_subdirectory(tests)