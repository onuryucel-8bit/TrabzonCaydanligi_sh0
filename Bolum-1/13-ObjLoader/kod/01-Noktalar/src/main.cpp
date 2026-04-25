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

#define TINYOBJLOADER_IMPLEMENTATION
#include "utils/tiny_obj_loader.h"

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
bool f_ralfa = true;


float alfa = 0;
float donusHizi = 0.00001f;

float FPS;
//====================================//
float mouseX = 0, mouseY = 0;

int screenWidth, screenHeight;
//====================================//



/*

Noktalarin tutuldugu yer
attrib_t::vertices => 3 floats per vertex

       v[0]        v[1]        v[2]        v[3]               v[n-1]
  +-----------+-----------+-----------+-----------+      +-----------+
  | x | y | z | x | y | z | x | y | z | x | y | z | .... | x | y | z |
  +-----------+-----------+-----------+-----------+      +-----------+

attrib_t::normals => 3 floats per vertex

       n[0]        n[1]        n[2]        n[3]               n[n-1]
  +-----------+-----------+-----------+-----------+      +-----------+
  | x | y | z | x | y | z | x | y | z | x | y | z | .... | x | y | z |
  +-----------+-----------+-----------+-----------+      +-----------+

attrib_t::texcoords => 2 floats per vertex

       t[0]        t[1]        t[2]        t[3]               t[n-1]
  +-----------+-----------+-----------+-----------+      +-----------+
  |  u  |  v  |  u  |  v  |  u  |  v  |  u  |  v  | .... |  u  |  v  |
  +-----------+-----------+-----------+-----------+      +-----------+

attrib_t::colors => 3 floats per vertex(vertex color. optional)

       c[0]        c[1]        c[2]        c[3]               c[n-1]
  +-----------+-----------+-----------+-----------+      +-----------+
  | x | y | z | x | y | z | x | y | z | x | y | z | .... | x | y | z |
  +-----------+-----------+-----------+-----------+      +-----------+

*/
tinyobj::attrib_t VertexAttributes;

/*
*   vertex_index
*   normal_index
*   texcoord_index
*/

std::vector<tinyobj::index_t> indexBufferObject;
std::vector<unsigned char> meshFaces;


std::vector<Triangle> renderTrigs;
std::vector<Vector2> renderPoints;

Vector3 position(0, 0, 0);
Vector3 scale(1.0f, 1.0f, 1.0f);
Vector3 rotate(0, 0, 0);
Vector3 center(-3.0f, -3.0f, -3.0f);


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
    "tavsan.obj",
    "caydanlik.obj"
};

void loadObjModel(std::string model)
{
    std::string inputfile = cmake_PROJECT_RES + model;

    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string err;

    bool ret = tinyobj::LoadObj(&VertexAttributes, &shapes, &materials, &err, inputfile.c_str());

    if (!err.empty())
    {
        std::cout << err << "\n";
    }

    indexBufferObject = shapes[0].mesh.indices;
}

Vector2 project(Vector3 vec)
{
    if (f_proj)
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
       
    std::string inputfile =  cmake_PROJECT_RES "caydanlik.obj";
    
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials; 
    std::string err;

    bool ret = tinyobj::LoadObj(&VertexAttributes, &shapes, &materials, &err, inputfile.c_str());

    if (!err.empty())
    {
        std::cout << err << "\n";
    }


    indexBufferObject = shapes[0].mesh.indices;
    //meshFaces = shapes[0].mesh.num_face_vertices;       
}

//------------------------------------------------//
//------------------------------------------------//
//------------------------------------------------//

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

