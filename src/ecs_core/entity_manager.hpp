//
// Created by Trist on 2023-12-16.
//

#pragma once

#include <vector>
#include <memory>
#include <bitset>
#include <queue>
#include <cassert>

namespace te {
    using Entity = uint32_t;
    const Entity MAX_ENTITIES = 5000;

    using ComponentType = uint8_t;
    const ComponentType MAX_COMPONENTS = 64;

    using Signature = std::bitset<MAX_COMPONENTS>;

    class EntityManager {
    public:
        EntityManager();

        Entity createEntity();
        void destroyEntity(Entity entity);
        void setSignature(Entity entity, Signature signature);
        Signature getSignature(Entity entity);

    private:
        std::queue<Entity> mAvailableEntities{};
        std::array<Signature, MAX_ENTITIES> mSignatures{};
        uint32_t mLivingEntityCount{};
    };

//    // Data of an object
//    class Component {
//        id_t entityId;
//    };
//
//    // Functionality of an object
//    class Service {
//        id_t entityId;
//    };

} // te
