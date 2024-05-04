//
// Created by Trist on 2024-05-04.
//

#pragma once

#include "../scene_manager.hpp"
#include "physics.hpp"

#include <random>
#include <chrono>

namespace te {

    SceneManager gSceneManager;

    int main()
    {
        gSceneManager.init();

        gSceneManager.registerComponent<Gravity>();
        gSceneManager.registerComponent<RigidBody>();
        gSceneManager.registerComponent<TransformComponent>();

        auto physicsSystem = gSceneManager.registerSystem<PhysicsSystem>();

        Signature signature;
        signature.set(gSceneManager.getComponentType<Gravity>());
        signature.set(gSceneManager.getComponentType<RigidBody>());
        signature.set(gSceneManager.getComponentType<TransformComponent>());
        gSceneManager.setSystemSignature<PhysicsSystem>(signature);

        std::vector<Entity> entities(MAX_ENTITIES);

        std::default_random_engine generator;
        std::uniform_real_distribution<float> randPosition(-100.0f, 100.0f);
        std::uniform_real_distribution<float> randRotation(0.0f, 3.0f);
        std::uniform_real_distribution<float> randScale(3.0f, 5.0f);
        std::uniform_real_distribution<float> randGravity(-10.0f, -1.0f);

        float scale = randScale(generator);

        for (auto& entity : entities)
        {
            entity = gSceneManager.createEntity();

            gSceneManager.addComponent(
                    entity,
                    Gravity{glm::vec3(0.0f, randGravity(generator), 0.0f)});

            gSceneManager.addComponent(
                    entity,
                    RigidBody{
                            .velocity = glm::vec3(0.0f, 0.0f, 0.0f),
                            .acceleration = glm::vec3(0.0f, 0.0f, 0.0f)
                    });

            gSceneManager.addComponent(
                    entity,
                    TransformComponent{
                            .translation = glm::vec3(randPosition(generator), randPosition(generator), randPosition(generator)),
                            .scale = glm::vec3(scale, scale, scale),
                            .rotation = glm::vec3(randRotation(generator), randRotation(generator), randRotation(generator))
                    });
        }

        float dt = 0.0f;

        auto quit = false;

        while (!quit)
        {
            auto startTime = std::chrono::high_resolution_clock::now();

            physicsSystem->update(dt);

            auto stopTime = std::chrono::high_resolution_clock::now();

            dt = std::chrono::duration<float, std::chrono::seconds::period>(stopTime - startTime).count();
        }
    }

}