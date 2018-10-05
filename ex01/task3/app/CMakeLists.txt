cmake_minimum_required(VERSION 3.10)
project(app CXX)




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
		libBar-library
		libBaz-library
		libFoo-library
		
)
