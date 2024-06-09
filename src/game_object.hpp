//
// Created by Tristan Brault on 2023-11-30.
//

#pragma once

#include "model.hpp"
#include "texture.hpp"
#include "swap_chain.hpp"

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

    struct GameObjectBufferData {
        glm::mat4 modelMatrix{1.f};
        glm::mat4 normalMatrix{1.f};
    };

    class GameObjectManager; // forward declare game object manager class

    class GameObject {
    public:
        using id_t = unsigned int;
        using Map = std::unordered_map<id_t, GameObject>;

        GameObject(const GameObject&) = delete;
        GameObject &operator=(const GameObject&) = delete;
        GameObject(GameObject&&) = default;
        GameObject &operator=(GameObject&&) = default;

        id_t getId() const {
            return id;
        }

        VkDescriptorBufferInfo getBufferInfo(int frameIndex);

        glm::vec3 color{};
        TransformComponent transform{};

        std::shared_ptr<Model> model{};
        std::shared_ptr<Texture> diffuseMap = nullptr;
        std::unique_ptr<PointLightComponent> pointLight = nullptr;

    private:
        GameObject(id_t objId, const GameObjectManager &manager);

        id_t id;
        const GameObjectManager &gameObjectManager;

        friend class GameObjectManager;
    };

    class GameObjectManager {
    public:
        static constexpr int MAX_GAME_OBJECTS = 1000;

        GameObjectManager(Device &device);
        GameObjectManager(const GameObjectManager &) = delete;
        GameObjectManager &operator=(const GameObjectManager &) = delete;
        GameObjectManager(GameObjectManager &&) = delete;
        GameObjectManager &operator=(GameObjectManager &&) = delete;

        GameObject &createGameObject() {
            assert(currentId < MAX_GAME_OBJECTS && "Max game object count exceeded!");
            auto gameObject = GameObject{currentId++, *this};
            auto gameObjectId = gameObject.getId();
            gameObject.diffuseMap = textureDefault;
            gameObjects.emplace(gameObjectId, std::move(gameObject));
            return gameObjects.at(gameObjectId);
        }

        GameObject &makePointLight(
                float intensity = 10.f, float radius = 0.1f, glm::vec3 color = glm::vec3(1.f));

        VkDescriptorBufferInfo getBufferInfoForGameObject(
                int frameIndex, GameObject::id_t gameObjectId) const {
            return uboBuffers[frameIndex]->descriptorInfoForIndex(gameObjectId);
        }

        void updateBuffer(int frameIndex);

        GameObject::Map gameObjects{};
        std::vector<std::unique_ptr<Buffer>> uboBuffers{SwapChain::MAX_FRAMES_IN_FLIGHT};

    private:
        GameObject::id_t currentId = 0;
        std::shared_ptr<Texture> textureDefault;
    };

}




