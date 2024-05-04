//
// Created by Trist on 2024-05-04.
//

#pragma once

#include "entity_manager.hpp"

#include <set>

namespace te {

    class System {
    public:
        std::set<Entity> mEntities{};
    };
} // te

