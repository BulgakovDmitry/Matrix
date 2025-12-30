cmake_minimum_required(VERSION 3.16)

project(Matrix LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(SOURCES
    ${CMAKE_SOURCE_DIR}/src/main.cpp
    ${CMAKE_SOURCE_DIR}/src/driver.cpp
)

add_executable(Matrix
    src/main.cpp
    src/driver.cpp
)

target_include_directories(Matrix PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)