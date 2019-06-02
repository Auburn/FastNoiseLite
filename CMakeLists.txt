cmake_minimum_required(VERSION 3.13)

project(FastNoise)

set(SOURCES FastNoise.cpp)

add_library(${PROJECT_NAME} ${SOURCES})

target_compile_options(${PROJECT_NAME} PRIVATE
        -Wall
        -Wextra
        -pedantic
        -Wno-implicit-fallthrough
        -Wno-unused-parameter)

set_target_properties(
        ${PROJECT_NAME} PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
)
