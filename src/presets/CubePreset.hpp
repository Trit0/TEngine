#ifndef CUBE_PRESET_HPP
#define CUBE_PRESET_HPP

#include "../components/ColoredMesh.hpp"
#include "../components/TexturedMesh.hpp"
#include "../core/GameObject.hpp"
#include "../core/Transform.hpp"
#include "../scripts/Transformer.hpp"

namespace GEngine {
class CubePreset : public GameObject {
   public:
    CubePreset(Transform* transform = new Transform()) : GameObject(transform, {}, {new ColoredMesh("../res/box.obj", Color(111, 78, 55, (Uint8)255)), new BoxCollider()}) {}
};
}  // namespace GEngine

#endif
