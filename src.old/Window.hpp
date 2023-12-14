/**
 * @file Window.hpp
 * @author Trito
 * @brief Fenêtre sdl
 * @version 0.1
 * @date 2021-09-22
 *
 * @copyright Copyright (c) 2021
 *
 */
#pragma once
#include <SDL2/SDL.h>

#include <string>

#include "EventContainer.hpp"
#include "design/Observer_Observable.hpp"

using namespace std;

namespace GEngine {
class Window : public Observer {
   protected:
    SDL_Window *sdlWindow;
    bool cursorMode = true;

    virtual void RenderUpdate() = 0;

   public:
    Window(const char *title, const int &x, const int &y, const int &w, const int &h, const unsigned int &flags) {
        sdlWindow = SDL_CreateWindow(title, x, y, w, h, flags);
    }

    virtual ~Window() {
        SDL_DestroyWindow(this->sdlWindow);
    }

    void SetTitle(const char *title) {
        SDL_SetWindowTitle(sdlWindow, title);
    }

    const char *GetTitle() {
        return SDL_GetWindowTitle(sdlWindow);
    }

    void SetSize(const int &w, const int &h) {
        SDL_SetWindowSize(sdlWindow, w, h);
    }

    void CenterCursor() {
        if (cursorMode) {
            SDL_SetRelativeMouseMode(SDL_TRUE);
            SDL_Point windowSize = GetSize();
            SDL_WarpMouseInWindow(sdlWindow, windowSize.x / 2, windowSize.y / 2);
        } else {
            SDL_SetRelativeMouseMode(SDL_FALSE);
        }
    }

    void Notification() {
        switch (EventContainer::GetKey()) {
            case SDLK_ESCAPE:
                cursorMode = !cursorMode;
                break;
            default:
                break;
        }
    }

    SDL_Point GetSize() {
        SDL_Point windowSize;
        SDL_GetWindowSize(sdlWindow, &windowSize.x, &windowSize.y);
        return windowSize;
    }
};
}  // namespace GEngine