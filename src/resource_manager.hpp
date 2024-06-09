//
// Created by Trist on 2024-05-12.
//

#pragma once

#include "model.hpp"
#include "texture.hpp"

#include <unordered_map>
#include <string>
#include <memory>

namespace te {

    class ResourceManager {
    public:
        std::shared_ptr<Model> getModel(Device& device, const std::string& filePath);
        std::shared_ptr<Texture> getTexture(Device& device, const std::string& filePath);
        std::shared_ptr<Texture> getTextureCubeMap(Device& device, std::array<std::string, 6> textureFilePaths);
    private:
        std::unordered_map<std::string, std::shared_ptr<Model>> models{};
        std::unordered_map<std::string, std::shared_ptr<Texture>> textures{};
    };

} // te
