#include "first_app.hpp"

#include "camera.hpp"
#include "systems/simple_render_system.hpp"
#include "systems/point_light_system.hpp"
#include "keyboard_movement_controller.hpp"
#include "buffer.hpp"
#include "texture.hpp"

#include <chrono>
#include <iostream>

namespace te {
    FirstApp::FirstApp() {
        globalPool = DescriptorPool::Builder(device)
        .setMaxSets(SwapChain::MAX_FRAMES_IN_FLIGHT)
        .addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, SwapChain::MAX_FRAMES_IN_FLIGHT)
        .build();

        // build frame descriptor pools
        framePools.resize(SwapChain::MAX_FRAMES_IN_FLIGHT);
        auto framePoolBuilder = DescriptorPool::Builder(device)
                .setMaxSets(1000)
                .addPoolSize(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000)
                .addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000)
                .setPoolFlags(VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT);
        for (int i = 0; i < framePools.size(); i++) {
            framePools[i] = framePoolBuilder.build();
        }

        loadGameObjects();
    }

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

        auto globalSetLayout =
        DescriptorSetLayout::Builder(device)
          .addBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL_GRAPHICS)
          .build();

        std::vector<VkDescriptorSet> globalDescriptorSets(SwapChain::MAX_FRAMES_IN_FLIGHT);
        for (int i = 0; i < globalDescriptorSets.size(); i++) {
            auto bufferInfo = uboBuffers[i]->descriptorInfo();
            DescriptorWriter(*globalSetLayout, *globalPool)
                .writeBuffer(0, &bufferInfo)
                .build(globalDescriptorSets[i]);
        }

        std::cout << "Alignment: " << device.properties.limits.minUniformBufferOffsetAlignment << "\n";
        std::cout << "Atom size: " << device.properties.limits.nonCoherentAtomSize << "\n";

        SimpleRenderSystem simpleRenderSystem{
            device,
            renderer.getSwapChainRenderPass(),
            globalSetLayout->getDescriptorSetLayout()};

        PointLightSystem pointLightSystem{
            device,
            renderer.getSwapChainRenderPass(),
            globalSetLayout->getDescriptorSetLayout()};

        Camera camera{};
        // camera.setViewDirection(glm::vec3{0.f}, glm::vec3{.5f, 0.f, 1.f});
        camera.setViewTarget(glm::vec3{-1.f, -2.f, 2.f}, glm::vec3{0.f, 0.f, 2.5f});

        auto& viewerObject = gameObjectManager.createGameObject();
        viewerObject.transform.translation.z = -2.5f;
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
            camera.setPerspectiveProjection(glm::radians(50.f), aspect, .1f, 100.f);

            if (auto commandBuffer = renderer.beginFrame()) {
                int frameIndex = renderer.getFrameIndex();
                framePools[frameIndex]->resetPool();
                FrameInfo frameInfo{frameIndex, frameTime, commandBuffer, camera, globalDescriptorSets[frameIndex], *framePools[frameIndex], gameObjectManager.gameObjects};

                //update
                GlobalUbo ubo{};
                ubo.projection = camera.getProjectionMatrix();
                ubo.view = camera.getViewMatrix();
                ubo.inverseView = camera.getInverseViewMatrix();
                pointLightSystem.update(frameInfo, ubo);
                uboBuffers[frameIndex]->writeToBuffer(&ubo);
                uboBuffers[frameIndex]->flush();

                // final step of update is updating the game objects buffer data
                // The render functions MUST not change a game objects transform data
                gameObjectManager.updateBuffer(frameIndex);

                // render
                renderer.beginSwapChainRenderPass(commandBuffer);

                //order here matters
                simpleRenderSystem.renderGameObjects(frameInfo);
                pointLightSystem.render(frameInfo);

                renderer.endSwapChainRenderPass(commandBuffer);
                renderer.endFrame();
            }
        }

        vkDeviceWaitIdle(device.device());
    }

    void FirstApp::loadGameObjects() {
        std::shared_ptr<Model> model = Model::createModelFromFile(device, "../models/flat_vase.obj");
        auto& flatVase = gameObjectManager.createGameObject();
        flatVase.model = model;
        flatVase.transform.translation = {0.5f, 0.5f, 0.f};
        flatVase.transform.scale = glm::vec3{3.0f, 1.5f, 3.0f};

        model = Model::createModelFromFile(device, "../models/smooth_vase.obj");
        auto& smoothVase = gameObjectManager.createGameObject();
        smoothVase.model = model;
        smoothVase.transform.translation = {-0.5f, .5f, 0.f};
        smoothVase.transform.scale = glm::vec3{3.0f, 1.5f, 3.0f};

        model = Model::createModelFromFile(device, "../models/quad.obj");
        std::shared_ptr<Texture> marbleTexture = Texture::createTextureFromFile(device, "../textures/missing.png");
        auto& floor = gameObjectManager.createGameObject();
        floor.model = model;
        floor.diffuseMap = marbleTexture;
        floor.transform.translation = {0.f, .5f, 0.f};
        floor.transform.scale = glm::vec3{3.0f, 1.f, 3.0f};

        std::vector<glm::vec3> lightColors{
          {1.f, .1f, .1f},
          {.1f, .1f, 1.f},
          {.1f, 1.f, .1f},
          {1.f, 1.f, .1f},
          {.1f, 1.f, 1.f},
          {1.f, 1.f, 1.f}  //
        };

        for (int i = 0; i < lightColors.size(); i++) {
            auto& pointLight = gameObjectManager.makePointLight(0.2f);
            pointLight.color = lightColors[i];
            auto rotateLight = glm::rotate(glm::mat4(1.f), (i * glm::two_pi<float>() / lightColors.size()), {0.f, -1.f, 0.f});
            pointLight.transform.translation = glm::vec3(rotateLight * glm::vec4(-1.f, -1.f, -1.f, 1.f));
        }
    }
} // namespace
