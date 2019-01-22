cmake_minimum_required(VERSION 3.10)
set(CMAKE_BUILD_TYPE "Release")
project(main CXX)


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

add_custom_target(main COMMAND make -C ${main_SOURCE_DIR}
        CLION_EXE_DIR=${PROJECT_BINARY_DIR})

