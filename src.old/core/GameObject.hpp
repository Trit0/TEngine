#ifndef GAMEOBJECT_HPP
#define GAMEOBJECT_HPP

#include "Component.hpp"
#include "LifeCycleObject.hpp"
#include "Transform.hpp"

namespace GEngine {
class GameObject : public LifeCycleObject {
   protected:
    Transform *transform;
    bool inited = false;
    std::vector<GameObject> children;
    std::vector<Component *> components;

   public:
    GameObject() {
        this->transform = new Transform();
        this->children = std::vector<GameObject>();
        this->components = std::vector<Component *>();
    }

    GameObject(Transform *transform, std::vector<GameObject> gameObjects, std::vector<Component *> components) {
        this->transform = transform;
        this->children = vector<GameObject>();
        for (int i = 0; i < gameObjects.size(); i++) {
            this->children.push_back(gameObjects[i]);
        }
        this->components = std::vector<Component *>();
        for (int i = 0; i < components.size(); i++) {
            components[i]->SetGameObjectTransform(transform);
            this->components.push_back(components[i]);
        }
    }

    // ~GameObject() {
    //     delete transform;
    // }

    void Init() {
        for (auto component : components) {
            component->Init();
        }
        inited = true;
        for (int i = 0; i < children.size(); i++) {
            children[i].Init();
        }
        this->transform->Notify();
    }

    void Start() {
        for (auto component : components) {
            component->Start();
        }
        for (int i = 0; i < children.size(); i++) {
            children[i].Start();
        }
    }

    void Update(double deltaTime) {
        for (auto component : components) {
            component->Update(deltaTime);
        }
        for (int i = 0; i < children.size(); i++) {
            children[i].Update(deltaTime);
        }
    }

    void LateUpdate(double deltaTime) {
        for (auto component : components) {
            component->LateUpdate(deltaTime);
        }
        for (int i = 0; i < children.size(); i++) {
            children[i].LateUpdate(deltaTime);
        }
    }

    void Quit() {
        for (auto component : components) {
            component->Quit();
        }
        for (int i = 0; i < children.size(); i++) {
            children[i].Quit();
        }
    }

    void Destroy() {
        for (auto component : components) {
            component->Destroy();
        }
        for (int i = 0; i < children.size(); i++) {
            children[i].Destroy();
        }
        // delete this;
    }

    void addChild(GameObject child) {
        this->children.push_back(child);
    }

    void addComponent(Component *component) {
        this->components.push_back(component);
    }

    Transform *getTransform() {
        return this->transform;
    }
};
}  // namespace GEngine

#endif