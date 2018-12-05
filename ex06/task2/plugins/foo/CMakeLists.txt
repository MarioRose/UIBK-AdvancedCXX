cmake_minimum_required(VERSION 3.9)
project(foo)

set(CMAKE_CXX_STANDARD 17)


include_directories(
	../
)

add_library(foo SHARED
	../plugin.hpp
	foo.hpp
	foo.cpp
)
