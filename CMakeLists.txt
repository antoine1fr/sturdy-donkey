cmake_minimum_required(VERSION 3.16)
project(sturdy-donkey VERSION 0.4.0 LANGUAGES C CXX)

# Search for dependencies in submodules when falling back to config mode.
list(INSERT CMAKE_PREFIX_PATH 0 ${PROJECT_SOURCE_DIR})
list(INSERT CMAKE_MODULE_PATH 0 ${PROJECT_SOURCE_DIR}/cmake)

find_package(gl3w REQUIRED)
find_package(glm REQUIRED MODULE)
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED MODULE)

add_library(tinyobjloader::tinyobjloader INTERFACE IMPORTED)
set_target_properties(tinyobjloader::tinyobjloader PROPERTIES
  INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_LIST_DIR}/lib/tinyobjloader")
target_sources(tinyobjloader::tinyobjloader
  INTERFACE
  lib/tinyobjloader/tiny_obj_loader.cc
)

add_library(imgui::imgui INTERFACE IMPORTED)
set_target_properties(imgui::imgui PROPERTIES
  INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_LIST_DIR}/lib/imgui")
target_sources(imgui::imgui
  INTERFACE
  lib/imgui/imgui.cpp
  lib/imgui/imgui_draw.cpp
  lib/imgui/imgui_widgets.cpp
)

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
  src/render/DebugHudRenderer.cpp
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
  PRIVATE
  gl3w
  SDL2::SDL2
  SDL2_image::SDL2_image
  glm::glm
  imgui::imgui
  tinyobjloader::tinyobjloader
)
