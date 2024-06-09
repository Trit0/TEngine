#include "first_app.hpp"

#include "camera.hpp"
#include "systems/simple_render_system.hpp"
#include "systems/point_light_system.hpp"
#include "keyboard_movement_controller.hpp"
#include "buffer.hpp"
#include "ecs_core/scene_manager.hpp"
#include "ecs_core/demo/physics.hpp"
#include "resource_manager.hpp"
#include "texture.hpp"
#include "ecs_core/demo/render_system_textured.hpp"

#include <chrono>

namespace te {

    FirstApp::FirstApp(std::unique_ptr<Scene> scene) {
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

        loadScene(std::move(scene));
    }

    FirstApp::~FirstApp() {
    }

    std::shared_ptr<PhysicsSystem> physicsSystem;
    std::shared_ptr<RenderSystemTextured> renderSystemTextured;
    SceneManager gSceneManager;

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

                //updateBuffers
                GlobalUbo ubo{};
                ubo.projection = camera.getProjectionMatrix();
                ubo.view = camera.getViewMatrix();
                ubo.inverseView = camera.getInverseViewMatrix();
                pointLightSystem.update(frameInfo, ubo);
                uboBuffers[frameIndex]->writeToBuffer(&ubo);
                uboBuffers[frameIndex]->flush();

                physicsSystem->update(frameTime);
                std::cout << "Frame time: " << frameTime << std::endl;

                // final step of updateBuffers is updating the game objects buffer data
                // The render functions MUST not change a game objects transform data
                //gameObjectManager.updateBuffer(frameIndex);
                //renderSystem->updateBuffers(frameInfo);
                renderSystemTextured->updateBuffers(frameInfo);

                // render
                renderer.beginSwapChainRenderPass(commandBuffer);

                //order here matters
                renderSystemTextured->render(frameInfo, simpleRenderSystem);
                //renderSystem->render(frameInfo, simpleRenderSystemNormal);
                // simpleRenderSystem.renderGameObjects(frameInfo);
                pointLightSystem.render(frameInfo);

