//
// Created by Trist on 2024-05-04.
//

#include "component_manager.hpp"

namespace te {

    template<typename T>
    void te::ComponentManager::registerComponent() {
        const char* typeName = typeid(T).name();

        assert(mComponentTypes.find(typeName) == mComponentTypes.end() && "Registering component type more than once.");

        mComponentTypes.insert({typeName, mNextComponentType});

        mComponentArrays.insert({typeName, std::make_shared<ComponentArray<T>>()});

        ++mNextComponentType;
    }

    template<typename T>
    te::ComponentType te::ComponentManager::getComponentType() {
        const char* typeName = typeid(T).name();

        assert(mComponentTypes.find(typeName) != mComponentTypes.end() && "Component not registered before usage.");

        return mComponentTypes[typeName];
    }

    template<typename T>
    void te::ComponentManager::addComponent(Entity entity, T component) {
        getComponentArray<T>()->insertData(entity, component);
    }

    template<typename T>
    void te::ComponentManager::removeComponent(te::Entity entity) {
        getComponentArray<T>()->removeData(entity);
    }

    template<typename T>
    T& te::ComponentManager::getComponent(te::Entity entity) {
        return getComponentArray<T>()->getData(entity);
    }

    void te::ComponentManager::entityDestroyed(Entity entity) {
        for (auto const& pair : mComponentArrays) {
            auto const& component = pair.second;
            component->entityDestroyed(entity);
        }
    }

    template<typename T>
    std::shared_ptr<ComponentArray<T>> te::ComponentManager::getComponentArray() {
        const char* typeName = typeid(T).name();
        assert(mComponentTypes.find(typeName) != mComponentTypes.end() && "Component not registered before usage.");
        return std::static_pointer_cast<ComponentArray<T>>(mComponentArrays[typeName]);
    }
} // te