//
// Created by Tristan Brault on 2023-12-05.
//

#pragma once

#include "game_object.hpp"
#include "window.hpp"

namespace te {
    class KeyboardMovementController {
    public:
        struct KeyMappings {
            int moveLeft = GLFW_KEY_A;
            int moveRight = GLFW_KEY_D;
            int moveForward = GLFW_KEY_W;
            int moveBackward = GLFW_KEY_S;
            int moveUp = GLFW_KEY_E;
            int moveDown = GLFW_KEY_Q;
            int lookLeft = GLFW_KEY_LEFT;
            int lookRight = GLFW_KEY_RIGHT;
            int lookUp = GLFW_KEY_UP;
            int lookDown = GLFW_KEY_DOWN;
        };

        void moveInPlaneXZ(GLFWwindow* window, float deltaTime, GameObject& gameObject);
        void moveFree(GLFWwindow* window, float deltaTime, GameObject& gameObject);

        KeyMappings keys{};
        float moveSpeed{3.f};
        float lookSpeed{1.5f};

    private:
        void rotate(GLFWwindow* window, float deltaTime, GameObject& gameObject);
        void move(GLFWwindow* window, float deltaTime, GameObject& gameObject, glm::vec3 forwardDir) const;
    };
}
