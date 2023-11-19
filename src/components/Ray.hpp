#pragma once

#include "../core/RenderComponent.hpp"
#include "../debug/Debugger.hpp"
#include "../resources/Color.hpp"

namespace GEngine {
class Ray : public RenderComponent {
   private:
    Vector3D target;
    Color color;
    Transform lastTransform;

   public:
    Ray(Vector3D target, Color color) {
        this->target = target;
        this->color = color;
        this->lastTransform = GEngine::Transform();
    }

    void Init() {
        RenderHandler::GetInstance().Subscribe(this);
        this->GetGameObjectTransform()->AddObserver(this);
    }

    void Destroy() {
        RenderHandler::GetInstance().Unsubscribe(this);
        this->GetGameObjectTransform()->RemoveObserver(this);
    }

    void Notification() {
        Vector3D rotationChange = this->GetGameObjectTransform()->GetRotationRad() - this->lastTransform.GetRotationRad();

        if (rotationChange.GetNorm() > 0) {
            Matrix44D rotation = Matrix44D();
            if (abs(rotationChange.x) > 0) {
                rotation.LoadRotationX(rotationChange.x);
                target = target - GetGameObjectTransform()->GetPosition();
                target = rotation * target;
                target = target + GetGameObjectTransform()->GetPosition();
            }
            if (abs(rotationChange.y) > 0) {
                rotation.LoadRotationY(rotationChange.y);
                target = target - GetGameObjectTransform()->GetPosition();
                target = rotation * target;
                target = target + GetGameObjectTransform()->GetPosition();
            }
            if (abs(rotationChange.z) > 0) {
                rotation.LoadRotationZ(rotationChange.z);
                target = target - GetGameObjectTransform()->GetPosition();
                target = rotation * target;
                target = target + GetGameObjectTransform()->GetPosition();
            }
        }

        this->lastTransform = *this->GetGameObjectTransform();
    }

    void Render(RenderConfig config) {
        glDisable(GL_TEXTURE_2D);
        Vector3D position = GetGameObjectTransform()->GetPosition();
        glBegin(GL_LINES);
        glColor4d(color.GetR(), color.GetG(), color.GetB(), color.GetA());

        glVertex3d(position.x, position.y, position.z);
        glVertex3d(target.x, target.y, target.z);

        glColor4d(1.0, 1.0, 1.0, 1.0);
        glEnd();
        glEnable(GL_TEXTURE_2D);
    }
};
}  // namespace GEngine
