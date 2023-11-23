//
// Created by Tristan Brault on 2023-11-22.
//

#pragma once
#include "window.hpp"


namespace te {
    class FirstApp {
    public:
        static const int WIDTH = 800;
        static const int HEIGHT = 600;

        void run();
    private:
        Window window = Window(WIDTH, HEIGHT, "Hello Vulkan");
    };
}



