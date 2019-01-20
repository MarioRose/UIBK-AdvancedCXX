# Set the project name
project (util)

include(FindPkgConfig)

# Add a library with the above sources
add_library(${PROJECT_NAME} Util.cpp)
add_library(sub::util ALIAS ${PROJECT_NAME})

target_include_directories( ${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}
)
