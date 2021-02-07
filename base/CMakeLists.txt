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
 "src/render/PipelineGenerator.cpp")

# Platform-specific source files:
if(MSVC)
  target_sources(sturdy-donkey PRIVATE
    "src/WindowsPageAllocator.cpp")
else()
  target_sources(sturdy-donkey PRIVATE
	"src/UnixPageAllocator.cpp")
endif()

if(MSVC)
	target_compile_options(sturdy-donkey PRIVATE /Wall)
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
