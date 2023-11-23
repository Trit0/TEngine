/**
 * @file EventContainer.hpp
 * @author Trito
 * @brief
 * @version 0.1
 * @date 2021-09-22
 *
 * @copyright Copyright (c) 2021
 *
 */
#ifndef EVENTCONTAINER_HPP
#define EVENTCONTAINER_HPP

#include <SDL2/SDL.h>

#include "math/Vector3D.hpp"

namespace GEngine {
class EventContainer {
   private:
    static SDL_Event sdlEvent;

   public:
    static unsigned int GetType() {
        return sdlEvent.type;
    }

    static void ManageEvents() {
    }

    static int Poll() {
        return SDL_PollEvent(&sdlEvent);
    }

    static unsigned int GetKey() {
        return sdlEvent.key.keysym.sym;
    }

    static Vector3D GetMouseMovement() {
        return Vector3D((double)(sdlEvent.motion.xrel), (double)(-sdlEvent.motion.yrel), 0.0);
    }

    static double GetAngle() {
        Vector3D v = Vector3D((double)sdlEvent.motion.x, (double)sdlEvent.motion.y, 0.0);
        Vector3D b = GetMouseMovement();
        return v.GetAngle(b);
    }
};
SDL_Event EventContainer::sdlEvent = SDL_Event();
}  // namespace GEngine

#endif