//
// Created by Trist on 2024-05-12.
//

#include "resource_manager.hpp"

#include <iostream>
#include <memory>
#include <array>

namespace te {
    std::shared_ptr<Model> ResourceManager::getModel(Device& device, const std::string& filePath) {
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

    std::shared_ptr<Texture> ResourceManager::getTexture(te::Device & device, const std::string& filePath) {
        std::shared_ptr<Texture> texture;
        if (!textures.empty()) {
            texture = textures.at(filePath);
        }
        if (texture == nullptr) {
            texture = Texture::createTextureFromFile(device, filePath);
            textures.emplace(filePath, texture);
        }
        return texture;
    }

    std::shared_ptr<Texture> ResourceManager::getTextureCubeMap(Device & device, std::array<std::string, 6> textureFilePaths) {
        std::shared_ptr<Texture> texture;
        if (!textures.empty()) {
            texture = textures.at(textureFilePaths[0]+"_cm");
        }
        if (texture == nullptr) {
            texture = Texture::createTextureFromFiles(device, textureFilePaths);
            textures.emplace(textureFilePaths[0]+"_cm", texture);
        }
        return texture;
    }
} // te