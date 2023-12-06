#include "first_app.hpp"

#include "camera.hpp"
#include "simple_render_system.hpp"
#include "keyboard_movement_controller.hpp"

#include <chrono>

namespace te {
    FirstApp::FirstApp() { loadGameObjects(); }

    FirstApp::~FirstApp() {
    }

    void FirstApp::run() {
        SimpleRenderSystem simpleRenderSystem{device, renderer.getSwapChainRenderPass()};
        Camera camera{};
        // camera.setViewDirection(glm::vec3{0.f}, glm::vec3{.5f, 0.f, 1.f});
        camera.setViewTarget(glm::vec3{-1.f, -2.f, 2.f}, glm::vec3{0.f, 0.f, 2.5f});

        auto viewerObject = GameObject::createGameObject();
        KeyboardMovementController cameraController{};

        auto currentTime = std::chrono::high_resolution_clock::now();

        while (!window.shouldClose()) {
            glfwPollEvents();

            auto newTime = std::chrono::high_resolution_clock::now();
            float frameTime = std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
            currentTime = newTime;

            cameraController.moveFree(window.getGLFWwindow(), frameTime, viewerObject);
            camera.setViewYXZ(viewerObject.transform.translation, viewerObject.transform.rotation);

            float aspect = renderer.getAspectRatio();
            // camera.setOrthographicProjection(-aspect, aspect, -1, 1, -1, 1);
            camera.setPerspectiveProjection(glm::radians(50.f), aspect, .1f, 10.f);

            if (auto commandBuffer = renderer.beginFrame()) {
                // render system
                renderer.beginSwapChainRenderPass(commandBuffer);
                simpleRenderSystem.renderGameObjects(commandBuffer, gameObjects, camera);
                renderer.endSwapChainRenderPass(commandBuffer);
                renderer.endFrame();
            }
        }

        vkDeviceWaitIdle(device.device());
    }

    void FirstApp::loadGameObjects() {
        std::shared_ptr<Model> model = Model::createModelFromFile(device, "../src/models/smooth_vase.obj");
        auto gameObj = GameObject::createGameObject();
        gameObj.model = model;
        gameObj.transform.translation = {.0f, .0f, 2.5f};
        gameObj.transform.scale = glm::vec3{3.0f};
        gameObjects.push_back(std::move(gameObj));
    }
} // namespace
