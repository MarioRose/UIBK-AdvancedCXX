cmake_minimum_required(VERSION 3.8)
project(task1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

#add_library(foo SHARED libFoo/foo.cpp)
#target_include_directories(foo PUBLIC libFoo)

add_executable(intermediate-1 task1.cpp)
#target_link_libraries(executable foo)
