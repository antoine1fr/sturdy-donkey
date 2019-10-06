/* Copyright (C) 2018 Antoine Luciani
 *
 * This file is part of Sturdy Donkey.
 *
 * Sturdy Donkey is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation, version 3.
 *
 * Sturdy Donkey is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * Sturdy Donkey. If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

#include <array>
#include <thread>
#include <condition_variable>
#include <atomic>
#include <SDL.h>
#include <limits>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include "IResourceLoaderDelegate.hpp"
#include "render/gl/Driver.hpp"
#include "render/FramePacket.hpp"
#include "render/AResourceManager.hpp"
#include "render/ResourceManager.hpp"
#include "render/RenderPass.hpp"
#include "render/Window.hpp"
#include "StackVector.hpp"

namespace donkey {
namespace render {


class DeferredRenderer
{
  private:
    std::vector<RenderPass> render_passes_;
    Window* window_;
    SDL_GLContext render_context_;
    gl::Driver* driver_;
    AResourceManager& gpu_resource_manager_;
    ResourceManager* resource_manager_;

    typedef std::list<StackFramePacket> FramePacketList;
    std::list<StackFramePacket> frame_packets_;

    std::thread* render_thread_;
    uint32_t light_program_id_;
    uint32_t albedo_program_id_;
    uint32_t ambient_program_id_;
    uint32_t light_rt_id_;
    uint32_t albedo_rt_id_;
    uint32_t normal_rt_id_;
    uint32_t depth_rt_id_;
    uint32_t light_plus_albedo_rt_id_;
    uint32_t gbuffer_id_;
    uint32_t light_framebuffer_id_;
    uint32_t albedo_framebuffer_id_;
    uint32_t screen_mesh_id_;

  private:
    void bind_light_uniforms_(
        CommandBucket& render_commands,
        const Material& material,
        const glm::mat4& view,
        const DirectionalLightNode* light_node) const;
    void bind_camera_uniforms_(
        CommandBucket& render_commands,
        const Material& material,
        const CameraNode& camera_node) const;
    void bind_mesh_uniforms_(
        CommandBucket& render_commands,
        const Material& material,
        const MeshNode& mesh_node) const;
    void create_light_pass_mesh_(int width, int height);
    void create_gbuffer_(int width, int height);
    void create_light_accu_render_target_(int width, int height);
    void create_albedo_render_target_(int width, int height);
    uint32_t create_light_material_(
        const std::string& vertex_shader_path,
        const std::string& fragment_shader_path);
    uint32_t create_ambient_material_(
        const std::string& vertex_shader_path,
        const std::string& fragment_shader_path);
    uint32_t create_albedo_material_(
        const std::string& vertex_shader_path,
        const std::string& fragment_shader_path);
    void create_light_accu_pass_frame_packet_(int width, int height);
    void create_albedo_pass_frame_packet_(int width, int height);
    void create_ambient_pass_frame_packet_(int width, int height);
    void render_geometry_(
        size_t pass_num,
        const RenderPass& render_pass,
        const StackVector<MeshNode>& mesh_nodes,
        const CameraNode& camera_node,
        const CameraNode* last_camera_node,
        const StackVector<DirectionalLightNode>& light_nodes,
        CommandBucket& render_commands,
        ResourceManager* resource_manager,
        AResourceManager* gpu_resource_manager);
    void execute_pass_(
        size_t pass_num,
        const RenderPass& render_pass,
        const StackFramePacket& frame_packet,
        const CameraNode* last_camera_node,
        const StackVector<DirectionalLightNode>& light_nodes,
        CommandBucket& render_commands,
        ResourceManager* resource_manager,
        AResourceManager* gpu_resource_manager);

  public:
    DeferredRenderer(
        Window* window,
        gl::Driver* driver,
        ResourceManager* resource_manager);
    ~DeferredRenderer();
    void render(StackFramePacket* frame_packet,
        CommandBucket& render_commands);
    void add_render_pass(const RenderPass& render_pass);
    void add_render_pass(RenderPass&& render_pass);
    uint32_t get_albedo_rt_id() const;
    uint32_t get_normal_rt_id() const;
    uint32_t get_depth_rt_id() const;
};

}
}
