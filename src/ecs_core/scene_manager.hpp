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
        void init();
        Entity createEntity();
        void destroyEntity(Entity entity);

        template<typename T>
        void registerComponent();

        template<typename T>
        void addComponent(Entity entity, T component);

        template<typename T>
        void removeComponent(Entity entity);

        template<typename T>
        T& getComponent(Entity entity);

        template<typename T>
        ComponentType getComponentType();

        template<typename T>
        std::shared_ptr<T> registerSystem();

        template<typename T>
        void setSystemSignature(Signature signature);


    private:
        std::unique_ptr<ComponentManager> mComponentManager;
        std::unique_ptr<EntityManager> mEntityManager;
        std::unique_ptr<SystemManager> mSystemManager;
    };

} // te
