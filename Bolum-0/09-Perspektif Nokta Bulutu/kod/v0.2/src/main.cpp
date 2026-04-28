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
    //Kamerayi kup bulutundan 3 birim geriye aliyoruz
    Vector3 position = {0.0f, 0.0f, -3.0f};
    float FOV_factor = 300;
};

Camera camera;

Vector3 position(0, 0, 0);
Vector3 scale(1.0f, 1.0f, 1.0f);
Vector3 rotate(0, 0, 0);

Color_t cubeColor = Color::WHITE;

bool f_running = true;

enum class ProjectMod
{
    Ortho,
    Perspective
};

ProjectMod projectMod = ProjectMod::Perspective;

//====================================//
float mouseX = 0, mouseY = 0;

int screenWidth, screenHeight;
//====================================//

std::vector<Vector3> modelPoints;
std::vector<Vector2> projectedPoints;

void loadCube(float inc)
{   
    modelPoints.clear();

    float incz = inc;
    float incy = inc;
    float incx = inc;

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
    //FOV_factor: gelen koordinatlar -1,1 gibi dar bir aralikta olucagi icin
    //bu degerleri ekrana uygun sekilde buyutmemiz gerekiyor
    return Vector2
    { 
        vec.x * camera.FOV_factor,
        vec.y * camera.FOV_factor
    };
}

Vector2 projectPerspective(Vector3 vec)
{
    return Vector2
    {
        (vec.x * camera.FOV_factor) / vec.z,
        (vec.y * camera.FOV_factor) / vec.z
    };
}

Vector2 project(Vector3 vec)
{
    if (projectMod == ProjectMod::Perspective)
    {
        return projectPerspective(vec);
    }
    return projectOrtho(vec);
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
    
    //Ciktiyi piksellestir
    SDL_SetTextureScaleMode(rcontext.canvas, SDL_SCALEMODE_NEAREST);    
}

//----------------------------------------------------------------------------//
//----------------------------------------------------------------------------//
//----------------------------------------------------------------------------//

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
    projectedPoints.clear();

    for (size_t i = 0; i < modelPoints.size(); i++)
    {
        Vector3 point = modelPoints[i];
                                                             
        //vektor carpimi ekle a * b operator*(...);
        point.x = point.x * scale.x;
        point.y = point.y * scale.y;
        point.z = point.z * scale.z;

        point = point.rotateX(rotate.x);
        point = point.rotateY(rotate.y);
        point = point.rotateZ(rotate.z);

        point = point + position;

        //Noktalari 3 birim kameradan uzaklastiriyoruz
        point.z -= camera.position.z;

        //Ekrana nokta bulutunun noktalari yansitiliyor
        Vector2 projPoint = project(point);

        //Yansitilan noktalar ekrana ortalaniyor
        projPoint.x += rcontext.WindowWidth / 2;
        projPoint.y += rcontext.WindowHeight / 2;

        projectedPoints.emplace_back(projPoint);
    }
}

void drawImgui()
{
    ImGui_ImplSDLRenderer3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();
    

    //------------------------------------------------------------------//
    //------------------------------------------------------------------//
    //------------------------------------------------------------------//
    ImGui::Begin("Kamera");
    
    ImGui::RadioButton("Perspektif", (int*)&projectMod, (int)ProjectMod::Perspective);
    ImGui::RadioButton("Ortografik", (int*)&projectMod, (int)ProjectMod::Ortho);

    ImGui::Text("Ekran Boyutu (%i, %i)", screenWidth, screenHeight);

    ImGui::NewLine();
    ImGui::Text("Fare fx,fy (%f, %f)",mouseX , mouseY);

    ImGui::SliderFloat("FOV", &camera.FOV_factor, 0, 600);    
  
    ImGui::End();

    //------------------------------------------------------------------//
    //------------------------------------------------------------------//
    //------------------------------------------------------------------//

    ImGui::Begin("Kontrol Paneli");

    static float color[4] = { 1, 0, 1, 0 };

    ImGui::ColorEdit4("Color", color);
    ImGui::ColorPicker4("Color Picker", color);

    uint8_t r = (uint8_t)(color[0] * 255);
    uint8_t g = (uint8_t)(color[1] * 255);
    uint8_t b = (uint8_t)(color[2] * 255);
    uint8_t a = (uint8_t)(color[3] * 255);
    
    cubeColor = (0xff << 24) | (r << 16) | (g << 8) | b;
    
    ImGui::Text("Kup renk 0x%08X", cubeColor);

    ImGui::NewLine();

    static float cubeinc = 0.25;
    ImGui::SliderFloat("Kup Boyutu", &cubeinc, 0.01, 0.9, "%0.9f");
    
    if (ImGui::Button("Sifirla"))
    {
        loadCube(0.25f);
    }

    if (ImGui::Button("Yeni Nokta Bulutu"))
    {
        loadCube(cubeinc);
    }
    
    if (ImGui::Button("R##0"))
    {
        position.x = 0;
        position.y = 0;
        position.z = 0;
    }
    ImGui::SliderFloat("pozisyon.x", &position.x, -20, 20);
    ImGui::SliderFloat("pozisyon.y", &position.y, -20, 20);
    ImGui::SliderFloat("pozisyon.z", &position.z, -20, 20);

    if (ImGui::Button("R##1"))
    {
        scale.x = 1;
        scale.y = 1;
        scale.z = 1;
    }
    ImGui::SliderFloat("boyut.x", &scale.x, -20, 20);
    ImGui::SliderFloat("boyut.y", &scale.y, -20, 20);
    ImGui::SliderFloat("boyut.z", &scale.z, -20, 20);

    if (ImGui::Button("R##2"))
    {
        rotate.x = 0;
        rotate.y = 0;
        rotate.z = 0;
    }
    ImGui::SliderFloat("dondurme.x", &rotate.x, -8, 8);
    ImGui::SliderFloat("dondurme.y", &rotate.y, -8, 8);
    ImGui::SliderFloat("dondurme.z", &rotate.z, -8, 8);
    
    ImGui::End();
    //------------------------------------------------------------------//
    //------------------------------------------------------------------//
    //------------------------------------------------------------------//

    ImGui::Render();
    ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), rcontext.renderer);
}

void draw()
{
    //------------------------------//    

    gp.clearColorBuffer(Color::BLACK);    
    
    for (size_t i = 0; i < projectedPoints.size(); i++)
    {
        gp.drawFilledRectangle(
            projectedPoints[i].x,
            projectedPoints[i].y,
            3,
            3,
            cubeColor
        );       
    }

    gp.drawColorBuffer();
    
    //------------------------------//
}

int main()
{        
    initSDL();
    initImgui();

    loadCube(0.25f);
    projectedPoints.resize(100);

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