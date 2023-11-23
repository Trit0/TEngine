#ifndef MATH_UTILS_HPP
#define MATH_UTILS_HPP

#include <math.h>

namespace GEngine {
class MathUtils {
   public:
    static double RadToDeg(double angleRad) {
        return (angleRad * 180) / M_PI;
    }

    static double DegToRad(double angleDeg) {
        return (angleDeg * M_PI) / 180;
    }

    static double Clamp(double value, double min, double max) {
        if (value > max) {
            value = max;
        } else if (value < min) {
            value = min;
        }
        return value;
    }
};

}  // namespace GEngine

#endif
