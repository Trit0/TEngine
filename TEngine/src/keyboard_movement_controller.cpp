//
// Created by Tristan Brault on 2023-12-05.
//

#include "keyboard_movement_controller.hpp"

namespace te {

    void KeyboardMovementController::rotate(GLFWwindow* window, float deltaTime, GameObject& gameObject) {
        glm::vec3 rotate{0};
        if (glfwGetKey(window, keys.lookRight) == GLFW_PRESS) {
            rotate.y += 1.f;
        }
        if (glfwGetKey(window, keys.lookLeft) == GLFW_PRESS) {
            rotate.y -= 1.f;
        }
        if (glfwGetKey(window, keys.lookUp) == GLFW_PRESS) {
            rotate.x += 1.f;
        }
        if (glfwGetKey(window, keys.lookDown) == GLFW_PRESS) {
            rotate.x -= 1.f;
        }

        if(glm::dot(rotate, rotate) > std::numeric_limits<float>::epsilon()) {
            gameObject.transform.rotation += lookSpeed * deltaTime * glm::normalize(rotate);
        }

        gameObject.transform.rotation.x = glm::clamp(gameObject.transform.rotation.x, -1.5f, 1.5f);
        gameObject.transform.rotation.y = glm::mod(gameObject.transform.rotation.y, glm::two_pi<float>());
    }

    void KeyboardMovementController::move(GLFWwindow* window, float deltaTime, GameObject& gameObject, glm::vec3 forwardDir) const {
        const glm::vec3 rightDir{forwardDir.z, 0.f, -forwardDir.x};
        const glm::vec3 upDir{0.f, -1.f, 0.f};

        float tmpMoveSpeed = moveSpeed;
        if (glfwGetKey(window, keys.leftShift) == GLFW_PRESS) {
            tmpMoveSpeed *= 5;
        }

        glm::vec3 moveDir{0.f};
        if (glfwGetKey(window, keys.moveForward) == GLFW_PRESS) {
            moveDir += forwardDir;
        }
        if (glfwGetKey(window, keys.moveBackward) == GLFW_PRESS) {
            moveDir -= forwardDir;
        }
        if (glfwGetKey(window, keys.moveRight) == GLFW_PRESS) {
            moveDir += rightDir;
        }
        if (glfwGetKey(window, keys.moveLeft) == GLFW_PRESS) {
            moveDir -= rightDir;
        }
        if (glfwGetKey(window, keys.moveUp) == GLFW_PRESS) {
            moveDir += upDir;
        }
        if (glfwGetKey(window, keys.moveDown) == GLFW_PRESS) {
            moveDir -= upDir;
        }

        if(glm::dot(moveDir, moveDir) > std::numeric_limits<float>::epsilon()) {
            gameObject.transform.translation += tmpMoveSpeed * deltaTime * glm::normalize(moveDir);
        }
    }


    void KeyboardMovementController::moveFree(GLFWwindow* window, float deltaTime, GameObject&gameObject) {
        rotate(window, deltaTime, gameObject);
        float yaw = gameObject.transform.rotation.y;

        const glm::vec3 forwardDir{sin(yaw), -sin(gameObject.transform.rotation.x), cos(yaw)};

        move(window, deltaTime, gameObject, forwardDir);
    }

    void KeyboardMovementController::moveInPlaneXZ(GLFWwindow* window, float deltaTime, GameObject& gameObject) {
        rotate(window, deltaTime, gameObject);
        float yaw = gameObject.transform.rotation.y;

        const glm::vec3 forwardDir{sin(yaw), 0.f, cos(yaw)};

        move(window, deltaTime, gameObject, forwardDir);
    }

}
