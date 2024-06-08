//
// Created by Trist on 2024-05-12.
//

#include "model_manager.hpp"

#include <iostream>
#include <memory>

namespace te {
    std::shared_ptr<Model> ModelManager::getModel(Device& device, const std::string& filePath) {
        std::shared_ptr<Model> model;
        if (!models.empty()) {
            model = models.at(filePath);
        }
        if (model == nullptr) {
            model = Model::createModelFromFile(device, filePath);
            models.emplace(filePath, model);
        }
        return model;
    }
} // te