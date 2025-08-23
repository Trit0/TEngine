#ifndef RENDERER3D_HPP
#define RENDERER3D_HPP

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#include "Window.hpp"

namespace GEngine {
class Renderer3D : public Window {
   private:
    SDL_GLContext glContext;

   public:
    Renderer3D(const int& w = 640, const int& h = 480, const char* title = "", const int& x = SDL_WINDOWPOS_CENTERED, const int& y = SDL_WINDOWPOS_CENTERED, const unsigned int& windowFlags = 0) : Window(title, x, y, w, h, windowFlags | SDL_WINDOW_OPENGL) {
        this->glContext = SDL_GL_CreateContext(this->sdlWindow);
    }

    ~Renderer3D() {
        SDL_GL_DeleteContext(this->glContext);
    }

    void RenderUpdate() {
        if (cursorMode) {
            SDL_GL_SwapWindow(this->sdlWindow);
        }
    }
};
}  // namespace GEngine

#endif