//
// Created by Trist on 2024-05-12.
//

#include "json_scene.hpp"

#include "model_manager.hpp"
#include "game_object.hpp"
#include "ecs_core/demo/physics.hpp"
#include "device.hpp"

#include <simdjson.h>

namespace te {
    JsonScene::JsonScene(const std::string& filePath) {
        source = filePath;
    }



    void JsonScene::init(te::Device& device) {
        simdjson::ondemand::parser parser;
        simdjson::padded_string json = simdjson::padded_string::load(source);
        simdjson::ondemand::document content = parser.iterate(json);
        auto jsonEntities = content["entities"];

        auto modelManager = ModelManager{};

        for (auto jsonEntity : jsonEntities) {
            auto entity = gSceneManager.createEntity();

            // Transform
            auto transform = jsonEntity["transform"];
            auto transformComponent = TransformComponent::fromJson(transform);
            std::cout << transformComponent.translation.x << std::endl;
            gSceneManager.addComponent(entity, transformComponent);

            // Model
            std::string_view filePath;
            auto error = jsonEntity["model"].get(filePath);
            if (!error) {
                auto model = modelManager.getModel(device, std::string{filePath});
                gSceneManager.addComponent(entity, model);
            }

            gSceneManager.addComponent(
                    entity,
                    Gravity{glm::vec3(0.0f, -5.f, 0.0f)});

            gSceneManager.addComponent(
                    entity,
                    RigidBody{
                            .velocity = glm::vec3(0.0f, 0.0f, 0.0f),
                            .acceleration = glm::vec3(0.0f, 0.0f, 0.0f)
                    });

        }
    }
} // te