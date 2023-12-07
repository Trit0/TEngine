#include "first_app.hpp"

#include "camera.hpp"
#include "simple_render_system.hpp"
#include "keyboard_movement_controller.hpp"
#include "buffer.hpp"

#include <chrono>

namespace te {

    struct GlobalUbo {
        glm::mat4 projectionView{1.f};
        glm::vec3 lightDirection = glm::normalize(glm::vec3{1.f, -3.f, -1.f});
    };

    FirstApp::FirstApp() { loadGameObjects(); }

    FirstApp::~FirstApp() {
    }

    void FirstApp::run() {
        std::vector<std::unique_ptr<Buffer>> uboBuffers(SwapChain::MAX_FRAMES_IN_FLIGHT);
        for (int i = 0; i < uboBuffers.size(); i++) {
            uboBuffers[i] = std::make_unique<Buffer>(device,
            sizeof(GlobalUbo),
            1,
            VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);

            uboBuffers[i]->map();
        }

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
                int frameIndex = renderer.getFrameIndex();
                FrameInfo frameInfo{frameIndex, frameTime, commandBuffer, camera};

                //update
                GlobalUbo ubo{};
                ubo.projectionView = camera.getProjectionMatrix() * camera.getViewMatrix();
                uboBuffers[frameIndex]->writeToBuffer(&ubo, frameIndex);
                uboBuffers[frameIndex]->flushIndex(frameIndex);

                // render
                renderer.beginSwapChainRenderPass(commandBuffer);
                simpleRenderSystem.renderGameObjects(frameInfo, gameObjects);
                renderer.endSwapChainRenderPass(commandBuffer);
                renderer.endFrame();
            }
        }

        vkDeviceWaitIdle(device.device());
    }

    void FirstApp::loadGameObjects() {
        std::shared_ptr<Model> flatVaseModel = Model::createModelFromFile(device, "../src/models/colored_cube.obj");
        auto gameObj = GameObject::createGameObject();
        gameObj.model = flatVaseModel;
        gameObj.transform.translation = {1.0f, 0.f, 2.5f};
        gameObj.transform.scale = glm::vec3{0.5f, 0.5f, 0.5f};
        gameObjects.push_back(std::move(gameObj));

        std::shared_ptr<Model> smoothVaseModel = Model::createModelFromFile(device, "../src/models/smooth_vase.obj");
        auto gameObj2 = GameObject::createGameObject();
        gameObj2.model = smoothVaseModel;
        gameObj2.transform.translation = {-0.5f, .5f, 2.5f};
        gameObj2.transform.scale = glm::vec3{3.0f, 1.5f, 3.0f};
        gameObjects.push_back(std::move(gameObj2));
    }
} // namespace
