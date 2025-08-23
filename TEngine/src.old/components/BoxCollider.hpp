#ifndef BOX_COLLIDER_HPP
#define BOX_COLLIDER_HPP

#include "../debug/Debugger.hpp"
#include "../math/Matrix44D.hpp"
#include "../math/Vector3D.hpp"
#include "../physics/PhysicsConstraint.hpp"
#include "../resources/Color.hpp"
#include "Collider.hpp"
#include "ColoredMesh.hpp"
#include "Ray.hpp"

namespace GEngine {
class BoxCollider : public Collider {
   public:
    BoxCollider(bool isTrigger = false) : Collider(new ColoredMesh("../res/box.obj", Color(0, 1, 0, 1.0)), isTrigger) {}

    void DetectInteractions(vector<PhysicsComponent*> others, int index) {
        for (int i = index + 1; i < others.size(); i++) {
            Collider* collider = (Collider*)others[i];
            Vector3D collision = DetectCollision(collider);
            if (!isTrigger && collision.GetNorm() > 0) {
                collider->GetGameObjectTransform()->Translate(collision);
            }
        }
    }

    Vector3D DetectCollision(Collider* collider) {
        Vector3D pos1 = this->GetGameObjectTransform()->GetPosition();
        Vector3D pos2 = collider->GetGameObjectTransform()->GetPosition();

        Vector3D distance = pos2 - pos1;
        Vector3D dNormalized = distance.GetNormalized();

        Vector3D scale1 = this->GetGameObjectTransform()->GetScale();
        Vector3D scale2 = collider->GetGameObjectTransform()->GetScale();

        Vector3D rotation1 = this->GetGameObjectTransform()->GetRotationRad();
        Vector3D rotation2 = collider->GetGameObjectTransform()->GetRotationRad();

        Vector3D scale1Rotated = Vector3D(scale1);
        Vector3D scale2Rotated = Vector3D(scale2);

        Vector3D right1 = Vector3D((scale1.x * 0.5), 0, 0);
        Vector3D up1 = Vector3D(0, (scale1.y * 0.5), 0);
        Vector3D front1 = Vector3D(0, 0, (scale1.z * 0.5));
        Vector3D left1 = Vector3D((-scale1.x * 0.5), 0, 0);
        Vector3D down1 = Vector3D(0.0, (-scale1.y * 0.5), 0.0);
        Vector3D back1 = Vector3D(0, 0, (-scale1.z * 0.5));

        Vector3D* faces1[] = {&right1, &up1, &front1, &left1, &down1, &back1};

        Vector3D right2 = Vector3D((scale2.x * 0.5), 0, 0);
        Vector3D up2 = Vector3D(0, (scale2.y * 0.5), 0);
        Vector3D front2 = Vector3D(0, 0, (scale2.z * 0.5));
        Vector3D left2 = Vector3D((-scale2.x * 0.5), 0, 0);
        Vector3D down2 = Vector3D(0.0, (-scale2.y * 0.5), 0.0);
        Vector3D back2 = Vector3D(0, 0, (-scale2.z * 0.5));

        Vector3D* faces2[] = {&right2, &up2, &front2, &left2, &down2, &back2};

        if (rotation1.GetNorm() > 0) {
            Matrix44D rotation;
            if (rotation1.x != 0) {
                rotation.LoadRotationX(rotation1.x);
                scale1Rotated = rotation * scale1Rotated;
                for (Vector3D* face : faces1) {
                    *face = rotation * *face;
                }
            }

            if (rotation1.y != 0) {
                rotation.LoadRotationY(rotation1.y);
                scale1Rotated = rotation * scale1Rotated;
                for (Vector3D* face : faces1) {
                    *face = rotation * *face;
                }
            }

            if (rotation1.z != 0) {
                rotation.LoadRotationZ(rotation1.z);
                scale1Rotated = rotation * scale1Rotated;
                for (Vector3D* face : faces1) {
                    *face = rotation * *face;
                }
            }
        }

        if (rotation2.GetNorm() > 0) {
            Matrix44D rotation;
            if (rotation2.x != 0) {
                rotation.LoadRotationX(rotation2.x);
                scale2Rotated = rotation * scale2Rotated;
                for (Vector3D* face : faces2) {
                    *face = rotation * *face;
                }
            }

            if (rotation2.y != 0) {
                rotation.LoadRotationY(rotation2.y);
                scale2Rotated = rotation * scale2Rotated;
                for (Vector3D* face : faces2) {
                    *face = rotation * *face;
                }
            }

            if (rotation2.z != 0) {
                rotation.LoadRotationZ(rotation2.z);
                scale2Rotated = rotation * scale2Rotated;
                for (Vector3D* face : faces2) {
                    *face = rotation * *face;
                }
            }
        }

        for (Vector3D* face : faces1) {
            *face = *face + pos1;
        }

        for (Vector3D* face : faces2) {
            *face = *face + pos2;
        }

        Vector3D testX = Vector3D::Right() * (Vector3D::Right() * scale1Rotated);
        Vector3D testY = Vector3D::Up() * (Vector3D::Up() * scale1Rotated);
        Vector3D testZ = Vector3D::Back() * (scale1Rotated * Vector3D::Back());

        Debugger::GetInstance().SetRenderComponent("testRayX1", new GEngine::Transform(left1), new Ray(right1, Color(0.8, 0.2, 0, 1)));
        Debugger::GetInstance().SetRenderComponent("testRayY1", new GEngine::Transform(down1), new Ray(up1, Color(0.2, 0.8, 0.2, 1)));
        Debugger::GetInstance().SetRenderComponent("testRayZ1", new GEngine::Transform(back1), new Ray(front1, Color(0.2, 0.2, 0.8, 1)));

        Debugger::GetInstance().SetRenderComponent("testRayX2", new GEngine::Transform(left2), new Ray(right2, Color(0.8, 0.2, 0, 1)));
        Debugger::GetInstance().SetRenderComponent("testRayY2", new GEngine::Transform(down2), new Ray(up2, Color(0.2, 0.8, 0.2, 1)));
        Debugger::GetInstance().SetRenderComponent("testRayZ2", new GEngine::Transform(back2), new Ray(front2, Color(0.2, 0.2, 0.8, 1)));

        // Vector3D p = pos1 + Vector3D(0, 1, 1) * 0.5;
        // Debugger::GetInstance().SetRenderComponent("testRayX2", new Ray(p - (scale1.JustX() * 0.5), p + (scale1.JustX() * 0.5), Color(0.8, 0.2, 0, 1)));
        // Debugger::GetInstance().SetRenderComponent("testRayY2", new Ray(p - (scale1.JustY() * 0.5), p + (scale1.JustY() * 0.5), Color(0.2, 0.8, 0.2, 1)));
        // Debugger::GetInstance().SetRenderComponent("testRayZ2", new Ray(p - (scale1.JustZ() * 0.5), p + (scale1.JustZ() * 0.5), Color(0.2, 0.2, 0.8, 1)));

        double radius1 = abs(scale1Rotated * dNormalized);
        double radius2 = abs(scale2Rotated * dNormalized);
        // if (rotation1.GetNorm() > 0) {
        //     radius1 = RotateRadius2(dNormalized, rotation1) * radius1;
        // }

        // if (rotation2.GetNorm() > 0) {
        //     radius2 = RotateRadius2(dNormalized, rotation2) * radius2;
        // }

        double dNorm = distance.GetNorm();
        double r = (radius1 + radius2) * 0.5;

        // TEST
        // Debugger::GetInstance().Set("ub", "Position 1 Upper limit: " + to_string(pos1.y + (scale1.y * 0.5)));
        // Debugger::GetInstance().Set("lb", "Position 2 Lower limit: " + to_string(pos2.y - (scale2.y * 0.5)));
        // Debugger::GetInstance().Set("pos2", "Position 2: " + pos2.toString());

        // Debugger::GetInstance().Set("pos1", "Position 1: " + pos1.toString());
        // Debugger::GetInstance().Set("pos2", "Position 2: " + pos2.toString());
        // Debugger::GetInstance().Set("scale1", "Scale1 1: " + scale1.toString());
        // Debugger::GetInstance().Set("scale2", "Scale2 2: " + scale2.toString());

        if (dNorm > r) {
            // Debugger::GetInstance().Set("return", "Return: Start");
            return Vector3D::Zero();
        }

        Vector3D hitFace;
        // for (Vector3D* face : faces1) {
        //     Vector3D faceNormalized = (*face - pos1).GetNormalized();
        //     Vector3D faceFar = *face + faceNormalized * 0.1;

        //     Vector3D faceDistance = *face - pos2;
        //     Vector3D faceFarDistance = faceFar - pos2;

        //     if (faceFarDistance.GetNorm() < faceDistance.GetNorm()) {
        //         hitFace = *face;
        //     }
        // }

        int i = 1;
        for (Vector3D* face : faces1) {
            Vector3D faceDistance = *face - pos1;
            Vector3D hitFaceDistance = hitFace - pos1;

            Vector3D ortho1 = faceDistance % distance;
            Vector3D ortho2 = hitFaceDistance % distance;

            // Debugger::GetInstance().SetRenderComponent("testRayDistance" + to_string(i), new GEngine::Transform(*face), new Ray(pos2, Color(0.8, 0.2, i * 0.1, 1)));
            // Debugger::GetInstance().SetRenderComponent("face" + to_string(i), new GEngine::Transform(*face, Vector3D::Zero(), Vector3D::Identity() * 0.25), new ColoredMesh("../res/sphere.obj", Color(i * 0.1, 1, 1, 1.0)));

            Vector3D faceDistance2 = *face - pos2;
            Vector3D hitFaceDistance2 = hitFace - pos2;

            if (ortho1.GetNorm() <= ortho2.GetNorm() || (ortho1.GetNorm() == ortho2.GetNorm() && faceDistance2.GetNorm() < hitFaceDistance2.GetNorm())) {
                hitFace = *face;
            }
            i++;
        }

        // for (Vector3D* face : faces1) {
        //     Vector3D faceDistance = *face - pos1;
        //     Vector3D hitFaceDistance = hitFace - pos1;

        //     Vector3D ortho1 = faceDistance % distance;
        //     Vector3D ortho2 = hitFaceDistance % distance;

        //     // Debugger::GetInstance().SetRenderComponent("testRayDistance" + to_string(i), new GEngine::Transform(*face), new Ray(pos2, Color(0.8, 0.2, i * 0.1, 1)));
        //     // Debugger::GetInstance().SetRenderComponent("face" + to_string(i), new GEngine::Transform(*face, Vector3D::Zero(), Vector3D::Identity() * 0.25), new ColoredMesh("../res/sphere.obj", Color(i * 0.1, 1, 1, 1.0)));

        //     if (ortho1.GetNorm() <= ortho2.GetNorm()) {
        //         // Vector3D tmp = (*face - pos1).GetNormalized();
        //         // Vector3D far1 = *face + tmp;
        //         // Vector3D tmp2 = (hitFace - pos1).GetNormalized();
        //         // Vector3D far2 = hitFace + tmp2;
        //         // Vector3D farDistance1 = far1 - pos2;
        //         // Vector3D farDistance2 = far2 - pos2;
        //         faceDistance = *face - pos2;
        //         hitFaceDistance = hitFace - pos2;

        //         if (faceDistance.GetNorm() < hitFaceDistance.GetNorm()) {
        //             hitFace = *face;
        //         }
        //     }
        //     i++;
        // }

        // Vector3D upOrtho = up1 % distance;
        // Vector3D downOrtho = down1 % distance;

        // Debugger::GetInstance().SetRenderComponent("uportho", new GEngine::Transform(up1), new Ray(up1 + upOrtho, Color(0.8, 0.2, 0.1, 1)));
        // Debugger::GetInstance().SetRenderComponent("downortho", new GEngine::Transform(down1), new Ray(down1 + downOrtho, Color(0.8, 0.2, 0.9, 1)));

        // Debugger::GetInstance().Set("upjsp", "Up ortho: " + to_string(upOrtho.GetNorm()));

        // Debugger::GetInstance().Set("downjsp", "Down ortho: " + to_string(downOrtho.GetNorm()));

        // if (distance.x != 0) {
        //     int compX = distance.x / abs(distance.x);

        //     Vector3D xFace = compX > 0 ? right1 : left1;
        //     Vector3D xFaceNormalized = xFace.GetNormalized();
        //     Vector3D xFaceFar = xFace + xFaceNormalized * 0.1;

        //     Vector3D xFaceDistance = xFace - pos2;
        //     Vector3D xFaceFarDistance = xFaceFar - pos2;

        //     isOnX = xFaceFarDistance.GetNorm() < xFaceDistance.GetNorm();
        // }

        // if (distance.y != 0) {
        //     int compY = distance.y / abs(distance.y);
        //     Vector3D yFace = Vector3D(pos1.x, (pos1.y + (scale1Rotated.y * 0.5)) * compY, pos1.z);
        //     Vector3D yFaceFar = Vector3D(yFace.x, yFace.y + (0.1 * compY), yFace.z);

        //     Vector3D yFaceDistance = yFace - pos2;
        //     Vector3D yFaceFarDistance = yFaceFar - pos2;

        //     isOnY = yFaceFarDistance.GetNorm() < yFaceDistance.GetNorm();
        // }

        // if (distance.z != 0) {
        //     int compZ = distance.z / abs(distance.z);
        //     Vector3D zFace = Vector3D(pos1.x, pos1.y, (pos1.z + (scale1Rotated.z * 0.5)) * compZ);
        //     Vector3D zFaceFar = Vector3D(zFace.x, zFace.y, zFace.z + (0.1 * compZ));

        //     Vector3D zFaceDistance = zFace - pos2;
        //     Vector3D zFaceFarDistance = zFaceFar - pos2;

        //     isOnZ = zFaceFarDistance.GetNorm() < zFaceDistance.GetNorm();
        // }

        Debugger::GetInstance().SetRenderComponent("hitFace", new GEngine::Transform(hitFace, Vector3D::Zero(), Vector3D::Identity() * 0.25), new ColoredMesh("../res/sphere.obj", Color(1, 1, 1, 1.0)));

        Vector3D normalOrientation1 = (pos1 - hitFace).GetNormalized();
        Vector3D normalOrientation2 = (pos2 - hitFace).GetNormalized();

        // Vector3D newDistance = Vector3D(isOnX ? distance.x : 0, isOnY ? distance.y : 0, isOnZ ? distance.z : 0);
        Vector3D newDistance = normalOrientation1 * (normalOrientation1 * distance);
        Vector3D newDistanceNormalized = newDistance.GetNormalized();

        Debugger::GetInstance().SetRenderComponent("testRayDistance", new GEngine::Transform(pos1), new Ray(pos2, Color(1.0, 0.5, 1.0, 1)));
        Debugger::GetInstance().SetRenderComponent("testRayDistance26", new GEngine::Transform(pos1), new Ray(pos1 + newDistance, Color(0.8, 0.8, 0.8, 1)));

        double newRadius1 = abs(scale1Rotated * normalOrientation1);
        double newRadius2 = abs(scale2Rotated * normalOrientation2);
        // if (rotation1.GetNorm() > 0) {
        //     newRadius1 = RotateRadius2(newDistanceNormalized, rotation1) * newRadius1;
        // }
        // if (rotation2.GetNorm() > 0) {
        //     newRadius2 = RotateRadius2(newDistanceNormalized, rotation2) * radius2;
        // }

        double newDistanceNorm = newDistance.GetNorm();
        double newR = (newRadius1 + newRadius2) * 0.5;

        // Debugger::GetInstance().Set("distance", "Distance: " + to_string(newDistanceNorm));
        // Debugger::GetInstance().Set("dNormalized", "Distance normlized: " + newDistanceNormalized.toString());
        // Debugger::GetInstance().Set("radius", "Radius: " + to_string(newR));

        // Vector3D closestFace;
        // int i = 0;
        // for (Vector3D face : faces) {
        //     if (i == 0 || (face * distance) > ((closestFace * distance))) {
        //         closestFace = face;
        //     }
        //     i++;
        // }

        if (newDistanceNorm > newR) {
            // Debugger::GetInstance().Set("return", "Return: Before End");
            return Vector3D::Zero();
        }

        double overlap = 0;

        overlap = (newR - newDistanceNorm);
        Debugger::GetInstance().Set("overlap", "Overlap: " + to_string(overlap));

        // Debugger::GetInstance().Set("return", "Return: End");

        return newDistanceNormalized * overlap;
    }

