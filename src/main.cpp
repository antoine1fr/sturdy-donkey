#include <GL/gl3w.h>
#include <SDL.h>
#include <SDL_image.h>
#include <chrono>
#include <fstream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <iostream>
#include <string>

#define CHECK_GL_ERROR assert(glGetError() == GL_NO_ERROR)

GLuint build_shader(GLenum type, const std::string& sources)
{
  GLuint shader = glCreateShader(type);
  CHECK_GL_ERROR;
  const char* source_ptr = sources.c_str();
  glShaderSource(shader, 1, &source_ptr, nullptr);
  CHECK_GL_ERROR;
  glCompileShader(shader);
  CHECK_GL_ERROR;
  char info_log[1000];
  GLsizei log_length;
  glGetShaderInfoLog(shader, sizeof(info_log), &log_length, info_log);
  CHECK_GL_ERROR;
  if (log_length > 0)
    std::cout << "Shader build log: " << info_log << "\n";
  else
    std::cout << "Shader built.\n";
  return shader;
}

GLuint build_vertex_shader(const std::string& sources)
{
  return build_shader(GL_VERTEX_SHADER, sources);
}

GLuint build_fragment_shader(const std::string& sources)
{
  return build_shader(GL_FRAGMENT_SHADER, sources);
}

template <GLenum type>
GLuint load_shader(const std::string& path)
{
  std::ifstream stream(path);
  stream.seekg(0, std::ios_base::end);
  size_t length = stream.tellg();
  stream.seekg(0, std::ios_base::beg);
  std::string sources(length, '\0');
  stream.read(&sources[0], length);
  return build_shader(type, sources);
}

GLuint link_gpu_program(GLuint vertex_shader, GLuint fragment_shader)
{
  GLuint gpu_program = glCreateProgram();
  CHECK_GL_ERROR;
  glAttachShader(gpu_program, vertex_shader);
  CHECK_GL_ERROR;
  glAttachShader(gpu_program, fragment_shader);
  CHECK_GL_ERROR;
  glLinkProgram(gpu_program);
  CHECK_GL_ERROR;
  char info_log[1000];
  GLsizei log_length;
  glGetProgramInfoLog(gpu_program, sizeof(info_log), &log_length, info_log);
  CHECK_GL_ERROR;
  if (log_length > 0)
    std::cout << "GPU program link log: " << info_log << "\n";
  else
    std::cout << "GPU program linked.\n";
  return gpu_program;
}

GLuint build_gpu_program(const std::string& vs_path,
    const std::string& fs_path)
{
  GLuint vertex_shader = load_shader<GL_VERTEX_SHADER>(vs_path);
  GLuint fragment_shader = load_shader<GL_FRAGMENT_SHADER>(fs_path);
  return link_gpu_program(vertex_shader, fragment_shader);
}

SDL_Window* setup_video()
{
  SDL_Window* window = SDL_CreateWindow("Pipelined rendering demo", 0, 0, 800,
    720, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS,
    SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
    SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
  SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
  SDL_GL_CreateContext(window);
  gl3wInit();
  return window;
}

GLenum sdl_to_gl_pixel_format(SDL_PixelFormat* format)
{
  uint32_t r_mask = format->Rmask;
  uint8_t bpp = format->BitsPerPixel;

  if (bpp == 32)
  {
    if (r_mask == 0x0000ff00)
      return GL_RGBA;
    else if (r_mask == 0xff000000)
      return GL_BGRA;
    std::cerr << "Sorry, we only support RGB, RGBA, BGR and BGRA images.\n";
    assert(false);
  }
  if (r_mask == 0x00ff0000)
    return GL_BGR;
  else if (r_mask == 0x000000ff)
    return GL_RGB;
  std::cerr << "Sorry, we only support RGB, RGBA, BGR and BGRA images.\n";
  assert(false);
}

GLenum sdl_to_gl_pixel_type(SDL_PixelFormat* format)
{
  switch (format->BitsPerPixel)
  {
    case 24:
    case 32:
      return GL_UNSIGNED_BYTE;
    default:
      std::cerr << "Sorry, we only support 24- and 32-bit images.";
      assert(false);
  }
}

GLuint load_texture(const std::string& path)
{
  SDL_Surface* img_surface = IMG_Load(path.c_str());
  GLuint texture;
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  GLenum format = sdl_to_gl_pixel_format(img_surface->format);
  GLenum type = sdl_to_gl_pixel_type(img_surface->format);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img_surface->w, img_surface->h,
      0, format, type, nullptr);
  glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, img_surface->w, img_surface->h,
      format, type, img_surface->pixels);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  SDL_FreeSurface(img_surface);
  return texture;
}

