#ifndef TRANSFORM_HPP
#define TRANSFORM_HPP

#include "../design/Observer_Observable.hpp"
#include "../math/MathUtils.hpp"
#include "../math/Vector3D.hpp"

namespace GEngine {
class Transform : public Observable {
   private:
    Vector3D position;
    Vector3D rotation;
    Vector3D scale;

   public:
    Transform(Vector3D position = Vector3D(), Vector3D rotation = Vector3D(), Vector3D scale = Vector3D::Identity()) {
        this->position = position;
        this->rotation = rotation;
        this->scale = scale;
    }

    Vector3D GetPosition() {
        return position;
    }

    Vector3D GetRotationRad() {
        return Vector3D(MathUtils::DegToRad(rotation.x), MathUtils::DegToRad(rotation.y), MathUtils::DegToRad(rotation.z));
    }

    Vector3D GetRotation() {
        return rotation;
    }

    Vector3D GetScale() {
        return scale;
    }

    void Translate(double x, double y, double z) {
        this->position = this->position + Vector3D(x, y, z);
        this->Notify();
    }

    void Translate(Vector3D translation) {
        this->position += translation;
        this->Notify();
    }

    void RotateRad(double xAngleRad, double yAngleRad, double zAngleRad) {
        this->rotation += Vector3D(MathUtils::RadToDeg(xAngleRad), MathUtils::RadToDeg(yAngleRad), MathUtils::RadToDeg(zAngleRad));
        this->Notify();
    }

    void Rotate(double xAngleDeg, double yAngleDeg, double zAngleDeg) {
        this->rotation += Vector3D(xAngleDeg, yAngleDeg, zAngleDeg);
        this->Notify();
    }

    void Scale(double xScale, double yScale, double zScale) {
        this->scale += Vector3D(xScale, yScale, zScale);
        this->Notify();
    }

    void SetScale(double x, double y, double z) {
        this->scale = Vector3D(x, y, z);
        this->Notify();
    }

    string toString() {
        return position.toString() + "\n" + rotation.toString() + "\n" + scale.toString() + "\n";
    }
};

}  // namespace GEngine

#endif