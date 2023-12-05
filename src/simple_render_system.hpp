//
// Created by Tristan Brault on 2023-11-22.
//

#pragma once

#include "window.hpp"
#include "pipeline.hpp"
#include "game_object.hpp"
#include "camera.hpp"

#include <vector>


namespace te {
    class SimpleRenderSystem {
    public:
        SimpleRenderSystem(Device& device, VkRenderPass renderPass);
        ~SimpleRenderSystem();

        SimpleRenderSystem(const SimpleRenderSystem&) = delete;
        SimpleRenderSystem &operator=(const SimpleRenderSystem&) = delete;

        void renderGameObjects(VkCommandBuffer commandBuffer, std::vector<GameObject>& gameObjects, const Camera& camera);
    private:
        void createPipelineLayout();
        void createPipeline(VkRenderPass renderPass);



        Device& device;

        std::unique_ptr<Pipeline> pipeline;
        VkPipelineLayout pipelineLayout;
    };
}



