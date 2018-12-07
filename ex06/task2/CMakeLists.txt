cmake_minimum_required(VERSION 3.9)
project(main)

set(CMAKE_CXX_STANDARD 17)

### Output directory ###
set(CMAKE_OUTPUT_DIR ${CMAKE_SOURCE_DIR}/build)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_OUTPUT_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_OUTPUT_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_OUTPUT_DIR})


set (CMAKE_CXX_FLAGS "-W -Wall -Wextra")

include_directories(
	plugins/
)

add_executable(main
	${DLLOADER_SRC}
	main.cpp
)

target_link_libraries(main ${CMAKE_DL_LIBS})

set_property( DIRECTORY PROPERTY VS_STARTUP_PROJECT main)

add_subdirectory(plugins/bar)
add_subdirectory(plugins/foo)
