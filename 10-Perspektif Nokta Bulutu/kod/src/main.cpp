#include <iostream>
#include <numbers>
#include <vector>
#include <string>

#include "SDL3/SDL.h"

#include "imgui.h"
#include "backends/imgui_impl_sdl3.h"
#include "backends/imgui_impl_sdlrenderer3.h"

#include "Graphics.h"
#include "Defs.h"
#include "math/Vector2.h"
#include "math/Vector3.h"

RenderContext rcontext;
Graphics gp(rcontext);

ImGuiIO* io;

struct Camera
{
    Vector3 position = {0.0f, 0.0f, -3.0f};
    //float alfa = 0.0f;
};

Camera camera;

bool f_running = true;
bool f_proj = true;

//====================================//
float mouseX = 0, mouseY = 0;

int screenWidth, screenHeight;
//====================================//

std::vector<Vector3> modelPoints;
std::vector<Vector2> projectedPoints;

float FOV_factor = 300;

void loadCube()
{
    float incz = 0.20f;
    float incy = 0.20f;
    float incx = 0.20f;

    for (float z = -1; z <= 1.0f; z += incz)
    {
        for (float y = -1; y <= 1.0f; y += incy)
        {
            for (float x = -1; x <= 1.0f; x+= incx)
            {
                Vector3 vec(x, y, z);

                modelPoints.push_back(vec);
            }
        }
    }    
}

Vector2 projectOrtho(Vector3 vec)
{
    return Vector2
    { 
        vec.x * FOV_factor,
        vec.y * FOV_factor
    };
}

Vector2 projectPerspective(Vector3 vec)
{
    return Vector2
    {
        (vec.x * FOV_factor) / vec.z,
        (vec.y * FOV_factor) / vec.z
    };
}

Vector2 project(Vector3 vec)
{
    if (f_proj)
    {
        return projectPerspective(vec);
    }
    return projectOrtho(vec);
}


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

    if (!SDL_GetWindowSize(rcontext.window, &screenWidth, &screenHeight))
    {
        std::cout << "hata var";
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

    loadCube();
    projectedPoints.resize(modelPoints.size());

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

        case SDLK_W:           
            //camera.position.y -= moveSpeed;
            break;
        case SDLK_S:
            //camera.position.y += moveSpeed;
            break;

        case SDLK_A:
            //camera.position.x -= moveSpeed;
            break;
        case SDLK_D:
            //camera.position.x += moveSpeed;
            break;
        }
    }

    SDL_GetMouseState(&mouseX, &mouseY);
}

void update()
{        
    for (size_t i = 0; i < modelPoints.size(); i++)
    {
        Vector3 point = modelPoints[i];

        point.z -= camera.position.z;

        projectedPoints[i] = project(point);

        projectedPoints[i].x += rcontext.WindowWidth / 2;
        projectedPoints[i].y += rcontext.WindowHeight / 2;
    }
}

void drawImgui()
{
    ImGui_ImplSDLRenderer3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();
    
    ImGui::Begin("Kontrol Paneli");
    
    ImGui::Checkbox("Perspektif", &f_proj);

    ImGui::NewLine();
    ImGui::Text("Ekran mx,my %f , %f",mouseX , mouseY);
  
    ImGui::End();

    ImGui::Render();
    ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), rcontext.renderer);
}

void draw()
{
    //------------------------------//    

    gp.clearColorBuffer(Color::BLACK);    

    //gp.drawDots(Color::GREEN);

    for (size_t i = 0; i < projectedPoints.size(); i++)
    {
        gp.drawFilledRectangle(
            projectedPoints[i].x,
            projectedPoints[i].y,
            3,
            3,
            Color::GREEN
        );
    }

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