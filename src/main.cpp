#include <GL/gl3w.h>
#include <SDL.h>
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
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
  SDL_GL_CreateContext(window);
  gl3wInit();
  return window;
}

int main()
{
  SDL_Window* window = setup_video();
  bool run = true;
  SDL_Event event;

  while (run)
  {
    while (SDL_PollEvent(&event))
    {
      if (event.type == SDL_QUIT)
        run = false;
    }

    glClearColor(0.0f, 1.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    SDL_GL_SwapWindow(window);
  }
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}
