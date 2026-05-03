//https://www.thebasemesh.com

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
#include "Camera.h"
#include "Model.h"
#include "ObjReader.h"

#include "utils/Timer.h"
#include "utils/Timer_t0.h"


RenderContext rcontext;
Graphics gp(rcontext);

ImGuiIO* io;

Camera camera;

//ENUM kullan...

bool f_running = true;
bool f_proj = true;
bool f_trigs = true;
bool f_rotateCenter = false;
bool f_crect = false;
bool f_ralfa = false;


float alfa = 0;
float donusHizi = 0.00001f;


//====================================//
float mouseX = 0, mouseY = 0;

int screenWidth, screenHeight;
//====================================//

std::vector<Vector3> modelPoints;
std::vector<Face> meshFaces;

std::vector<Vector3> axisPoints;
std::vector<Triangle> renderTrigs;
std::vector<Vector2> axisProjectPoints;

Vector3 position(0, 0, 0);
Vector3 scale(1.0f, 1.0f, 1.0f);
Vector3 rotate(0, 0, 0);
Vector3 center(-3.0f, -3.0f, -3.0f);


ObjReader objr;
int currentModel = 0;
int lastModel = 0;
const char* models[] = {
    "kasa.obj",
    "kup.obj",
    "tuzluk.obj",
    "kale.obj",
    "cardboard_box_02.obj",
    //"canned_food_01.obj", hata var
    "candle_holder_01.obj",
    //"bowl_01.obj", hata var 
    "bottle_02.obj",
    "apple.obj",
    "amphora_01.obj",
};

void loadObjModel(std::string model)
{
    objr.read(cmake_PROJECT_RES + model);

    modelPoints = objr.vertices;
    meshFaces = objr.faces;

    renderTrigs.clear();
    renderTrigs.resize(meshFaces.size());

}

Vector2 project(Vector3 vec)
{
    if (f_proj)
    {
        return camera.projectPerspective(vec);
    }
    return camera.projectOrtho(vec);
}

void loadAxis()
{
    axisPoints.emplace_back(5, 0, 0);
    axisPoints.emplace_back(-5, 0, 0);

    axisPoints.emplace_back(0, 5, 0);
    axisPoints.emplace_back(0, -5, 0);

    axisPoints.emplace_back(0, 0, 5);
    axisPoints.emplace_back(0, 0, -5);
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

    //loadCube(modelPoints, meshFaces);
    //loadAxis();
    
    

    objr.read(cmake_PROJECT_RES "kale.obj");

    
    modelPoints = objr.vertices;
    meshFaces = objr.faces;
    
    renderTrigs.resize(meshFaces.size());

    Vector3 sum(0, 0, 0);

    for (auto& p : modelPoints)
    {
        sum = sum + p;
    }

    center = sum / (float)modelPoints.size();

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
    for (size_t i = 0; i < meshFaces.size(); i++)
    {
        Face face = meshFaces[i];

        Vector3 faceVertices[3];

        faceVertices[0] = modelPoints[face.a];
        faceVertices[1] = modelPoints[face.b];
        faceVertices[2] = modelPoints[face.c];


        Triangle projectedTrig;

        for (size_t j = 0; j < 3; j++)
        {
            Vector3 point = faceVertices[j];

            //kucultme/buyultme
            point.x = point.x * scale.x;
            point.y = point.y * scale.y;
            point.z = point.z * scale.z;


            if (f_ralfa)
            {
                //dondurme
                point = point.rotateX(rotate.x);
                point = point.rotateY(rotate.y);
                point = point.rotateZ(rotate.z);
            }
            else
            {
                point = point.rotateY(alfa);
                alfa += donusHizi;
            }
            
            //tasima
            point = point + position;

            point.z -= camera.position.z;

            //ekrana yansit
            Vector2 projectedPoint = project(point);

            // y ters cevir boylece yukarsi y + asagi y - olur
            projectedPoint.y = -projectedPoint.y;
            projectedPoint.x += rcontext.WindowWidth / 2;
            projectedPoint.y += rcontext.WindowHeight / 2;

            projectedTrig.points[j] = projectedPoint;
        }

        renderTrigs.emplace_back(projectedTrig);                        
    }
    
    

}

