//
// Created by Tristan Brault on 2023-11-22.
//

#pragma once

#include "window.hpp"
#include "game_object.hpp"
#include "renderer.hpp"
#include "descriptors.hpp"

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
        void loadGameObjects();


        Window window{WIDTH, HEIGHT, "Vulkan Test"};
        Device device{window};
        Renderer renderer{window, device};

        std::unique_ptr<DescriptorPool> globalPool{};
        GameObject::Map gameObjects;
     };
}



