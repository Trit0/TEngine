//
// Created by Trist on 2024-05-12.
//

#include "scene.hpp"

#include "json_scene.hpp"

#include <memory>

namespace te {
    std::unique_ptr<Scene> Scene::fromJson(const std::string& filePath) {
        auto scene = std::make_unique<JsonScene>(JsonScene{filePath});
        return std::move(scene);
    }
} // te