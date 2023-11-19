#ifndef PHYSICS_CONSTRAINT_HPP
#define PHYSICS_CONSTRAINT_HPP

#include "../core/Component.hpp"
#include "../math/Vector3D.hpp"

namespace GEngine {
class PhysicsConstraint {
   private:
    Vector3D v;

   public:
    PhysicsConstraint(Vector3D v) {
        this->v = v;
    }

    void Apply(Component* component, double deltaTime) {
        component->GetGameObjectTransform()->Translate(v * deltaTime);
    }
};
}  // namespace GEngine

#endif
