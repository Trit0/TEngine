//
// Created by Trist on 2024-05-12.
//

#pragma once

#include "model.hpp"

#include <unordered_map>
#include <string>
#include <memory>

namespace te {

    class ModelManager {
    public:
        std::shared_ptr<Model> getModel(Device& device, const std::string& filePath);
    private:
        std::unordered_map<std::string, std::shared_ptr<Model>> models{};
    };

} // te
