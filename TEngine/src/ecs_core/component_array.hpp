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
        void insertData(Entity entity, T component) {
            assert(mEntityToIndexMap.find(entity) == mEntityToIndexMap.end() && "Component added to same entity more than once.");

            size_t newIndex = mSize;
            mEntityToIndexMap[entity] = newIndex;
            mIndexToEntityMap[newIndex] = entity;
            mComponentArray[newIndex] = component;
            ++mSize;
        }

        void removeData(Entity entity) {
            assert(mEntityToIndexMap.find(entity) != mEntityToIndexMap.end() && "Removing non-existent component.");

            size_t indexOfRemovedEntity = mEntityToIndexMap[entity];
            size_t indexOfLastElement = mSize - 1;
            mComponentArray[indexOfRemovedEntity] = mComponentArray[indexOfLastElement];

            Entity entityOfLastElement = mIndexToEntityMap[indexOfLastElement];
            mEntityToIndexMap[entityOfLastElement] = indexOfRemovedEntity;
            mIndexToEntityMap[indexOfRemovedEntity] = entityOfLastElement;

            mEntityToIndexMap.erase(entity);
            mIndexToEntityMap.erase(indexOfLastElement);
            --mSize;
        }

        T& getData(Entity entity) {
            assert(dataExist(entity) && "Retrieving non-existent component.");

            return mComponentArray[mEntityToIndexMap[entity]];
        }

        bool dataExist(Entity entity) {
            return mEntityToIndexMap.find(entity) != mEntityToIndexMap.end();
        }

        void entityDestroyed(Entity entity) override {
            if (mEntityToIndexMap.find(entity) != mEntityToIndexMap.end()) {
                removeData(entity);
            }
        }


    private:
        std::array<T, MAX_ENTITIES> mComponentArray;
        std::unordered_map<Entity, size_t> mEntityToIndexMap;
        std::unordered_map<size_t, Entity> mIndexToEntityMap;
        size_t mSize;
    };

} // te
