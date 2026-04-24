#include <iostream>

#include "SDL3/SDL.h"

const int WindowWidth = 800;
const int WindowHeight = 600;
SDL_Window* window = nullptr;

SDL_Renderer* renderer = nullptr;
bool f_running = true;

void initSDL()
{
    window = SDL_CreateWindow("TrabzonCaydanligi", WindowWidth, WindowHeight, NULL);

    if (window == nullptr)
    {
        std::cout << "HATA:: Pencere olusturulamadi\n";
        f_running = false;
    }

    renderer = SDL_CreateRenderer(window, NULL);

    if (renderer == nullptr)
    {
        std::cout << "HATA:: Renderer olusturulamadi\n";
        f_running = false;
    }

}

int main()
{
    initSDL();

    SDL_Event event;
    while(f_running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
            {
                f_running = false;
            }

            switch (event.key.key)
            {
            case SDLK_ESCAPE:
                f_running = false;
                break;
            }
        }
    }
}