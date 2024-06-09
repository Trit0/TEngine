//
// Created by Trist on 2024-05-12.
//

#include "json_scene.hpp"

#include "resource_manager.hpp"
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

        auto resourceManager = ResourceManager{};
        int count = 0;
        for (auto jsonEntity : jsonEntities) {
            auto entity = gSceneManager.createEntity();
            count++;

            // Transform
            auto transform = jsonEntity["transform"];
            auto transformComponent = TransformComponent::fromJson(transform);
            gSceneManager.addComponent(entity, transformComponent);

            // Model
            std::string_view filePath;
            auto error = jsonEntity["model"].get(filePath);
            if (!error) {
                auto model = resourceManager.getModel(device, std::string{filePath});
                gSceneManager.addComponent(entity, model);
            }

            //Texture
            error = jsonEntity["texture"].get(filePath);
            if (!error) {
                auto texture = resourceManager.getTexture(device, std::string{filePath});
//                auto side = std::string{"../textures/crate.png"};
//                auto top = std::string{"../textures/crate.png"};
//                auto texture = resourceManager.getTextureCubeMap(device, std::array{ top, side, side, side, side, top });
                gSceneManager.addComponent(entity, texture);
            }

//            gSceneManager.addComponent(
//                    entity,
//                    Gravity{glm::vec3(0.0f, -5.f, 0.0f)});

            gSceneManager.addComponent(
                    entity,
                    RigidBody{
                            .velocity = glm::vec3(0.0f, 0.0f, 0.0f),
                            .acceleration = glm::vec3(0.0f, 0.0f, 0.0f)
                    });

        }
        std::cout << count << " entities loaded" << std::endl;
    }
} // te