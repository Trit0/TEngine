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
        void registerComponent() {
            const char* typeName = typeid(T).name();

            assert(mComponentTypes.find(typeName) == mComponentTypes.end() && "Registering component type more than once.");

            mComponentTypes.insert({typeName, mNextComponentType});

            mComponentArrays.insert({typeName, std::make_shared<ComponentArray<T>>()});

            ++mNextComponentType;
        }

        template<typename T>
        ComponentType getComponentType() {
            const char* typeName = typeid(T).name();

            assert(mComponentTypes.find(typeName) != mComponentTypes.end() && "Component not registered before usage.");

            return mComponentTypes[typeName];
        }

        template<typename T>
        void addComponent(Entity entity, T component) {
            getComponentArray<T>()->insertData(entity, component);
        }

        template<typename T>
        void removeComponent(Entity entity) {
            getComponentArray<T>()->removeData(entity);
        }

        template<typename T>
        T& getComponent(Entity entity){
            return getComponentArray<T>()->getData(entity);
        }

        template<typename T>
        bool componentExist(Entity entity){
            return getComponentArray<T>()->dataExist(entity);
        }

        void entityDestroyed(Entity entity) {
            for (auto const& pair : mComponentArrays) {
                auto const& component = pair.second;
                component->entityDestroyed(entity);
            }
        }

    private:
        std::unordered_map<const char*, ComponentType> mComponentTypes{};
        std::unordered_map<const char*, std::shared_ptr<IComponentArray>> mComponentArrays{};
        ComponentType mNextComponentType{};

        template<typename T>
        std::shared_ptr<ComponentArray<T>> getComponentArray() {
            const char* typeName = typeid(T).name();
            assert(mComponentTypes.find(typeName) != mComponentTypes.end() && "Component not registered before usage.");
            return std::static_pointer_cast<ComponentArray<T>>(mComponentArrays[typeName]);
        }
    };

} // te
