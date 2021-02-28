add_library(sturdy-donkey STATIC
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
  src/render/Mesh.cpp
  src/render/ResourceManager.cpp
  src/render/TextureMaterialSlot.cpp
  src/render/Window.cpp
  src/render/gl/Driver.cpp
  src/render/gl/Material.cpp
  src/render/gl/Mesh.cpp
  src/render/gl/ResourceManager.cpp
  src/render/gl/State.cpp
  src/render/gl/Texture.cpp
 "src/render/core_renderer.cpp"
 "src/GBufferPass.cpp")

# Platform-specific source files:
if(MSVC)
  target_sources(sturdy-donkey PRIVATE
    "src/WindowsPageAllocator.cpp")
else()
  target_sources(sturdy-donkey PRIVATE
	"src/UnixPageAllocator.cpp")
endif()

if(MSVC)
	# Don't bother with /Wall on MSVC since it's incompatible with system headers.
	# Also their magical /external: switch family doesn't seem to work anymore.
	# Sad times.
else()
	target_compile_options(sturdy-donkey PRIVATE -Werror -Wall -pedantic)
endif()
target_compile_features(sturdy-donkey PRIVATE cxx_std_17)
set_target_properties(sturdy-donkey PROPERTIES CXX_EXTENSIONS OFF)

target_include_directories(sturdy-donkey
  PUBLIC
  $<INSTALL_INTERFACE:include>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

target_link_libraries(sturdy-donkey
  SDL2::SDL2
  SDL2_image::SDL2_image
  gl3w::gl3w
  glm::glm
  tinyobjloader
  imgui
)
