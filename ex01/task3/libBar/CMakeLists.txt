cmake_minimum_required(VERSION 3.10)
project(libBar)

set(	LIB_BAR_DEFAULT_COMPILE_OPTIONS
	-std=c++17
	-Wextra
	-Wall
)

add_library("${PROJECT_NAME}-library" STATIC "")

target_sources(
	"${PROJECT_NAME}-library"
	PRIVATE
		"${CMAKE_CURRENT_LIST_DIR}/bar.cpp"
)

target_include_directories(
	"${PROJECT_NAME}-library"
	PUBLIC
		"${CMAKE_CURRENT_LIST_DIR}"
)

target_compile_options(
	"${PROJECT_NAME}-library"
	PRIVATE
		${LIB_BAR_DEFAULT_COMPILE_OPTIONS}
)


