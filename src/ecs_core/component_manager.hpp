//
// Created by Trist on 2024-05-04.
//

#pragma once

#include "entity_manager.hpp"
#include "component_array.hpp"

#include <unordered_map>

namespace te {

    class ComponentManager {
    public:
        template<typename T>
        void registerComponent();

        template<typename T>
        ComponentType getComponentType();

        template<typename T>
        void addComponent(Entity entity, T component);

        template<typename T>
        void removeComponent(Entity entity);

        template<typename T>
        T& getComponent(Entity entity);

        void entityDestroyed(Entity entity);

    private:
        std::unordered_map<const char*, ComponentType> mComponentTypes{};
        std::unordered_map<const char*, std::shared_ptr<IComponentArray>> mComponentArrays{};
        ComponentType mNextComponentType{};

        template<typename T>
        std::shared_ptr<ComponentArray<T>> getComponentArray();
    };

} // te
