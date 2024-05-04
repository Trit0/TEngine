//
// Created by Trist on 2024-05-04.
//

#pragma once

#include "entity_manager.hpp"

#include <unordered_map>

namespace te {

    class IComponentArray {
    public:
        virtual ~IComponentArray() = default;
        virtual void entityDestroyed(Entity entity) = 0;
    };

    template<typename T>
    class ComponentArray : public IComponentArray {
    public:
        void insertData(Entity entity, T component);
        void removeData(Entity entity);
        T& getData(Entity entity);
        void entityDestroyed(Entity entity) override;


    private:
        std::array<T, MAX_ENTITIES> mComponentArray{};
        std::unordered_map<Entity, size_t> mEntityToIndexMap{};
        std::unordered_map<size_t, Entity> mIndexToEntityMap{};
        size_t mSize{};
    };

} // te
