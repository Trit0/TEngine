//
// Created by Trist on 2024-05-04.
//

#include "system_manager.hpp"

namespace te {
    template<typename T>
    std::shared_ptr<T> SystemManager::registerSystem() {
        const char* typeName = typeid(T).name();
        assert(mSystems.find(typeName) == mSystems.end() && "Registering system more than once.");

        auto system = std::make_shared<T>();
        mSystems.insert({typeName, system});
        return system;
    }

    template<typename T>
    void SystemManager::setSignature(Signature signature) {
        const char* typeName = typeid(T).name();
        assert(mSystems.find(typeName) != mSystems.end() && "System used before registration.");

        mSignatures.insert({typeName, signature});
    }

    void SystemManager::entityDestroyed(Entity entity) {
        for (auto const& pair : mSystems) {
            auto const& system = pair.second;
            system->mEntities.erase(entity);
        }
    }

    void SystemManager::entitySignatureChanged(Entity entity, Signature entitySignature) {
        for (auto const& pair : mSystems) {
            auto const& type = pair.first;
            auto const& system = pair.second;
            auto const& systemSignature = mSignatures[type];

            if ((entitySignature & systemSignature) == systemSignature) {
                system->mEntities.insert(entity);
            } else {
                system->mEntities.erase(entity);
            }
        }
    }
} // te