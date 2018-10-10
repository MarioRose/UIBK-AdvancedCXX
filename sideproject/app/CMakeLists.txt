cmake_minimum_required(VERSION 3.10)
project(app CXX)


find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

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
		"${CMAKE_CURRENT_LIST_DIR}/app.cpp"
)

target_link_libraries(
	"${PROJECT_NAME}"
	PRIVATE
		"${SDL2_LIBRARIES}"
)
