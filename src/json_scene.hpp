//
// Created by Trist on 2024-05-12.
//

#pragma once

#include "scene.hpp"

namespace te {

    class JsonScene : public Scene {
    public:
        JsonScene(const std::string& filePath);
        void init(te::Device &device) override;
    private:
        std::string source;
    };

} // te
