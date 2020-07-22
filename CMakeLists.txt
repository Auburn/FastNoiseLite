cmake_minimum_required(VERSION 3.17.0 FATAL_ERROR)

project(FastNoise VERSION 0.1.0 LANGUAGES CXX)

add_library(fastnoise
	src/FastNoise.cpp
	src/FastNoise.h
)

target_include_directories(fastnoise
	PUBLIC
	$<INSTALL_INTERFACE:include>
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
	PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/src
)