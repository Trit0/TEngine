#ifndef COLLIDER_HPP
#define COLLIDER_HPP

#include "../core/PhysicsComponent.hpp"
#include "../handlers/PhysicsHandler.hpp"
#include "../math/Matrix44D.hpp"
#include "../math/Vector3D.hpp"
#include "../resources/Color.hpp"
#include "ColoredMesh.hpp"
#include "Mesh.hpp"

namespace GEngine {
class Collider : public PhysicsComponent {
   protected:
    Mesh* mesh;
    bool isTrigger;
    std::vector<PhysicsConstraint> constraints;
    std::vector<PhysicsConstraint> temporaryConstraints;

   public:
    Collider(Mesh* mesh, bool isTrigger = false) {
        this->mesh = mesh;
    }

    void Init() {
        this->mesh->SetGameObjectTransform(this->GetGameObjectTransform());
        this->mesh->Init();
        RenderConfig overrideConfig = RenderConfig();
        overrideConfig.drawLines = true;
        this->mesh->SetRenderConfig(overrideConfig);
        PhysicsHandler::GetInstance().Subscribe(this);
    }

    void Destroy() {
        this->mesh->Destroy();
        PhysicsHandler::GetInstance().Unsubscribe(this);
    }

    virtual void DetectInteractions(vector<PhysicsComponent*> others, int index) = 0;
    virtual void Evaluate(double deltaTime) = 0;
    virtual void ApplyConstraint(PhysicsConstraint constraint) = 0;
    virtual void Draw() = 0;
    virtual void Translate(const Vector3D& translation) = 0;
    virtual void Transform(const Matrix44D& matrix, const Vector3D& position) = 0;

    bool IsRigidBody() {
        return false;
    }
};

}  // namespace GEngine

#endif
