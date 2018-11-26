cmake_minimum_required(VERSION 3.10)
set(CMAKE_BUILD_TYPE "Release")
project(task2 CXX)


add_executable("${PROJECT_NAME}" "")

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
        "${CMAKE_CURRENT_LIST_DIR}/task2.cpp"
)