                renderer.endSwapChainRenderPass(commandBuffer);
                renderer.endFrame();
            }
        }

        vkDeviceWaitIdle(device.device());
    }

    void FirstApp::loadScene(std::unique_ptr<Scene> scene) {
//        std::shared_ptr<Model> cubeModel = Model::createModelFromFile(device, "../models/cube.obj");
//        auto flatVase = GameObject::createGameObject();
//        flatVase.model = cubeModel;
//        flatVase.transform.translation = {0.0f, 0.0f, 0.f};
//        flatVase.transform.scale = glm::vec3{0.1f, 0.1f, 0.1f};
//        gameObjects.emplace(flatVase.getId(), std::move(flatVase));

//        cubeModel = Model::createModelFromFile(device, "../models/colored_cube.obj");
//        auto smoothVase = GameObject::createGameObject();
//        smoothVase.model = cubeModel;
//        smoothVase.transform.translation = {-0.5f, .0f, 0.f};
//        smoothVase.transform.scale = glm::vec3{0.1f, 0.1f, 0.1f};
//        gameObjects.emplace(smoothVase.getId(), std::move(smoothVase));

//        std::shared_ptr<Model> model = Model::createModelFromFile(device, "../models/quad.obj");
//        auto floor = GameObject::createGameObject();
//        floor.model = model;
//        floor.transform.translation = {0.f, .5f, 0.f};
//        floor.transform.scale = glm::vec3{3.0f, 1.f, 3.0f};
//        gameObjects.emplace(floor.getId(), std::move(floor));



        std::vector<glm::vec3> lightColors{
          {1.f, .1f, .1f},
          {.1f, .1f, 1.f},
          {.1f, 1.f, .1f},
          {1.f, 1.f, .1f},
          {.1f, 1.f, 1.f},
          {1.f, 1.f, 1.f}  //
        };

        for (int i = 0; i < lightColors.size(); i++) {
            auto& pointLight = gameObjectManager.makePointLight(50.f);
            pointLight.color = lightColors[i];
            auto rotateLight = glm::rotate(glm::mat4(1.f), (i * glm::two_pi<float>() / lightColors.size()), {0.f, -1.f, 0.f});
            pointLight.transform.translation = glm::vec3(rotateLight * glm::vec4(-1.f, -1.f, -1.f, 1.f));
        }

        gSceneManager.init();

        gSceneManager.registerComponent<Gravity>();
        gSceneManager.registerComponent<RigidBody>();
        gSceneManager.registerComponent<TransformComponent>();
        gSceneManager.registerComponent<std::shared_ptr<Model>>();
        gSceneManager.registerComponent<std::shared_ptr<Texture>>();

        physicsSystem = gSceneManager.registerSystem<PhysicsSystem>();
        renderSystemTextured = gSceneManager.registerSystem<RenderSystemTextured>();

        Signature signature;
        signature.set(gSceneManager.getComponentType<TransformComponent>());
        // TODO register only model
        signature.set(gSceneManager.getComponentType<std::shared_ptr<Model>>());
        gSceneManager.setSystemSignature<RenderSystemTextured>(signature);

        signature.set(gSceneManager.getComponentType<Gravity>());
        signature.set(gSceneManager.getComponentType<RigidBody>());
        gSceneManager.setSystemSignature<PhysicsSystem>(signature);

        //signature.set(gSceneManager.getComponentType<std::shared_ptr<Texture>>());

        //gSceneManager.setSystemSignature<RenderSystemTextured>(signature);

        //renderSystem->init(device);
        renderSystemTextured->init(device);

        scene->init(device);

//        auto scenePath = "../scenes/scene.json";
//        simdjson::ondemand::parser parser;
//        simdjson::padded_string json = simdjson::padded_string::load(scenePath);
//        simdjson::ondemand::document content = parser.iterate(json);
//        auto jsonEntities = content["entities"];
//
//        auto modelManager = ResourceManager{};
//
//        for (auto jsonEntity : jsonEntities) {
//            auto entity = gSceneManager.createEntity();
//
//            // Transform
//            auto transform = jsonEntity["transform"];
//            auto transformComponent = TransformComponent::fromJson(transform);
//            std::cout << transformComponent.translation.x << std::endl;
//            gSceneManager.addComponent(entity, transformComponent);
//
//            // Model
//            std::string_view filePath;
//            auto error = jsonEntity["model"].get(filePath);
//            if (!error) {
//                auto model = modelManager.getModel(device, std::string{filePath});
//                gSceneManager.addComponent(entity, model);
//            }
//
//            gSceneManager.addComponent(
//                    entity,
//                    Gravity{glm::vec3(0.0f, -5.f, 0.0f)});
//
//            gSceneManager.addComponent(
//                    entity,
//                    RigidBody{
//                            .velocity = glm::vec3(0.0f, 0.0f, 0.0f),
//                            .acceleration = glm::vec3(0.0f, 0.0f, 0.0f)
//                    });
//
//        }

//        std::vector<Entity> entities(MAX_ENTITIES);
//
//        std::default_random_engine generator;
//        std::uniform_real_distribution<float> randPosition(-50.0f, 150.0f);
//        std::uniform_real_distribution<float> randRotation(0.0f, 3.0f);
//        std::uniform_real_distribution<float> randScale(1.f, 4.0f);
//        std::uniform_real_distribution<float> randGravity(-10.0f, -1.0f);
//
//        float scale = randScale(generator);

//        for (auto& entity : entities)
//        {
//            entity = gSceneManager.createEntity();
//
//            gSceneManager.addComponent(
//                    entity,
//                    Gravity{glm::vec3(0.0f, randGravity(generator), 0.0f)});
//
//            gSceneManager.addComponent(
//                    entity,
//                    RigidBody{
//                            .velocity = glm::vec3(0.0f, 0.0f, 0.0f),
//                            .acceleration = glm::vec3(0.0f, 0.0f, 0.0f)
//                    });
//
//            gSceneManager.addComponent(
//                    entity,
//                    TransformComponent{
//                            .translation = glm::vec3(randPosition(generator), randPosition(generator), randPosition(generator) + 50.f),
//                            .scale = glm::vec3(scale, scale, scale),
//                            .rotation = glm::vec3(randRotation(generator), randRotation(generator), randRotation(generator))
//                    });
//
//            gSceneManager.addComponent(entity, cubeModel);
//
//        }
    }
} // namespace