    double RotateRadius2(Vector3D radius, Vector3D rotation) {
        Vector3D angleVec = Vector3D(acos(radius.x), acos(radius.y), acos(radius.z));
        Vector3D jsp = angleVec - (rotation * 2);
        Vector3D newVec = Vector3D(cos(jsp.x), cos(jsp.y), cos(jsp.z));
        return newVec.GetNorm();
    }

    double RotateRadius(double radius, Vector3D rotation) {
        double angleDroitRad = MathUtils::DegToRad(90);

        if (rotation.x != 0) {
            radius = radius / sin(rotation.x) * sin(angleDroitRad);
        }
        if (rotation.y) {
            radius = radius / sin(rotation.y) * sin(angleDroitRad);
        }
        if (rotation.z) {
            radius = radius / sin(rotation.z) * sin(angleDroitRad);
        }
        return radius;
    }

    void Evaluate(double deltaTime) {
        for (PhysicsConstraint constraint : constraints) {
            constraint.Apply(this, deltaTime);
        }
        for (PhysicsConstraint constraint : temporaryConstraints) {
            constraint.Apply(this, deltaTime);
        }
        this->temporaryConstraints.clear();
    }

    void Draw() {
        RenderConfig config;
        config.drawLines = true;
        this->mesh->Render(config);
    }

    void ApplyConstraint(PhysicsConstraint constraint) {
        this->temporaryConstraints.push_back(constraint);
    }

    void Translate(const Vector3D& translation) {
        this->mesh->Translate(translation);
    }

    void Transform(const Matrix44D& matrix, const Vector3D& position) {
        this->mesh->Transform(matrix, position);
    }
};

}  // namespace GEngine

#endif