int main()
{
  SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
  IMG_Init(IMG_INIT_PNG);
  SDL_Window* window = setup_video();
  bool run = true;
  SDL_Event event;
  int width, height;
  SDL_GL_GetDrawableSize(window, &width, &height);
  float ratio = static_cast<float>(width) / static_cast<float>(height);
  glm::mat4 projection = glm::perspective(glm::radians(45.0f),
      ratio, 0.1f, 100.0f);
  glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f),
      glm::vec3(0.0f, 0.0f, 0.0f),
      glm::vec3(0.0f, 1.0f, 0.0f));
  glm::mat4 model = glm::mat4(1.0f);
  glm::mat4 light_model = glm::mat4(1.0f);
  float positions[] = {
    -0.5f, -0.5f, 0.0f,
    -0.5f, 0.5f, 0.0f,
    0.5f, 0.5f, 0.0f,
    0.5f, -0.5f, 0.0f
  };
  float uvs[] = {
    0.0f, 0.0f,
    0.0f, 1.0f,
    1.0f, 1.0f,
    1.0f, 0.0f
  };
  uint8_t indices[] = {0, 1, 2, 0, 2, 3};
  GLuint diffuse_map = load_texture("textures/quad-diffuse.png");
  GLuint normal_map = load_texture("textures/quad-normal.png");

  GLuint gpu_program = build_gpu_program("shaders/simple-vs.glsl",
      "shaders/simple-fs.glsl");
  glUseProgram(gpu_program);
  GLint position_location = glGetAttribLocation(gpu_program, "position");
  GLint uv_location = glGetAttribLocation(gpu_program, "vertex_uv");
  GLint model_location = glGetUniformLocation(gpu_program, "model");
  GLint light_model_location =
    glGetUniformLocation(gpu_program, "light_model");
  GLint projection_location = glGetUniformLocation(gpu_program, "projection");
  GLint view_location = glGetUniformLocation(gpu_program, "view");
  GLint diffuse_map_location = glGetUniformLocation(gpu_program,
      "diffuse_map");
  GLint normal_map_location = glGetUniformLocation(gpu_program,
      "normal_map");

  GLuint vertex_array;
  glGenVertexArrays(1, &vertex_array);
  glBindVertexArray(vertex_array);

  GLuint position_buffer;
  glGenBuffers(1, &position_buffer);
  glBindBuffer(GL_ARRAY_BUFFER, position_buffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(positions), nullptr, GL_STATIC_DRAW);
  glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(positions), positions);
  glVertexAttribPointer(position_location, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
  glEnableVertexAttribArray(position_location);

  GLuint uv_buffer;
  glGenBuffers(1, &uv_buffer);
  glBindBuffer(GL_ARRAY_BUFFER, uv_buffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(uvs), nullptr, GL_STATIC_DRAW);
  glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(uvs), uvs);
  glVertexAttribPointer(uv_location, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
  glEnableVertexAttribArray(uv_location);

  GLuint index_buffer;
  glGenBuffers(1, &index_buffer);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
      GL_STATIC_DRAW);

  auto last_time = std::chrono::high_resolution_clock::now();

  while (run)
  {
    auto time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> delta_time = time - last_time;
    last_time = time;
    light_model = glm::rotate(light_model,
        delta_time.count() * glm::radians(30.f),
        glm::vec3(0.0f, 0.0f, 1.0f));

    while (SDL_PollEvent(&event))
    {
      if (event.type == SDL_QUIT)
        run = false;
    }

    glClearColor(0.0f, 1.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // setup everything
    glUniform1i(diffuse_map_location, 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, diffuse_map);
    glUniform1i(normal_map_location, 1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, normal_map);
    glUseProgram(gpu_program);
    glUniformMatrix4fv(light_model_location, 1, GL_FALSE,
        &(light_model[0][0]));
    glUniformMatrix4fv(model_location, 1, GL_FALSE, &(model[0][0]));
    glUniformMatrix4fv(view_location, 1, GL_FALSE, &(view[0][0]));
    glUniformMatrix4fv(projection_location, 1, GL_FALSE, &(projection[0][0]));

    // draw
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, nullptr);
    SDL_GL_SwapWindow(window);
  }
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}
