//
// Created by Tristan Brault on 2023-11-22.
//

#pragma once

#include "window.hpp"
#include "game_object.hpp"
#include "renderer.hpp"
#include "descriptors.hpp"
#include "scene.hpp"

#include <memory>
#include <vector>


namespace te {
    class FirstApp {
    public:
        static const int WIDTH = 1200;
        static const int HEIGHT = 800;

        FirstApp(std::unique_ptr<Scene> scene);
        ~FirstApp();

        FirstApp(const FirstApp&) = delete;
        FirstApp &operator=(const FirstApp&) = delete;

        void run();
    private:
        void loadScene(std::unique_ptr<Scene> scene);

        Window window{WIDTH, HEIGHT, "Vulkan Test"};
        Device device{window};
        Renderer renderer{window, device};

        std::unique_ptr<DescriptorPool> globalPool{};
        std::vector<std::unique_ptr<DescriptorPool>> framePools;
        GameObjectManager gameObjectManager{device};
     };
}



