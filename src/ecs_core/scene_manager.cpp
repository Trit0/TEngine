//
// Created by Trist on 2024-05-04.
//

#include "scene_manager.hpp"

namespace te {
    void SceneManager::init() {
        mComponentManager = std::make_unique<ComponentManager>();
        mEntityManager = std::make_unique<EntityManager>();
        mSystemManager = std::make_unique<SystemManager>();
    }

    Entity SceneManager::createEntity() {
        return mEntityManager->createEntity();
    }

    void SceneManager::destroyEntity(Entity entity) {
        mEntityManager->destroyEntity(entity);
        mComponentManager->entityDestroyed(entity);
        mSystemManager->entityDestroyed(entity);
    }

    template<typename T>
    void SceneManager::registerComponent() {
        mComponentManager->registerComponent<T>();
    }

    template<typename T>
    void SceneManager::addComponent(Entity entity, T component) {
        mComponentManager->addComponent<T>(entity, component);

        auto signature = mEntityManager->getSignature(entity);
        signature.set(mComponentManager->getComponentType<T>(), true);
        mEntityManager->setSignature(entity, signature);
        mSystemManager->entitySignatureChanged(entity, signature);
    }

    template<typename T>
    void SceneManager::removeComponent(Entity entity) {
        mComponentManager->removeComponent<T>(entity);

        auto signature = mEntityManager->getSignature(entity);
        signature.set(mComponentManager->getComponentType<T>(), false);
        mEntityManager->setSignature(entity, signature);

        mSystemManager->entitySignatureChanged(entity, signature);
    }

    template<typename T>
    T& SceneManager::getComponent(Entity entity) {
        return mComponentManager->getComponent<T>(entity);
    }

    template<typename T>
    ComponentType SceneManager::getComponentType() {
        return mComponentManager->getComponentType<T>();
    }

    template<typename T>
    std::shared_ptr<T> SceneManager::registerSystem() {
        return mSystemManager->registerSystem<T>();
    }

    template<typename T>
    void SceneManager::setSystemSignature(Signature signature) {
        mSystemManager->setSignature<T>(signature);
    }
} // te