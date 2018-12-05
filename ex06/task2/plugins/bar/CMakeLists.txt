cmake_minimum_required(VERSION 3.9)
project(bar)

set(CMAKE_CXX_STANDARD 17)


include_directories(
	../
)

add_library(bar SHARED
	../plugin.hpp
	bar.hpp
	bar.cpp
)
