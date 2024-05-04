//
// Created by Trist on 2024-05-04.
//

#pragma once

#include "../system.hpp"
#include "../scene_manager.hpp"
#include "../../game_object.hpp"

#include <glm/glm.hpp>

namespace te {

    struct Gravity {
        glm::vec3 force;
    };

    struct RigidBody {
        glm::vec3 velocity;
        glm::vec3 acceleration;
    };

    extern SceneManager gSceneManager;

    class PhysicsSystem : public System {
    public:
        void update(float dt) {
            for (auto const& entity : mEntities) {
                auto& rigidBody = gSceneManager.getComponent<RigidBody>(entity);
                auto& transform = gSceneManager.getComponent<TransformComponent>(entity);
                auto const& gravity = gSceneManager.getComponent<Gravity>(entity);

                transform.translation += rigidBody.velocity * dt;
                rigidBody.velocity += gravity.force * dt;
            }
        }
    };
}
