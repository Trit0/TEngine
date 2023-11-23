#ifndef RIGID_BODY_HPP
#define RIGID_BODY_HPP

#include "../core/PhysicsComponent.hpp"
#include "../handlers/PhysicsHandler.hpp"

#define gravity 9.81

namespace GEngine {
class RigidBody : public PhysicsComponent {
   private:
    double mass;
    Vector3D speed;
    Vector3D acceleration;
    vector<PhysicsConstraint> constraints;

   public:
    RigidBody(double mass = 0, Vector3D speed = Vector3D::Zero(), Vector3D acceleration = Vector3D::Zero()) {
        this->mass = mass;
        this->speed = speed;
        this->acceleration = acceleration;
    }

    void Init() {
        PhysicsHandler::GetInstance().Subscribe(this);
    }

    void Update(double deltaTime) {
        ApplyConstraint(PhysicsConstraint(Vector3D::Down() * mass * gravity));
        ApplyConstraint(PhysicsConstraint(speed));
        this->speed += acceleration;
    }

    void Evaluate(double deltaTime) {
        for (PhysicsConstraint constraint : constraints) {
            constraint.Apply(this, deltaTime);
        }
        constraints.clear();
    }

    void ApplyConstraint(PhysicsConstraint constraint) {
        constraints.push_back(constraint);
    }

    void DetectInteractions(vector<PhysicsComponent*> others, int index) {}

    void Destroy() {
        PhysicsHandler::GetInstance().Unsubscribe(this);
    }

    bool IsRigidBody() {
        return true;
    }
};
}  // namespace GEngine

#endif
