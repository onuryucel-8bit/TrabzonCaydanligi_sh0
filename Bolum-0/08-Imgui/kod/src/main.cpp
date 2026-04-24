#include <iostream>
#include <numbers>
#include <vector>

#include "SDL3/SDL.h"

#include "imgui.h"
#include "backends/imgui_impl_sdl3.h"
#include "backends/imgui_impl_sdlrenderer3.h"

#include "Graphics.h"
#include "Defs.h"
#include "math/Vector2.h"

RenderContext rcontext;
Graphics gp(rcontext);

ImGuiIO* io;

Vector2 trigStatic[3] = { {390,290} , {370,250}, {350,290} };
Vector2 trig[3] = { {390,290} , {370,250}, {350,290} };

bool f_running = true;

float alfa = 0;

Vector2 trigTranslate(0, 0);
Vector2 trigScale(1.0f, 1.0f);

float degToRad(float deg)
{
    return deg * std::numbers::pi / 180.0;
}

float radToDeg(float rad)
{
    return rad * 180.0 / std::numbers::pi;
}

void initImgui()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

     io = &ImGui::GetIO();

    // Enable Docking
    io->ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io->FontGlobalScale = 1;

    ImGui_ImplSDL3_InitForSDLRenderer(rcontext.window, rcontext.renderer);
    ImGui_ImplSDLRenderer3_Init(rcontext.renderer);
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
        ImGui_ImplSDL3_ProcessEvent(&event);

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

    //ekranin ortasi
    float cx = rcontext.WindowWidth / 2.0f;
    float cy = rcontext.WindowHeight / 2.0f;

    for (size_t i = 0; i < 3; i++)
    {
        //noktayi ekranin ortasina tasi
        float px = trigStatic[i].x - cx;
        float py = trigStatic[i].y - cy;

        //buyut/kucult
        px *= trigScale.x;
        py *= trigScale.y;

        //dondur
        float rx = px * cos(alfa) - py * sin(alfa);
        float ry = px * sin(alfa) + py * cos(alfa);

        //tasi
        rx += trigTranslate.x;
        ry += trigTranslate.y;

        //noktayi ekran uzayina geri donustur
        trig[i].x = rx + cx;
        trig[i].y = ry + cy;
    }   
}

void drawImgui()
{
    ImGui_ImplSDLRenderer3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();
    
    ImGui::Begin("Dondurme Zimbirtisi");

    ImGui::NewLine();
    ImGui::SliderFloat("dondurme", &alfa, 0, std::numbers::pi * 2);

    ImGui::NewLine();
    ImGui::SliderFloat("x", &trigTranslate.x, -rcontext.WindowWidth, rcontext.WindowWidth);

    ImGui::NewLine();
    ImGui::SliderFloat("y", &trigTranslate.y, -rcontext.WindowHeight, rcontext.WindowHeight);

    ImGui::NewLine();
    ImGui::SliderFloat("sx", &trigScale.x, -5, 5);

    ImGui::NewLine();
    ImGui::SliderFloat("sy", &trigScale.y, -5, 5);

    ImGui::End();

    ImGui::Render();
    ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), rcontext.renderer);
}

void draw()
{
    //------------------------------//    

    gp.clearColorBuffer(Color::BLACK);

    gp.drawTriangle(
        trig[0].x, trig[0].y,
        trig[1].x, trig[1].y,
        trig[2].x, trig[2].y,
        Color::WHITE);

    gp.drawLine(0, rcontext.WindowHeight / 2, rcontext.WindowWidth, rcontext.WindowHeight / 2, Color::GREEN);

    gp.drawLine(rcontext.WindowWidth / 2,0, rcontext.WindowWidth / 2, rcontext.WindowHeight, Color::GREEN);

    gp.drawDots(Color::GREEN);

    gp.drawColorBuffer();
    
    //------------------------------//
}

int main()
{
    
    initSDL();
    initImgui();

    rcontext.colorBuffer = new Color_t[rcontext.WindowWidth * rcontext.WindowHeight];
    
    while(f_running)
    {
        inputs();

        update();

        SDL_RenderClear(rcontext.renderer);
        drawImgui();
        draw();
        //swap buffers
        SDL_RenderPresent(rcontext.renderer);
    }
}