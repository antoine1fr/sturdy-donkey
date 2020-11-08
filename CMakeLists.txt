cmake_minimum_required(VERSION 3.12)
project(sturdy-donkey VERSION 0.4.0 LANGUAGES C CXX)

# Search for dependencies in submodules when falling back to config mode.
list(INSERT CMAKE_PREFIX_PATH 0 ${PROJECT_SOURCE_DIR})
list(INSERT CMAKE_MODULE_PATH 0 ${PROJECT_SOURCE_DIR}/cmake)

find_package(glm REQUIRED MODULE)
find_package(SDL2 REQUIRED MODULE)
find_package(SDL2_image REQUIRED MODULE)

add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/lib/gl3w")
add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/lib/tinyobjloader")

add_library(sturdy-donkey SHARED
  src/Buffer.cpp
  src/BufferPool.cpp
  src/Game.cpp
  src/GameManager.cpp
  src/MeshLoader.cpp
  src/Scene.cpp
  src/StackAllocator.cpp
  src/render/AMaterial.cpp
  src/render/AResourceManager.cpp
  src/render/CommandBucket.cpp
  src/render/DeferredRenderer.cpp
  src/render/Mesh.cpp
  src/render/RenderPass.cpp
  src/render/ResourceManager.cpp
  src/render/TextureMaterialSlot.cpp
  src/render/Window.cpp
  src/render/gl/Driver.cpp
  src/render/gl/Material.cpp
  src/render/gl/Mesh.cpp
  src/render/gl/ResourceManager.cpp
  src/render/gl/State.cpp
  src/render/gl/Texture.cpp
)
target_compile_options(sturdy-donkey PRIVATE -Werror -Wall -pedantic)
target_compile_features(sturdy-donkey PRIVATE cxx_std_17)
set_target_properties(sturdy-donkey PROPERTIES CXX_EXTENSIONS OFF)

target_include_directories(sturdy-donkey
  PUBLIC
  $<INSTALL_INTERFACE:include>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

target_link_libraries(sturdy-donkey
  PUBLIC
  SDL2::SDL2
  SDL2_image::SDL2_image
  gl3w
  glm::glm
  tinyobjloader
)

set(SHADER_DIR "${CMAKE_CURRENT_LIST_DIR}/shaders")
set(SHADER_FILES
  "${SHADER_DIR}/albedo-pass.frag.glsl"
	"${SHADER_DIR}/ambient-pass.frag.glsl"
	"${SHADER_DIR}/gbuffer-pass.frag.glsl"
	"${SHADER_DIR}/gbuffer-pass.vert.glsl"
	"${SHADER_DIR}/light-pass.frag.glsl"
	"${SHADER_DIR}/simple.vert.glsl"
)

foreach(FILE ${SHADER_FILES})
  add_custom_command(
    OUTPUT ${FILE}
    COMMAND m4 --prefix-builtins -I${SHADER_DIR} "${FILE}.m4" > ${FILE}
    DEPENDS "${FILE}.m4"
    WORKING_DIRECTORY "${SHADER_DIR}"
  )
endforeach()

add_custom_target(shaders ALL
  DEPENDS ${SHADER_FILES})


add_subdirectory("test")
