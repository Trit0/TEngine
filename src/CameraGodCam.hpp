#ifndef FIRSTPERSONCAMERA_HPP
#define FIRSTPERSONCAMERA_HPP

#include <iostream>

#include "Camera3d.hpp"
#include "EventContainer.hpp"
#include "math/Matrix44D.hpp"
#include "math/Vector3D.hpp"

namespace GEngine {
class CameraGodCam : public Camera3D {
   private:
    bool keyW = false, keyS = false, keyA = false, keyD = false, keySpace = false, keyCtrl = false;
    double sensitivity = 0.001;
    double angleX = 0.0;
    double angleY = 0.0;
    double movementSpeed = 1;
    Matrix44D rotationMatrix;

   public:
    CameraGodCam(Vector3D tar = Vector3D(0.0, 0.0, -1.0), Vector3D pos = Vector3D::Zero(), Vector3D angleUp = Vector3D(0.0, 1.0, 0.0)) : Camera3D(tar, pos, angleUp) {}

    void Notification() {
        switch (EventContainer::GetType()) {
            case SDL_KEYDOWN:
                switch (EventContainer::GetKey()) {
                    case SDLK_w:
                        keyW = true;
                        break;
                    case SDLK_a:
                        keyA = true;
                        break;
                    case SDLK_s:
                        keyS = true;
                        break;
                    case SDLK_d:
                        keyD = true;
                        break;
                    case SDLK_SPACE:
                        keySpace = true;
                        break;
                    case SDLK_LCTRL:
                        keyCtrl = true;
                        break;
                }
                break;
            case SDL_KEYUP:
                switch (EventContainer::GetKey()) {
                    case SDLK_w:
                        keyW = false;
                        break;
                    case SDLK_a:
                        keyA = false;
                        break;
                    case SDLK_s:
                        keyS = false;
                        break;
                    case SDLK_d:
                        keyD = false;
                        break;
                    case SDLK_SPACE:
                        keySpace = false;
                        break;
                    case SDLK_LCTRL:
                        keyCtrl = false;
                        break;
                }
                break;
            case SDL_MOUSEMOTION:
                Vector3D mousePosition = EventContainer::GetMouseMovement();
                RotateView(mousePosition);
                break;
        }
    }

    void Update(double deltaTime) {
        double speed = movementSpeed * deltaTime;

        if (keyW)
            Move(front * speed);
        if (keyS)
            Move(front * -speed);
        if (keyD)
            Move(side * speed);
        if (keyA)
            Move(side * -speed);
        if (keySpace)
            Move(up * speed);
        if (keyCtrl)
            Move(up * -speed);

        if (updateCameraAngle) {
            front = Vector3D::Front();
            rotationMatrix.LoadRotationX(angleX);
            front = rotationMatrix * front;
            rotationMatrix.LoadRotationY(angleY);
            front = rotationMatrix * front;

            target = position + front;
        }
    }

    void RotateView(Vector3D mousePosition) {
        angleY -= mousePosition.x * sensitivity;

        double angleXtmp = angleX - mousePosition.y * sensitivity;
        if (angleXtmp <= 0.8 && angleXtmp >= -0.8) {
            angleX = angleXtmp;
        }

        updateCameraAngle = true;
    }
};
}  // namespace GEngine
#endif