void drawImgui()
{
    ImGui_ImplSDLRenderer3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();
    //===================================================//
    //===================================================//
    //===================================================//
    ImGui::Begin("Model Yukleyici");

    if (ImGui::Combo("Modeller", &currentModel, models, IM_ARRAYSIZE(models)))
    {
        if (currentModel != lastModel)
        {
            loadObjModel(models[currentModel]);
            lastModel = currentModel;
        }        
    }

    ImGui::End();
    //------------------------------//
    ImGui::Begin("Kontrol Paneli");
    
    ImGui::Checkbox("Perspektif", &f_proj);
    ImGui::Checkbox("Ucgenler", &f_trigs);
    ImGui::Checkbox("Kose noktalari", &f_crect);
    ImGui::Checkbox("Y dondur", &f_ralfa);
    ImGui::Checkbox("Merkezi Donus", &f_rotateCenter);

    ImGui::SliderFloat("Donus hizi", &donusHizi, 0, 0.0001f, "%.6f");

    ImGui::NewLine();
    ImGui::Text("fare mx,my %f , %f",mouseX , mouseY);

    ImGui::SliderFloat("FOV_factor", &camera.FOV_factor, 0, 1000);
    
   

    if (ImGui::Button("R##0"))
    {
        position.x = 0;
        position.y = 0;
        position.z = 0;
    }
    ImGui::SliderFloat("pozisyon.x", &position.x, -2, 2);
    ImGui::SliderFloat("pozisyon.y", &position.y, -2, 2);
    ImGui::SliderFloat("pozisyon.z", &position.z, -2, 2);
    
    
   

    if (ImGui::Button("R##1"))
    {
        scale.x = 1;
        scale.y = 1;
        scale.z = 1;
    }
    ImGui::SliderFloat("boyut.x", &scale.x, -2, 100);
    ImGui::SliderFloat("boyut.y", &scale.y, -2, 100);
    ImGui::SliderFloat("boyut.z", &scale.z, -2, 100);

   

    if (ImGui::Button("R##2"))
    {
        rotate.x = 0;
        rotate.y = 0;
        rotate.z = 0;
    }
    ImGui::SliderFloat("dondurme.x", &rotate.x, -4, 4);
    ImGui::SliderFloat("dondurme.y", &rotate.y, -4, 4);
    ImGui::SliderFloat("dondurme.z", &rotate.z, -4, 4);

   

    if (ImGui::Button("R##3"))
    {
        Vector3 sum(0, 0, 0);

        for (auto& p : modelPoints)
        {
            sum = sum + p;
        }

        center = sum / (float)modelPoints.size();
    }
    ImGui::SliderFloat("merkez.x", &center.x, -5, 5);
    ImGui::SliderFloat("merkez.y", &center.y, -5, 5);
    ImGui::SliderFloat("merkez.z", &center.z, -5, 5);


    ImGui::End();

    //===================================================//
    //===================================================//
    //===================================================//

    ImGui::Render();
    ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), rcontext.renderer);
}

void draw()
{
    //------------------------------//    

    gp.clearColorBuffer(Color::BLACK);    

    //gp.drawDots(Color::GREEN);

    for (size_t i = 0; i < renderTrigs.size(); i++)
    {
        Triangle trig = renderTrigs[i];

        if (f_crect)
        {
            gp.drawFilledRectangle(trig.points[0].x, trig.points[0].y, 5, 5, Color::BLUE);
            gp.drawFilledRectangle(trig.points[1].x, trig.points[1].y, 5, 5, Color::BLUE);
            gp.drawFilledRectangle(trig.points[2].x, trig.points[2].y, 5, 5, Color::BLUE);
        }
        
        if (f_trigs)
        {
            gp.drawTriangle(
                trig.points[0].x, trig.points[0].y,
                trig.points[1].x, trig.points[1].y,
                trig.points[2].x, trig.points[2].y,
                Color::GREEN
            );
        }
    }

    /*
       [0,1] [1,2] [2,3] [3,0]
       [0,4] [1,5] [2,6] [3,7]
       [4,5] [5,6] [6,7] [7,4]
    */

    

    gp.drawColorBuffer();
    
    //------------------------------//

    renderTrigs.clear();
}

int main()
{        
    /*ObjReader objreader;
    objreader.read(cmake_PROJECT_RES "kup.obj");

    for (size_t i = 0; i < objreader.vertices.size(); i++)
    {
        std::cout << objreader.vertices[i].x << ","
            << objreader.vertices[i].y << ","
            << objreader.vertices[i].z
            << "\n";
    }
    std::cout << "----\n";
    for (size_t i = 0; i < objreader.faces.size(); i++)
    {
        std::cout << objreader.faces[i].a << ","
            << objreader.faces[i].b << ","
            << objreader.faces[i].c
            << "\n";
    }*/

    initSDL();
    initImgui();

    rcontext.colorBuffer = new Color_t[rcontext.WindowWidth * rcontext.WindowHeight];
     
    
    //TinyTimer::Timer timer;
    uti::Timer timer;
    while(f_running)
    {
        float dt = timer.getElapsedTime_seconds();
        float fps = 1.0f / dt;
        std::cout << fps << "\n";
        timer.start();

        inputs();

        update();

        SDL_RenderClear(rcontext.renderer);
        drawImgui();
        draw();
        //swap buffers
        SDL_RenderPresent(rcontext.renderer);
        
        
    }
}