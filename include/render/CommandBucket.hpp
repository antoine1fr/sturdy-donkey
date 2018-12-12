#pragma once

#include <list>

#include "render/Mesh.hpp"

namespace render
{

class Command
{
  public:
    enum class Type
    {
      kBindMesh = 0,
      kDrawElements
    };

    Type type;

  protected:
    Command(Type type);
};

struct BindMeshCommand: Command
{
  BindMeshCommand(const Mesh& mesh, int position_location, int uv_location);
  const Mesh& mesh;
  int position_location;
  int uv_location;
};

struct DrawElementsCommand: Command
{
  DrawElementsCommand(size_t count, GLenum element_type);
  size_t count;
  GLenum element_type;
};

struct SortedCommand
{
  uint64_t sort_key;
  Command& command;
};

class CommandBucket
{
  private:
    std::list<SortedCommand> sorted_commands_;
    std::list<BindMeshCommand> bind_mesh_commands_;
    std::list<DrawElementsCommand> draw_elements_commands_;

  public:
    void bind_mesh(const Mesh& mesh, int position_location, int uv_location);
    void draw_elements(size_t count, GLenum element_type);
    const std::list<SortedCommand>& get_commands() const;
};

}
