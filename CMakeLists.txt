cmake_minimum_required(VERSION 3.17.0 FATAL_ERROR)

project(FastNoise VERSION 0.1.0 LANGUAGES CXX)

add_library(fastnoise
	FastNoise.cpp
	FastNoise.h
)