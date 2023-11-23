#ifndef TEST_SCENE_HPP
#define TEST_SCENE_HPP

#include "../components/BoxCollider.hpp"
#include "../components/Collider.hpp"
#include "../components/Ray.hpp"
#include "../components/RigidBody.hpp"
#include "../components/TexturedMesh.hpp"
#include "../core/GameObject.hpp"
#include "../core/Scene.hpp"
#include "../core/Transform.hpp"
#include "../physics/PhysicsConstraint.hpp"
#include "../presets/CubePreset.hpp"
#include "Transformer.hpp"

using namespace GEngine;

namespace GEngineExample {
class TestScene : public Scene {
   public:
    TestScene() {
        GameObject monkey1 = GameObject(new Transform(Vector3D(0, 2, -5), Vector3D(0, 0, 45)), {}, {new TexturedMesh("../res/monkey.obj", "../res/crate.png"), new BoxCollider()});
        GameObject monkey2 = GameObject(new Transform(Vector3D(0, 2, -5), Vector3D(0, 0, 0), Vector3D(1, 1, 1)), {}, {new TexturedMesh("../res/monkey.obj", "../res/crate.png"), new RigidBody(0.1, Vector3D::Left() * 5), new BoxCollider()});
        GameObject monkey3 = GameObject(new Transform(Vector3D(2, 2, -5)), {}, {new TexturedMesh("../res/monkey.obj", "../res/crate.png"), new RigidBody(0.1), new BoxCollider()});
        CubePreset cube = CubePreset(new Transform(Vector3D(0, 0, -5), Vector3D(0, 0, 0), Vector3D(5, 1, 5)));
        GameObject ray = GameObject(new Transform(Vector3D(1, 1, -1)), {}, {new Ray(Vector3D(1, 2, -1), Color(0.5, 1, 0.5, 1)), new Transformer()});
        this->addGameObject(cube);
        this->addGameObject(monkey1);
        // this->addGameObject(monkey2);
        // this->addGameObject(monkey3);
        // this->addGameObject(ray);
    }
};
}  // namespace GEngineExample

#endif