void update(float dt)
{        
    renderPoints.clear();
    int j = 0;
    for (size_t i = 0; i < VertexAttributes.vertices.size(); i += 3)
    {
        Vector3 point =
        {
            VertexAttributes.vertices[i],
            VertexAttributes.vertices[i + 1],
            VertexAttributes.vertices[i + 2]
        };
   
        //kucultme/buyultme
        point.x = point.x * scale.x;
        point.y = point.y * scale.y;
        point.z = point.z * scale.z;

        //dondurme
        if (f_ralfa)
        {
            point = point.rotateX(rotate.x);
            point = point.rotateY(rotate.y);
            point = point.rotateZ(rotate.z);
        }
        else
        {
            point = point.rotateY(alfa);
            alfa += donusHizi * dt;
        }

        //tasima
        point = point + position;

        point.z -= camera.position.z;

        Triangle projectedTrig;
        
        //ekrana yansit
        Vector2 projectedPoint = project(point);
        
        // y ters cevir boylece yukarsi y + asagi y - olur
        projectedPoint.y = -projectedPoint.y;
        projectedPoint.x += rcontext.WindowWidth / 2;
        projectedPoint.y += rcontext.WindowHeight / 2;
        
        renderPoints.emplace_back(projectedPoint);
        j++;              
    }


    //for (size_t i = 0; i < meshFaces.size(); i++)
    //{
    //    Face face = meshFaces[i];

    //    Vector3 faceVertices[3];

    //    faceVertices[0] = indexBufferObject[face.a];
    //    faceVertices[1] = indexBufferObject[face.b];
    //    faceVertices[2] = indexBufferObject[face.c];


    //    Vector3 transformedVertices[3];

    //    for (size_t j = 0; j < 3; j++)
    //    {
    //        Vector3 point = faceVertices[j];

    //        //kucultme/buyultme
    //        point.x = point.x * scale.x;
    //        point.y = point.y * scale.y;
    //        point.z = point.z * scale.z;


    //        if (f_ralfa)
    //        {
    //            //dondurme
    //            point = point.rotateX(rotate.x);
    //            point = point.rotateY(rotate.y);
    //            point = point.rotateZ(rotate.z);
    //        }
    //        else
    //        {
    //            point = point.rotateY(alfa);
    //            alfa += donusHizi * dt;
    //        }

    //        //tasima
    //        point = point + position;

    //        point.z -= -2;

    //        transformedVertices[j] = point;
    //    }

    //    //-----------------------------------//
    //    Vector3 vecA = transformedVertices[0];
    //    Vector3 vecB = transformedVertices[1];
    //    Vector3 vecC = transformedVertices[2];

    //    Vector3 vecAB = vecB - vecA;
    //    Vector3 vecAC = vecC - vecA;

    //    Vector3 normal = vecAB.cross(vecAC);

    //    Vector3 cameraRay = vecA - camera.position;

    //    float cameraDotNormal = normal.dot(cameraRay);

    //    if (cameraDotNormal > 0)
    //    {
    //        continue;
    //    }

    //    //-----------------------------------//

    //    Triangle projectedTrig;
    //    for(int j = 0; j < 3; j++)
    //    {
    //        //ekrana yansit
    //        Vector2 projectedPoint = project(transformedVertices[j]);

    //        // y ters cevir boylece yukarsi y + asagi y - olur
    //        projectedPoint.y = -projectedPoint.y;
    //        projectedPoint.x += rcontext.WindowWidth / 2;
    //        projectedPoint.y += rcontext.WindowHeight / 2;

    //        projectedTrig.points[j] = projectedPoint;
    //    }

    //    renderTrigs.emplace_back(projectedTrig);                        
    //}
    
    

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
    
    ImGui::Text("FPS: %f", FPS);
    ImGui::Checkbox("Perspektif", &f_proj);
    //ImGui::Checkbox("Ucgenler", &f_trigs);
    //ImGui::Checkbox("Kose noktalari", &f_crect);
    ImGui::Checkbox("Y dondur", &f_ralfa);  

    ImGui::SliderFloat("Donus hizi", &donusHizi, 0, 0.009f, "%.6f");

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

    for (const auto& point : renderPoints)
    {
        gp.drawFilledRectangle(point.x, point.y, 1, 1, Color::GREEN);
    }
   

    

    gp.drawColorBuffer();
    
    //------------------------------//

    renderTrigs.clear();
}

int main()
{            
    initSDL();
    initImgui();

    rcontext.colorBuffer = new Color_t[rcontext.WindowWidth * rcontext.WindowHeight];

    uti::Timer timer;
    while(f_running)
    {
        float dt = timer.getElapsedTime_seconds();
        FPS = 1.0f / dt;
        
        timer.start();

        inputs();

        update(dt);

        SDL_RenderClear(rcontext.renderer);
        drawImgui();
        draw();
        //swap buffers
        SDL_RenderPresent(rcontext.renderer);
        
        
    }
}