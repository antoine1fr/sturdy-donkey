cmake_minimum_required(VERSION 3.12)
project(sturdy-donkey VERSION 0.4.0 LANGUAGES C CXX)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Search for dependencies in submodules when falling back to config mode.
list(INSERT CMAKE_PREFIX_PATH 0 ${PROJECT_SOURCE_DIR})
list(INSERT CMAKE_MODULE_PATH 0 ${PROJECT_SOURCE_DIR}/cmake)

find_package(glm REQUIRED MODULE)
find_package(gl3w REQUIRED MODULE)
find_package(SDL2 REQUIRED MODULE)
find_package(SDL2_image REQUIRED MODULE)

add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/lib/imgui")
add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/lib/tinyobjloader")
add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/base")
add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/shaders")
add_subdirectory("test")
