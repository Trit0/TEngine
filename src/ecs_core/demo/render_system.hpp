//
// Created by Trist on 2024-05-11.
//

#pragma once

#include "../system.hpp"
#include "frame_info.hpp"
#include "systems/simple_render_system.hpp"

namespace te {

    class RenderSystem : public System {
    public:
        void render(FrameInfo& frameInfo, SimpleRenderSystem& simpleRenderSystem) {
            simpleRenderSystem.renderGameObjects(frameInfo, mEntities);
        }
    };

} // te
