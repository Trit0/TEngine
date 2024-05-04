//
// Created by Trist on 2024-05-04.
//

#include "component_array.hpp"

namespace te {
    template<typename T>
    void ComponentArray<T>::insertData(Entity entity, T component) {
        assert(mEntityToIndexMap.find(entity) == mEntityToIndexMap.end() && "Component added to same entity more than once.");

        size_t newIndex = mSize;
        mEntityToIndexMap[entity] = newIndex;
        mIndexToEntityMap[newIndex] = entity;
        mComponentArray[newIndex] = component;
        ++mSize;
    }

    template<typename T>
    void ComponentArray<T>::removeData(Entity entity) {
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

    template<typename T>
    T& ComponentArray<T>::getData(te::Entity entity) {
        assert(mEntityToIndexMap.find(entity) != mEntityToIndexMap.end() && "Retrieving non-existent component.");

        return mComponentArray[mEntityToIndexMap[entity]];
    }

    template<typename T>
    void ComponentArray<T>::entityDestroyed(te::Entity entity) {
        if (mEntityToIndexMap.find(entity) != mEntityToIndexMap.end()) {
            removeData(entity);
        }
    }


} // te