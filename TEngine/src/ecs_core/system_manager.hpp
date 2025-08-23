//
// Created by Trist on 2024-05-04.
//

#pragma once

#include "entity_manager.hpp"
#include "system.hpp"

#include <unordered_map>

namespace te {

    class SystemManager {
    public:
        template<typename T>
        std::shared_ptr<T> registerSystem() {
            const char* typeName = typeid(T).name();
            assert(mSystems.find(typeName) == mSystems.end() && "Registering system more than once.");

            auto system = std::make_shared<T>();
            mSystems.insert({typeName, system});
            return system;
        }

        template<typename T>
        void setSignature(Signature signature) {
            const char* typeName = typeid(T).name();
            assert(mSystems.find(typeName) != mSystems.end() && "System used before registration.");

            mSignatures.insert({typeName, signature});
        }

        template<typename T>
        Signature getSignature() {
            const char* typeName = typeid(T).name();
            assert(mSystems.find(typeName) != mSystems.end() && "System used before registration.");

            auto count = mSignatures.count(typeName);
            if (count == 0) {
                mSignatures.insert({typeName, Signature{}});
            }

            return mSignatures.at(typeName);
        }

        void entityDestroyed(Entity entity) {
            for (auto const& pair : mSystems) {
                auto const& system = pair.second;
                system->mEntities.erase(entity);
            }
        }

        void entitySignatureChanged(Entity entity, Signature entitySignature) {
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

    private:
        std::unordered_map<const char*, Signature> mSignatures{};
        std::unordered_map<const char*, std::shared_ptr<System>> mSystems{};
    };

} // te
