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

RenderContext rcontext;
Graphics gp(rcontext);

ImGuiIO* io;

Camera camera;

//ENUM kullan...

bool f_running = true;

enum class ProjectMod
{
    Ortho,
    Perspective
};

enum RenderMod : uint8_t
{
    RenderMode_Vertex = 1 << 0,
    RenderMode_Triangle = 1 << 1
};

enum class CullMode
{
    NONE,
    ACTIVE
};

enum class ModelType
{
    Cube,
    Triangle
};

enum RotateMod : uint8_t
{
    Null,
    Rx,
    Ry,
    Rz
};

RotateMod rotateMod = RotateMod::Null;
ModelType modelType = ModelType::Cube;
ProjectMod projectMod = ProjectMod::Perspective;
uint8_t renderMod = RenderMod::RenderMode_Triangle;
CullMode cullmode = CullMode::ACTIVE;

float rotateVel = 0.00001f;
float alfa = 0;
float cullThreshold = 0.0f;

//====================================//
float mouseX = 0, mouseY = 0;

int screenWidth, screenHeight;
//====================================//

std::vector<Vector3> modelPoints;
std::vector<Face> meshFaces;

std::vector<Triangle> renderTrigs;

Vector3 position(0, 0, 0);
Vector3 scale(1.0f, 1.0f, 1.0f);
Vector3 rotate(0, 0, 0);

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
    if (projectMod == ProjectMod::Perspective)
    {
        return camera.projectPerspective(vec);
    }
    return camera.projectOrtho(vec);
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
    
    renderTrigs.resize(100);
    
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
    renderTrigs.clear();    
    
    for (size_t i = 0; i < meshFaces.size(); i++)
    {
        Face face = meshFaces[i];

        Vector3 faceVertices[3];

        faceVertices[0] = modelPoints[face.a];
        faceVertices[1] = modelPoints[face.b];
        faceVertices[2] = modelPoints[face.c];


        Vector3 transformedPoints[3];

        for (size_t j = 0; j < 3; j++)
        {
            Vector3 point = faceVertices[j];

            //kucultme/buyultme
            point.x = point.x * scale.x;
            point.y = point.y * scale.y;
            point.z = point.z * scale.z;

            //dondurme
            if (rotateMod == RotateMod::Null)
            {
                point = point.rotateZ(rotate.z);
                point = point.rotateX(rotate.x);
                point = point.rotateY(rotate.y);
            }
            else
            {
                switch (rotateMod)
                {
                case RotateMod::Rx:
                    point = point.rotateX(alfa);
                    break;
                case RotateMod::Ry:
                    point = point.rotateY(alfa);
                    break;
                case RotateMod::Rz:
                    point = point.rotateZ(alfa);
                    break;
                }
            }

            //tasima
            point = point + position;

            point.z -= camera.position.z;

            transformedPoints[j] = point;           
        }

        Vector3 vectorA = transformedPoints[0];
        Vector3 vectorB = transformedPoints[1];
        Vector3 vectorC = transformedPoints[2];

        Vector3 vectorAB = vectorB - vectorA;
        Vector3 vectorAC = vectorC - vectorA;

        Vector3 normal = vectorAB.cross(vectorAC);
        normal.normalize();

        Vector3 cameraRay = camera.position - vectorA;

        cameraRay.normalize();
        float dotNormalCamera = normal.dot(cameraRay);

        if (cullmode == CullMode::ACTIVE)
        {
            if (dotNormalCamera <= cullThreshold)
            {
                continue;
            }
        }

        Triangle projectedTrig;

        for (size_t j = 0; j < 3; j++)
        {
            //ekrana yansit
            Vector2 projectedPoint = project(transformedPoints[j]);

            //ekran koordinati  3 boyutta ise y+ yukari bundan dolayi -projectPoint.y yaziyoruz
            // --->x+
            // |
            // |
            // v
            // y+
            // 
            // y ters cevir boylece yukarsi y + asagi y - olur
            projectedPoint.y = -projectedPoint.y;
            projectedPoint.x += rcontext.WindowWidth / 2;
            projectedPoint.y += rcontext.WindowHeight / 2;

            projectedTrig.points[j] = projectedPoint;
        }

        renderTrigs.emplace_back(projectedTrig);
    }
    
    alfa += rotateVel;
}

