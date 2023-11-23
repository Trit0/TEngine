#ifndef CHRONO_HPP
#define CHRONO_HPP

#include <chrono>

#define NANOSECOND_TO_SECOND 1000000000.0

using namespace std::chrono;

namespace GEngine {
class Chrono {
   private:
    steady_clock::time_point lastTick;
    steady_clock::time_point lastFPSTick;

    int frame = 0;
    double frameCount = 0;

   public:
    Chrono() {
        Reset();
    }

    inline void Reset() {
        lastTick = steady_clock::now();
    }

    double EllapsedTime() {
        return duration_cast<nanoseconds>(steady_clock::now() - lastTick).count() / NANOSECOND_TO_SECOND;
    }

    double FramePerSecondInstant() {
        double ellapsedTime = EllapsedTime();
        return 1 / ellapsedTime;
    }

    double FramePerSecond(float frameReset = 1.0f) {
        if ((steady_clock::now() - lastFPSTick) >= duration<float>(frameReset)) {
            double multiplier = 1.0f / frameReset;
            frameCount = frame * multiplier;
            frame = 0;
            lastFPSTick = steady_clock::now();
        }
        frame++;
        return frameCount;
    }
};
}  // namespace GEngine

#endif