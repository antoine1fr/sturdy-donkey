#pragma once

#include <string>
#include <tiny_obj_loader.h>
#include <IResourceLoaderDelegate.hpp>
#include <render/ResourceManager.hpp>

class ResourceLoaderDelegate: public donkey::IResourceLoaderDelegate
{
  public:
    virtual void load(
        donkey::render::Window& window,
        donkey::Scene& scene,
        donkey::render::DeferredRenderer& renderer);

  private:
    uint32_t load_mesh_(donkey::render::ResourceManager& resource_manager,
        const std::string& path) const;
    void consolidate_indices_(
        const tinyobj::attrib_t& attributes,
        const std::vector<tinyobj::index_t>& tinyobj_indices,
        std::vector<uint32_t>& indices,
        std::vector<float>& positions,
        std::vector<float>& uvs) const;
};