void drawImgui()
{
    ImGui_ImplSDLRenderer3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();
    //===================================================//
    //===================================================//
    //===================================================//

    ImGui::Begin("Kamera");

    ImGui::RadioButton("Perspektif", (int*)&projectMod, (int)ProjectMod::Perspective);
    ImGui::RadioButton("Ortografik", (int*)&projectMod, (int)ProjectMod::Ortho);

    ImGui::Text("Ekran Boyutu (%i, %i)", screenWidth, screenHeight);

    ImGui::NewLine();
    ImGui::Text("Fare fx,fy (%f, %f)", mouseX, mouseY);

    ImGui::SliderFloat("FOV", &camera.FOV_factor, 0, 600);

    ImGui::End();
    //------------------------------------------------------------------//
    //------------------------------------------------------------------//
    //------------------------------------------------------------------//


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
    //------------------------------------------------------------------//
    //------------------------------------------------------------------//
    //------------------------------------------------------------------//


    ImGui::Begin("Kontrol Paneli");
    
    if (ImGui::RadioButton("Dondur.kapat", (int*)&rotateMod, (int)RotateMod::Null))
    {
        alfa = 0;
    }
    ImGui::RadioButton("Dondur.x", (int*)&rotateMod, (int)RotateMod::Rx);
    ImGui::RadioButton("Dondur.y", (int*)&rotateMod, (int)RotateMod::Ry);
    ImGui::RadioButton("Dondur.z", (int*)&rotateMod, (int)RotateMod::Rz);
    
    ImGui::SliderFloat("Dondurme Hizi", &rotateVel, 0, 0.005f, "%0.6f");

    

    static bool cullModecb = true;

    if (ImGui::Checkbox("Arka Yuz Eleme", &cullModecb))
    {
        if (cullModecb)
        {
            cullmode = CullMode::ACTIVE;
        }
        else
        {
            cullmode = CullMode::NONE;
        }
    }
    
    if (ImGui::Button("R##4"))
    {
        cullThreshold = 0;
    }
    ImGui::SliderFloat("Arka Yuz Eleme Orani", &cullThreshold, -10.0f, 10.0f, "%0.3f");

    static bool showTriangles = true;

    if (ImGui::Checkbox("Ucgenler", &showTriangles))
    {
        if (showTriangles)
        {
            renderMod |= RenderMod::RenderMode_Triangle;
        }
        else
        {
            renderMod &= ~RenderMod::RenderMode_Triangle;
        }
    }

    static bool showVertex = false;

    if (ImGui::Checkbox("Noktalar", &showVertex))
    {
        if (showVertex)
        {
            renderMod |= RenderMod::RenderMode_Vertex;
        }
        else
        {
            renderMod &= ~RenderMod::RenderMode_Vertex;
        }
    }

    ImGui::NewLine();
    ImGui::Text("fare mx,my %f , %f",mouseX , mouseY);

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

        if ((renderMod & RenderMod::RenderMode_Vertex) == RenderMod::RenderMode_Vertex)
        {
            gp.drawFilledRectangle(trig.points[0].x, trig.points[0].y, 5, 5, Color::BLUE);
            gp.drawFilledRectangle(trig.points[1].x, trig.points[1].y, 5, 5, Color::BLUE);
            gp.drawFilledRectangle(trig.points[2].x, trig.points[2].y, 5, 5, Color::BLUE);
        }

        if ((renderMod & RenderMod::RenderMode_Triangle) == RenderMod::RenderMode_Triangle)
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