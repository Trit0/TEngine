#ifndef PHYSICS_HANDLER_HPP
#define PHYSICS_HANDLER_HPP

#include "../core/PhysicsComponent.hpp"
#include "Handler.hpp"

namespace GEngine {
class PhysicsHandler : public Handler<PhysicsHandler, PhysicsComponent> {
   public:
    void Evaluate(double deltaTime) {
        if (currentScene == nullptr) {
            return;
        }

        std::vector<PhysicsComponent*> currentScenePhysicsComponents = components[currentScene];
        std::vector<PhysicsComponent*> currentSceneColliders;
        std::vector<PhysicsComponent*> currentSceneRigidBodies;

        // TODO change mais je suis BS
        for (PhysicsComponent* component : currentScenePhysicsComponents) {
            if (component->IsRigidBody()) {
                currentSceneRigidBodies.push_back(component);
            } else {
                currentSceneColliders.push_back(component);
            }
        }

        for (int i = 0; i < currentSceneColliders.size(); i++) {
            currentSceneColliders[i]->DetectInteractions(currentSceneColliders, i);
        }

        for (int i = 0; i < currentSceneRigidBodies.size(); i++) {
            currentSceneRigidBodies[i]->DetectInteractions(currentSceneRigidBodies, i);
        }

        for (int i = 0; i < currentSceneColliders.size(); i++) {
            currentSceneColliders[i]->Evaluate(deltaTime);
        }

        for (int i = 0; i < currentSceneRigidBodies.size(); i++) {
            currentSceneRigidBodies[i]->Evaluate(deltaTime);
        }
    }
};

}  // namespace GEngine

#endif
