//
// Created by Tristan Brault on 2023-11-22.
//

#pragma once

#include "pipeline.hpp"
#include "game_object.hpp"
#include "frame_info.hpp"
#include "ecs_core/system.hpp"

#include <vector>


namespace te {
    class SimpleRenderSystem : public System {
    public:
        SimpleRenderSystem(Device& device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout);
        ~SimpleRenderSystem();

        SimpleRenderSystem(const SimpleRenderSystem&) = delete;
        SimpleRenderSystem &operator=(const SimpleRenderSystem&) = delete;

        void renderGameObjects(FrameInfo& frameInfo, std::set<Entity>& entities);
    private:
        void createPipelineLayout(VkDescriptorSetLayout globalSetLayout);
        void createPipeline(VkRenderPass renderPass);



        Device& device;

        std::unique_ptr<Pipeline> pipeline;
        VkPipelineLayout pipelineLayout;
    };
}



