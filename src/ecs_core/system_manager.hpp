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
        std::shared_ptr<T> registerSystem();

        template<typename T>
        void setSignature(Signature signature);

        void entityDestroyed(Entity entity);

        void entitySignatureChanged(Entity entity, Signature entitySignature);




    private:
        std::unordered_map<const char*, Signature> mSignatures{};
        std::unordered_map<const char*, std::shared_ptr<System>> mSystems{};
    };

} // te
