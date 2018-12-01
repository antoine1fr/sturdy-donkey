#pragma once

#include <IResourceLoaderDelegate.hpp>

class ResourceLoaderDelegate: public IResourceLoaderDelegate
{
  public:
    virtual void load(
        render::Window& window,
        Scene& scene,
        render::DeferredRenderer& renderer);
};
