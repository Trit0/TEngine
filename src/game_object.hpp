//
// Created by Tristan Brault on 2023-11-30.
//

#pragma once

#include "model.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <simdjson.h>
#include <unordered_map>

namespace  te {
    struct TransformComponent {
        glm::vec3 translation{};
        glm::vec3 scale{1.0f, 1.0f, 1.0f};
        glm::vec3 rotation{};

        // Matrix corrsponds to Translate * Ry * Rx * Rz * Scale
        // Rotations correspond to Tait-bryan angles of Y(1), X(2), Z(3)
        // https://en.wikipedia.org/wiki/Exluler_angles#Rotation_matrix
        glm::mat4 mat4();
        glm::mat3 normalMatrix();
        static TransformComponent fromJson(simdjson::simdjson_result<simdjson::ondemand::value> json) {
            auto transformComponent = TransformComponent{};
            transformComponent.translation.x = json["translation"]["x"].get_double().value();
            transformComponent.translation.y = json["translation"]["y"].get_double().value();
            transformComponent.translation.z = json["translation"]["z"].get_double().value();
            transformComponent.rotation.x = json["rotation"]["x"].get_double().value();
            transformComponent.rotation.y = json["rotation"]["y"].get_double().value();
            transformComponent.rotation.z = json["rotation"]["z"].get_double().value();
            transformComponent.scale.x = json["scale"]["x"].get_double().value();
            transformComponent.scale.y = json["scale"]["y"].get_double().value();
            transformComponent.scale.z = json["scale"]["z"].get_double().value();
            return transformComponent;
        }
    };

    struct PointLightComponent {
        float lightIntensity = 1.0f;
    };

    class GameObject {
    public:
        using id_t = unsigned int;
        using Map = std::unordered_map<id_t, GameObject>;

        static GameObject createGameObject() {
            static id_t currentId = 0;
            return GameObject{currentId++};
        }

        GameObject(const GameObject&) = delete;
        GameObject &operator=(const GameObject&) = delete;
        GameObject(GameObject&&) = default;
        GameObject &operator=(GameObject&&) = default;

        id_t getId() const {
            return id;
        }

        static GameObject makePointLight(float intensity = 10.0f, float radius = 0.1f, glm::vec3 color = glm::vec3(1.f));

        glm::vec3 color{};
        TransformComponent transform{};

        std::shared_ptr<Model> model{};
        std::unique_ptr<PointLightComponent> pointLight = nullptr;

    private:
        GameObject(id_t objId) : id{objId} {}

        id_t id;
    };
}




