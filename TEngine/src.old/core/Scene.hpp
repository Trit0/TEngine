#ifndef SCENE_HPP
#define SCENE_HPP

#include "GameObject.hpp"
#include "LifeCycleObject.hpp"

namespace GEngine {
class Scene : public LifeCycleObject {
   protected:
    std::vector<GameObject> gameObjects;

   public:
    Scene() {
        this->gameObjects = std::vector<GameObject>();
    }

    Scene(std::vector<GameObject> gameObjects) {
        this->gameObjects = std::vector<GameObject>();
        for (int i = 0; i < gameObjects.size(); i++) {
            this->gameObjects.push_back(gameObjects[i]);
        }
    }

    void Init() {
        for (int i = 0; i < gameObjects.size(); i++) {
            this->gameObjects[i].Init();
        }
    }

    void Start() {
        for (int i = 0; i < gameObjects.size(); i++) {
            this->gameObjects[i].Start();
        }
    }

    void Update(double deltaTime) {
        for (int i = 0; i < gameObjects.size(); i++) {
            this->gameObjects[i].Update(deltaTime);
        }
    }

    void LateUpdate(double deltaTime) {
        for (int i = 0; i < gameObjects.size(); i++) {
            this->gameObjects[i].LateUpdate(deltaTime);
        }
    }

    void Quit() {
        for (int i = 0; i < gameObjects.size(); i++) {
            this->gameObjects[i].Quit();
        }
    }

    void Destroy() {
        for (int i = 0; i < gameObjects.size(); i++) {
            this->gameObjects[i].Destroy();
        }
    }

    void addGameObject(GameObject gameObject) {
        this->gameObjects.push_back(gameObject);
    }
};
}  // namespace GEngine

#endif