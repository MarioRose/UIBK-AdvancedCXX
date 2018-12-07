cmake_minimum_required(VERSION 3.10)
project(sidescroller)

# Create the executable
add_executable(${PROJECT_NAME} app.cpp)

include(FindPkgConfig)

PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)
PKG_SEARCH_MODULE(SDL2IMAGE REQUIRED SDL2_image>=2.0.0)
PKG_SEARCH_MODULE(SDL2MIXER REQUIRED SDL2_mixer>=2.0.0)

include_directories(${SDL2_INCLUDE_DIRS} ${SDL2IMAGE_INCLUDE_DIRS} ${SDL2MIXER_INCLUDE_DIRS})

add_custom_command(TARGET sidescroller POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy_directory
			${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:sidescroller>/assets
)

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
		"${SDL2IMAGE_LIBRARIES}"
		"${SDL2MIXER_LIBRARIES}"
		sub::timer
		sub::texture
		sub::character
    sub::player
		sub::settings
		sub::room
    sub::gameobjects
    sub::moveable
)
