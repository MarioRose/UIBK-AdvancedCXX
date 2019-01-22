cmake_minimum_required(VERSION 3.10)
set(CMAKE_BUILD_TYPE "Release")
project(task2 CXX)


add_executable("${PROJECT_NAME}" Task2.cpp)

set(CMAKE_PREFIX_PATH "/home/christoph/Qt/5.12.0/gcc_64/lib/cmake/Qt5Widgets/;${CMAKE_PREFIX_PATH}")

include_directories(${Qt5Widgets_INCLUDE_DIRS})


target_compile_options(
        "${PROJECT_NAME}"
        PRIVATE
        -std=c++17
        -Wextra
        -Wall
)

target_sources(
        "${PROJECT_NAME}"
        PRIVATE
        "${CMAKE_CURRENT_LIST_DIR}/Task2.cpp"
)
