#pragma once

#include "common.hpp"
#include "Scene.hpp"
#include "render/DeferredRenderer.hpp"

class Demo
{
  private:
    Scene scene_;
    render::DeferredRenderer renderer_;

  public:
    Demo(int width, int height);
    ~Demo();
    void render() const;
    void update(Duration elapsed_time);
};
