/**
 * @file Camera3d.hpp
 * @author Trito
 * @brief
 * @version 0.1
 * @date 2021-09-22
 *
 * @copyright Copyright (c) 2021
 *
 */
#ifndef CAMERA3D_HPP
#define CAMERA3D_HPP

#include <SDL2/SDL_opengl.h>

#include "design/Observer_Observable.hpp"
#include "math/Matrix44D.hpp"
#include "math/Vector3D.hpp"

namespace GEngine {
class Camera3D : public Observer {
   protected:
    Vector3D position;
    Vector3D front;
    Vector3D target;
    Vector3D up;
    Vector3D side;
    Matrix44D viewMatrix;
    bool updateCameraAngle;

   public:
    Camera3D(Vector3D target = Vector3D::Front(), Vector3D position = Vector3D::Back() * 5, Vector3D up = Vector3D::Up()) {
        this->position = position;
        this->target = this->front = target;
        this->up = up;

        updateCameraAngle = true;
    }

    void ApplyView() {
        // Multiplier la matrice de vue
        if (updateCameraAngle) {
            LoadView();
            updateCameraAngle = false;
        }

        glMultMatrixd(viewMatrix.matrix);
        glTranslated(-position.x, -position.y, -position.z);
    }

    void LoadView() {
        up = Vector3D::Up();

        front = target - position;
        front.Normalize();

        side = front % up;
        side.Normalize();

        up = side % front;
        up.Normalize();

        viewMatrix.LoadView(front, side, up);
    }

    void LookAt(Vector3D newTarget) {
        this->target = newTarget;
        updateCameraAngle = true;
    }

    inline void Move(Vector3D movement) {
        if (movement.GetNorm() < 1) {
            printf(movement.toString().c_str());
        }
        this->position = this->position + movement;
    }

    void DrawRay() {
        glDisable(GL_TEXTURE_2D);
        glBegin(GL_LINES);
        glVertex3d(position.x, position.y + 1.0, position.z);
        Vector3D f = position - target;
        f = f * 100.0;
        f.y -= 10;
        glVertex3d(f.x, f.y, f.z);
        glEnd();
        glEnable(GL_TEXTURE_2D);
    }

    virtual void Notification() = 0;

    virtual void Update(double deltaTime) = 0;
};
}  // namespace GEngine

#endif