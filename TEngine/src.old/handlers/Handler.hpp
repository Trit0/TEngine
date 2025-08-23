#ifndef HANDLER_HPP
#define HANDLER_HPP

#include "../core/Scene.hpp"

namespace GEngine {
template <typename H, typename C>
class Handler : public Singleton<H> {
   protected:
    Scene* currentScene;
    std::map<Scene*, std::vector<C*>> components;

   public:
    virtual void SetCurrentScene(Scene* scene) {
        this->currentScene = scene;
        if (components.find(scene) == components.end()) {
            components[scene] = std::vector<C*>();
        }
    }

    virtual void Subscribe(C* myComponent) {
        if (currentScene != nullptr) {
            components[currentScene].push_back(myComponent);
        }
    }

    virtual void Unsubscribe(C* myComponent) {
        if (currentScene != nullptr) {
            int i = 0;
            int indexToRemove = -1;
            for (C* component : components[currentScene]) {
                if (component == myComponent) {
                    indexToRemove = i;
                }
                i++;
            }
            if (indexToRemove != -1) {
                components[currentScene].erase(components[currentScene].begin() + indexToRemove);
            }
        }
    }

    virtual void ClearCurrentScene() {
        if (currentScene == nullptr) {
            return;
        }

        components[currentScene].clear();
    }
};

}  // namespace GEngine

#endif
