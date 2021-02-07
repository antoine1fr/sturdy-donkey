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

add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/lib/tinyobjloader")
add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/base")

set(SHADER_DIR "${CMAKE_CURRENT_LIST_DIR}/shaders")
set(SHADER_FILES
  "${SHADER_DIR}/albedo-pass.frag.glsl"
	"${SHADER_DIR}/ambient-pass.frag.glsl"
	"${SHADER_DIR}/gbuffer-pass.frag.glsl"
	"${SHADER_DIR}/gbuffer-pass.vert.glsl"
	"${SHADER_DIR}/light-pass.frag.glsl"
	"${SHADER_DIR}/simple.vert.glsl"
)

#foreach(FILE ${SHADER_FILES})
#  add_custom_command(
#    OUTPUT ${FILE}
#    COMMAND m4 --prefix-builtins -I${SHADER_DIR} "${FILE}.m4" > ${FILE}
#    DEPENDS "${FILE}.m4"
#    WORKING_DIRECTORY "${SHADER_DIR}"
#  )
#endforeach()

add_custom_target(shaders ALL
  DEPENDS ${SHADER_FILES})


add_subdirectory("test")
