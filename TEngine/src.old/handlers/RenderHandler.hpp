#ifndef RENDER_HANDLER_HPP
#define RENDER_HANDLER_HPP

#include <map>

#include "../core/RenderComponent.hpp"
#include "../core/Scene.hpp"
#include "../design/Singleton.hpp"
#include "Handler.hpp"
#include "RenderConfig.hpp"

namespace GEngine {

class RenderHandler : public Handler<RenderHandler, RenderComponent> {
   private:
    RenderConfig config;

   public:
    void Render() {
        if (currentScene == nullptr) {
            return;
        }

        config.drawLines = false;

        std::vector<RenderComponent*> currentSceneRenderComponents = components[currentScene];
        for (RenderComponent* component : currentSceneRenderComponents) {
            component->Render(this->config);
        }
    }

    RenderConfig GetRenderConfig() {
        return this->config;
    }
};

}  // namespace GEngine

#endif
