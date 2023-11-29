//
// Created by Tristan Brault on 2023-11-22.
//

#pragma once
#include "window.hpp"
#include "pipeline.h"
#include "swap_chain.hpp"
#include "model.h"

#include <memory>
#include <vector>


namespace te {
    class FirstApp {
    public:
        static const int WIDTH = 800;
        static const int HEIGHT = 600;

        FirstApp();
        ~FirstApp();

        FirstApp(const FirstApp&) = delete;
        FirstApp &operator=(const FirstApp&) = delete;

        void run();
    private:
        void loadModel();
        void createPipelineLayout();
        void createPipeline();
        void createCommandBuffers();
        void freeCommandBuffers();
        void drawFrame();
        void recreateSwapChain();
        void recordCommandBuffer(int imageIndex);
        void sierpinski(std::vector<Model::Vertex> &vertices, int depth, glm::vec2 left, glm::vec2 right, glm::vec2 top);


        Window window{WIDTH, HEIGHT, "Vulkan Test"};
        Device device{window};
        std::unique_ptr<SwapChain> swapChain;
        std::unique_ptr<Pipeline> pipeline;
        VkPipelineLayout pipelineLayout;
        std::vector<VkCommandBuffer> commandBuffers;
        std::unique_ptr<Model> model;
     };
}



