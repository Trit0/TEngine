//
// Created by Trist on 2024-05-12.
//

#pragma once

#include "rendering/device.hpp"
#include "scene.hpp"

namespace te {

    class JsonScene : public Scene {
    public:
        JsonScene(const std::string& filePath);
        void init(te::Device &device) override;
        void free() override;
    private:
        std::string source;
    };

} // te
