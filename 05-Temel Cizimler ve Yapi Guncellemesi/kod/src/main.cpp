#include <iostream>


#include "SDL3/SDL.h"

#include "Graphics.h"
#include "Defs.h"

RenderContext rcontext;
Graphics gp(rcontext);

bool f_running = true;


void initSDL()
{
    rcontext.window = SDL_CreateWindow("TrabzonCaydanligi", rcontext.WindowWidth, rcontext.WindowHeight, SDL_WINDOW_FULLSCREEN);

    if (rcontext.window == nullptr)
    {
        std::cout << "HATA:: Pencere olusturulamadi\n";
        f_running = false;
    }

    rcontext.renderer = SDL_CreateRenderer(rcontext.window, NULL);

    if (rcontext.renderer == nullptr)
    {
        std::cout << "HATA:: Renderer olusturulamadi\n";
        f_running = false;
    }

    rcontext.canvas = SDL_CreateTexture(rcontext.renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, rcontext.WindowWidth, rcontext.WindowHeight);

    if(rcontext.canvas == nullptr)
    {
        std::cout << "Error:: Texture initializing failed\n";
        //return false;
    }

    //make it pixaled
    SDL_SetTextureScaleMode(rcontext.canvas, SDL_SCALEMODE_NEAREST);
}

void inputs()
{
    SDL_Event event;
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

void draw()
{
    //------------------------------//
    SDL_RenderClear(rcontext.renderer);

    gp.clearColorBuffer(Color::BLACK);

    gp.drawFilledRectangle(100,100, 50, 70, Color::GREEN);

    gp.drawColorBuffer();

    //swap buffers
    SDL_RenderPresent(rcontext.renderer);
    //------------------------------//
}

int main()
{
    
    initSDL();

    rcontext.colorBuffer = new Color_t[rcontext.WindowWidth * rcontext.WindowHeight];
    
    while(f_running)
    {
        inputs();
       
        draw();                        
    }
}