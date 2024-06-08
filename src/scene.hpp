//
// Created by Trist on 2024-05-12.
//

#pragma once

#include "ecs_core/scene_manager.hpp"
#include "device.hpp"

#include <string>

namespace te {

    extern SceneManager gSceneManager;

    class Scene {
    public:
        static std::unique_ptr<Scene> fromJson(const std::string& filePath);
        virtual void init(Device& device) = 0;
        virtual ~Scene() {};
    };

} // te
