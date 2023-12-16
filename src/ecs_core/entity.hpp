//
// Created by Trist on 2023-12-16.
//

#pragma once

#include <vector>
#include <memory>

namespace te {

    class EntityManager;

    class Entity {

    };

    class Component {

        std::shared_ptr<Entity> entity;
    };

    class Service {

    };

    class EntityManager {
    public:
        std::vector<Entity> findEntities();

    private:
        std::vector<Entity> entities{};
        std::vector<Service> services{};
    };

} // te
