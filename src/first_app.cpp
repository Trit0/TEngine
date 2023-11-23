//
// Created by Tristan Brault on 2023-11-22.
//

#include "first_app.hpp"

namespace te {
    void FirstApp::run() {
        while(!window.shouldClose()) {
            glfwPollEvents();
        }
    }

}
