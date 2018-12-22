#pragma once

#include <list>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat2x2.hpp>
#include <glm/mat3x3.hpp>
#include <glm/mat4x4.hpp>

#include "render/Mesh.hpp"
#include "render/Texture.hpp"

namespace donkey
{
namespace render
{

class Command
{
  public:
    enum class Type
    {
      kBindMesh = 0,
      kDrawElements,
      kBindUniformFloat,
      kBindUniformInt,
      kBindUniformVec2,
      kBindUniformVec3,
      kBindUniformVec4,
      kBindUniformMat2,
      kBindUniformMat3,
      kBindUniformMat4,
      kBindTexture
    };

    Type type;

  protected:
    Command(Type type);
};

struct BindMeshCommand: Command
{
  BindMeshCommand(
      const Mesh& mesh,
      int position_location,
      int uv_location);
  const Mesh& mesh;
  int position_location;
  int uv_location;
};

struct BindUniformFloatCommand: Command
{
  BindUniformFloatCommand(int location, float uniform);
  int location;
  float uniform;
};

struct BindUniformIntCommand: Command
{
  BindUniformIntCommand(int location, int uniform);
  int location;
  int uniform;
};

struct BindUniformVec2Command: Command
{
  BindUniformVec2Command(int location, const glm::vec2& uniform);
  int location;
  glm::vec2 uniform;
};

struct BindUniformVec3Command: Command
{
  BindUniformVec3Command(int location, const glm::vec3& uniform);
  int location;
  glm::vec3 uniform;
};

struct BindUniformVec4Command: Command
{
  BindUniformVec4Command(int location, const glm::vec4& uniform);
  int location;
  glm::vec4 uniform;
};

struct BindUniformMat2Command: Command
{
  BindUniformMat2Command(int location, const glm::mat2& uniform);
  int location;
  glm::mat2 uniform;
};

struct BindUniformMat3Command: Command
{
  BindUniformMat3Command(int location, const glm::mat3& uniform);
  int location;
  glm::mat3 uniform;
};

struct BindUniformMat4Command: Command
{
  BindUniformMat4Command(int location, const glm::mat4& uniform);
  int location;
  glm::mat4 uniform;
};

struct BindTextureCommand: Command
{
  BindTextureCommand(
      int location,
      unsigned texture_unit,
      const Texture& texture);
  int location;
  unsigned int texture_unit;
  const Texture& texture;
};

struct DrawElementsCommand: Command
{
  DrawElementsCommand(size_t count);
  size_t count;
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
    std::list<BindUniformFloatCommand> bind_float_commands_;
    std::list<BindUniformIntCommand> bind_int_commands_;
    std::list<BindUniformVec2Command> bind_vec2_commands_;
    std::list<BindUniformVec3Command> bind_vec3_commands_;
    std::list<BindUniformVec4Command> bind_vec4_commands_;
    std::list<BindUniformMat2Command> bind_mat2_commands_;
    std::list<BindUniformMat3Command> bind_mat3_commands_;
    std::list<BindUniformMat4Command> bind_mat4_commands_;
    std::list<BindTextureCommand> bind_texture_commands_;
    std::list<DrawElementsCommand> draw_elements_commands_;

  private:
    uint64_t make_sort_key_(Command::Type type);

  public:
    void bind_uniform(int location, int uniform);
    void bind_uniform(int location, float uniform);
    void bind_uniform(int location, const glm::vec2& uniform);
    void bind_uniform(int location, const glm::vec3& uniform);
    void bind_uniform(int location, const glm::vec4& uniform);
    void bind_uniform(int location, const glm::mat2& uniform);
    void bind_uniform(int location, const glm::mat3& uniform);
    void bind_uniform(int location, const glm::mat4& uniform);
    void bind_texture(
        int location,
        unsigned int texture_unit,
        const Texture& texture);
    void bind_mesh(
        const Mesh& mesh,
        int position_location,
        int uv_location);
    void draw_elements(size_t count);
    const std::list<SortedCommand>& get_commands() const;
};

}
}
