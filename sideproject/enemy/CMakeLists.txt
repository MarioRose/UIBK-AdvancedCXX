# Set the project name
project (enemy)

include(FindPkgConfig)

PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)
PKG_SEARCH_MODULE(SDL2IMAGE REQUIRED SDL2_image>=2.0.0)

include_directories(
  ${SDL2_INCLUDE_DIRS}
  ${SDL2IMAGE_INCLUDE_DIRS}
  ../character
)

# Add a library with the above sources
add_library(${PROJECT_NAME}
        Enemy.cpp
        Boss.cpp
        SecondBoss.cpp
        ThirdBoss.cpp
        Projectile.cpp
        Projectile_vert.cpp
        )

add_library(sub::enemy ALIAS ${PROJECT_NAME})

target_include_directories( ${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}
)

target_link_libraries(
	"${PROJECT_NAME}"
	PRIVATE
        sub::settings
        sub::texture
        sub::gameobjects
)
