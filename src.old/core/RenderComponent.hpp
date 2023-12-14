#ifndef RENDER_COMPONENT_HPP
#define RENDER_COMPONENT_HPP

#include "../handlers/RenderConfig.hpp"
#include "Component.hpp"

namespace GEngine {
class RenderComponent : public Component {
   public:
    RenderConfig config;
    bool configIsOverriden = false;

    void SetRenderConfig(RenderConfig newConfig) {
        configIsOverriden = true;
        this->config = newConfig;
    }

    virtual void Render(RenderConfig config) = 0;
};
}  // namespace GEngine

#endif
