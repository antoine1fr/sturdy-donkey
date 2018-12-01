#pragma once

#include "render/DeferredRenderer.hpp"
#include "render/Window.hpp"
#include "Scene.hpp"

class IResourceLoaderDelegate
{
  public:
    virtual void load(
        render::Window& window,
        Scene& scene,
        render::DeferredRenderer& renderer) = 0;
};
