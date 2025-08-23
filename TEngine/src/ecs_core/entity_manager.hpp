//
// Created by Trist on 2023-12-16.
//

#pragma once

#include <vector>
#include <memory>
#include <bitset>
#include <queue>
#include <cassert>
#include <array>

namespace te {
    using Entity = uint32_t;
    const Entity MAX_ENTITIES = 50000;

    using ComponentType = uint8_t;
    const ComponentType MAX_COMPONENTS = 64;

    using Signature = std::bitset<MAX_COMPONENTS>;

    class EntityManager {
    public:
        EntityManager() {
            for (Entity entity = 0; entity < MAX_ENTITIES; ++entity){
                mAvailableEntities.push(entity);
            }
        }

        Entity createEntity() {
            assert(mLivingEntityCount < MAX_ENTITIES && "Too many entities in existence.");

            Entity id = mAvailableEntities.front();
            mAvailableEntities.pop();
            ++mLivingEntityCount;

            return id;
        }

        void destroyEntity(Entity entity) {
            assert(entity < MAX_ENTITIES && "Entity out of range.");

            mSignatures[entity].reset();

            mAvailableEntities.push(entity);
            --mLivingEntityCount;
        }

        void setSignature(Entity entity, Signature signature) {
            assert(entity < MAX_ENTITIES && "Entity out of range.");
            mSignatures[entity] = signature;
        }

        Signature getSignature(Entity entity) {
            assert(entity < MAX_ENTITIES && "Entity out of range.");

            return mSignatures[entity];
        }

    private:
        std::queue<Entity> mAvailableEntities{};
        std::array<Signature, MAX_ENTITIES> mSignatures{};
        uint32_t mLivingEntityCount{};
    };

} // te
