#include <iostream>
#include <numbers>

#include "SDL3/SDL.h"

#include "Graphics.h"
#include "Defs.h"

RenderContext rcontext;
Graphics gp(rcontext);

bool f_running = true;

float x = 400, y = 400;
float alfa = 0;


float degToRad(float deg)
{
    return deg * std::numbers::pi / 180.0;
}

float radToDeg(float rad)
{
    return rad * 180.0 / std::numbers::pi;
}

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

void update()
{
    // float x2 = x * cos(theta) - y * sin(theta);
    // float y2 = x * sin(theta) + y * cos(theta);

    //ekranin ortasi
    float cx = rcontext.WindowWidth / 2.0f;
    float cy = rcontext.WindowHeight / 2.0f;

    //noktayi ekranin ortasina tasi
    float px = x - cx;
    float py = y - cy;

    //dondur
    float rx = px * cos(alfa) - py * sin(alfa);
    float ry = px * sin(alfa) + py * cos(alfa);

    //noktayi ekran uzayina geri donustur
    x = rx + cx;
    y = ry + cy;


    //usengeclikten f(x) => alfa++ => g(x)
    //normalde alfa += RADIAN_D90 gibi bisey yazilmasi daha iyi olur
    alfa = radToDeg(alfa);

    alfa += 0.00001;

    alfa = degToRad(alfa);
}

void draw()
{
    //------------------------------//
    SDL_RenderClear(rcontext.renderer);

    gp.clearColorBuffer(Color::BLACK);

    gp.drawPixel((int)x, (int)y, Color::WHITE);

    gp.drawLine(0, rcontext.WindowHeight / 2, rcontext.WindowWidth, rcontext.WindowHeight / 2, Color::GREEN);

    gp.drawLine(rcontext.WindowWidth / 2,0, rcontext.WindowWidth / 2, rcontext.WindowHeight, Color::GREEN);

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

        update();

        draw();                        
    }
}