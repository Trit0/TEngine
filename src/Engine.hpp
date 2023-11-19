#ifndef ENGINE_HPP
#define ENGINE_HPP

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_main.h>
#include <SDL2/SDL_test_log.h>
#include <SDL2/SDL_ttf.h>

#include <list>

#include "Window.hpp"
#include "design/Singleton.hpp"

using namespace std;
namespace GEngine {
template <typename T>
class Engine : public Singleton<T> {
   private:
    // list<Window> windows;

   public:
    Engine() {
        SDL_Init(SDL_INIT_EVERYTHING);
        IMG_Init(IMG_INIT_PNG);
        TTF_Init();
    }

    ~Engine() {
        SDL_Quit();
        TTF_Quit();
        IMG_Quit();
    }

    virtual void Start() = 0;

    void Update() {
    }
    /*
                void CreateWindow2D(string title, int x, int y, int w, int h)
                {
                    windows.push_front(Renderer2D(title, x, y, w, h));
                }

                void CreateWindow3D(string title, int x, int y, int w, int h)
                {
                    windows.push_front(Renderer3D(title, x, y, w, h));
                }*/

    void Quit() {
    }
};
}  // namespace GEngine

#endif