#ifndef RENDERER2D_HPP
#define RENDERER2D_HPP

#include <SDL2/SDL.h>
#include <SDL2/SDL_main.h>
#include "Window.hpp"

namespace GEngine {
    class Renderer2D : public Window {
        private:
            SDL_Renderer* sdlRenderer;

        public:
            Renderer2D(const int& w = 480, const int& h = 480, string title = "", const int& x = SDL_WINDOWPOS_CENTERED, const int& y = SDL_WINDOWPOS_CENTERED,  const unsigned int& windowFlags = 0, const unsigned int& renderFlags = 0) : Window(title, x, y, w, h, windowFlags) {
                this->sdlRenderer = SDL_CreateRenderer(this->sdlWindow, -1, renderFlags));
            }

            ~Renderer2D(){
                SDL_DestroyRenderer(this->sdlRenderer);
            }

            void RenderUpdate(){
                SDL_RenderPresent(this->sdlRenderer);
            }
    };
}

#endif