//
// Created by Trist on 2024-05-04.
//

#pragma once

#include "component_manager.hpp"
#include "entity_manager.hpp"
#include "system_manager.hpp"

#include <memory>

namespace te {

    class SceneManager {
    public:
        void init() {
            mComponentManager = std::make_unique<ComponentManager>();
            mEntityManager = std::make_unique<EntityManager>();
            mSystemManager = std::make_unique<SystemManager>();
        }

        Entity createEntity() {
            return mEntityManager->createEntity();
        }

        void destroyEntity(Entity entity) {
            mEntityManager->destroyEntity(entity);
            mComponentManager->entityDestroyed(entity);
            mSystemManager->entityDestroyed(entity);
        }

        template<typename T>
        void registerComponent() {
            mComponentManager->registerComponent<T>();
        }

        template<typename T>
        void addComponent(Entity entity, T component) {
            mComponentManager->addComponent<T>(entity, component);

            auto signature = mEntityManager->getSignature(entity);
            signature.set(mComponentManager->getComponentType<T>(), true);
            mEntityManager->setSignature(entity, signature);
            mSystemManager->entitySignatureChanged(entity, signature);
        }

        template<typename T>
        void removeComponent(Entity entity) {
            mComponentManager->removeComponent<T>(entity);

            auto signature = mEntityManager->getSignature(entity);
            signature.set(mComponentManager->getComponentType<T>(), false);
            mEntityManager->setSignature(entity, signature);

            mSystemManager->entitySignatureChanged(entity, signature);
        }

        template<typename T>
        T& getComponent(Entity entity) {
            return mComponentManager->getComponent<T>(entity);
        }

        template<typename T>
        ComponentType getComponentType() {
            return mComponentManager->getComponentType<T>();
        }

        template<typename T>
        std::shared_ptr<T> registerSystem() {
            return mSystemManager->registerSystem<T>();
        }

        template<typename T>
        void setSystemSignature(Signature signature) {
            mSystemManager->setSignature<T>(signature);
        }


    private:
        std::unique_ptr<ComponentManager> mComponentManager;
        std::unique_ptr<EntityManager> mEntityManager;
        std::unique_ptr<SystemManager> mSystemManager;
    };

} // te
