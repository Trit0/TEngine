#ifndef TRANSFORMER_HPP
#define TRANSFORMER_HPP

#include "../core/Component.hpp"
#include "../core/Transform.hpp"
#include "../math/MathUtils.hpp"
#include "../math/Matrix44D.hpp"

using namespace GEngine;

namespace GEngineExample {
class Transformer : public Component {
   public:
    double cumulative = 0;
    Transformer() {}

    void Update(double deltaTime) {
        double angle = 1;
        double maxAngle = 400000000000;
        // GetGameObjectTransform()->Translate(0.001, 0, 0);
        if (cumulative < maxAngle) {
            GetGameObjectTransform()->Rotate(0, 0, angle);
            cumulative += std::min(angle, maxAngle - cumulative);
            Debugger::GetInstance().Set("rot", "Rotation: " + GetGameObjectTransform()->GetRotation().toString());
        }
    }
};
}  // namespace GEngineExample

#endif
