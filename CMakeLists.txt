cmake_minimum_required(VERSION 3.13)

project(FastNoise)

add_library(${PROJECT_NAME} FastNoise.cpp)

target_compile_options(${PROJECT_NAME} PRIVATE
        $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:GNU>>:
        -Wall
        -Wextra
        -pedantic
        -Wno-implicit-fallthrough
        -Wno-unused-parameter
        -Wno-unused-function>
        $<$<CXX_COMPILER_ID:MSVC>:
        /W4
        /w14640>
        )

set_target_properties(
        ${PROJECT_NAME} PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
)
