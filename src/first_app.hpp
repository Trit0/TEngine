//
// Created by Tristan Brault on 2023-11-22.
//

#pragma once
#include "window.hpp"
#include "pipeline.h"


namespace te {
    class FirstApp {
    public:
        static const int WIDTH = 800;
        static const int HEIGHT = 600;

        void run();
    private:
        Window window{WIDTH, HEIGHT, "Test"};
        Device device{window};
        Pipeline pipeline{device, "../src/shaders/simple_shader.vert.spv", "../src/shaders/simple_shader.frag.spv", Pipeline::defaultPipelineConfigInfo(WIDTH, HEIGHT)};
    };
}



