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
#include "Triangle.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include "utils/tiny_obj_loader.h"

#include "utils/Timer.h"
#include "utils/Timer_t0.h"


RenderContext rcontext;
Graphics gp(rcontext);

ImGuiIO* io;

Camera camera;

//ENUM kullan...

enum class TransformMode
{
    None,
    Translate,  //G
    Rotate,     //R
    Scale       //S
};

enum RenderMode
{
    RENDER_WIREFRAME,
    RENDER_VERTEX,
    RENDER_WIREFRAME_VERTEX,
    RENDER_FILLED
};

enum class CullMode
{
    NONE,
    ACTIVE
};

enum class DepthTest
{
    NONE,
    PAINTER
};

TransformMode transformMode = TransformMode::None;
RenderMode currentRenderMode = RenderMode::RENDER_WIREFRAME;
DepthTest deptTest = DepthTest::PAINTER;

CullMode cullmode = CullMode::ACTIVE;

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

std::vector<unsigned char> meshFaces;
std::vector<tinyobj::material_t> materials;
std::vector<tinyobj::shape_t> shapes;

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
    "canned_food_01.obj", 
    "candle_holder_01.obj",
    "bowl_01.obj", 
    "bottle_02.obj",
    "apple.obj",
    "amphora_01.obj",
    "tavsan.obj",
    "caydanlik.obj"
};

void loadObjModel(std::string model)
{
    std::string inputfile = cmake_PROJECT_RES + model;
        
    std::string err;
    std::string warn;
    
    bool ret = tinyobj::LoadObj(&VertexAttributes, &shapes, &materials, &warn, &err, inputfile.c_str(), cmake_PROJECT_RES);

    if (!err.empty())
    {
        std::cout << err << "\n";
    }    
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
       
    loadObjModel("kup.obj");

    for (size_t i = 0; i < 12; i++)
    {
        int matID = shapes[0].mesh.material_ids[i];
        
        tinyobj::material_t& mat = materials[matID];

        float r = mat.diffuse[0];
        float g = mat.diffuse[1];
        float b = mat.diffuse[2];

        std::cout << r << " " << g << " " << g << "\n";
        //std::cout << (int)r << " " << (int)g << " " << (int)g << "\n";
    }
    
    
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

        if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN)
        {
            transformMode = TransformMode::None;
        }

        switch (event.key.key)
        {
        case SDLK_ESCAPE:
            f_running = false;
            break;

        case SDLK_W:           
            //camera.position.y -= moveSpeed;
            break;
        //case SDLK_S:
        //    //camera.position.y += moveSpeed;
        //    break;

        case SDLK_A:
            //camera.position.x -= moveSpeed;
            break;
        case SDLK_D:
            //camera.position.x += moveSpeed;
            break;

        case SDLK_R:
            transformMode = TransformMode::Rotate;
            break;

        case SDLK_G:
            transformMode = TransformMode::Translate;
            break;

        case SDLK_S:
            transformMode = TransformMode::Scale;
            break;
        }
    }

    SDL_GetMouseState(&mouseX, &mouseY);
}

void update(float dt)
{            
    renderTrigs.clear();

    std::vector<Vector3> transformedVertices;
    
    for (size_t i = 0; i < VertexAttributes.vertices.size(); i += 3)
    {
        Vector3 p =
        {
            VertexAttributes.vertices[i + 0],
            VertexAttributes.vertices[i + 1],
            VertexAttributes.vertices[i + 2]
        };

        //boyut
        p.x *= scale.x;
        p.y *= scale.y;
        p.z *= scale.z;

        float dx, dy;
        SDL_GetRelativeMouseState(&dx, &dy);

        //dondur
        if (f_ralfa)
        {            
            if (transformMode == TransformMode::Rotate)
            {
                rotate.x += dy * 0.01f;
            }


            p = p.rotateX(rotate.x);
            p = p.rotateY(rotate.y);
            p = p.rotateZ(rotate.z);
        }
        else
        {
            p = p.rotateY(alfa);
        }

        if (transformMode == TransformMode::Translate)
        {
            position.x += dx * 0.01f;                        
        }

        //tasi
        p = p - position;

        p.z -= 5.0;

        transformedVertices.push_back(p);
    }
   
    std::vector<tinyobj::index_t>& indexBufferObject = shapes[0].mesh.indices;
    
    for (size_t i = 0; i < indexBufferObject.size(); i += 3)
    {
        Vector3 vectorA = transformedVertices[indexBufferObject[i + 0].vertex_index];
        Vector3 vectorB = transformedVertices[indexBufferObject[i + 1].vertex_index];
        Vector3 vectorC = transformedVertices[indexBufferObject[i + 2].vertex_index];

        if (cullmode == CullMode::ACTIVE)
        {
            Vector3 vectorAB = vectorB - vectorA;
            Vector3 vectorAC = vectorC - vectorA;

            Vector3 normal = vectorAB.cross(vectorAC);

            Vector3 cameraRay = camera.position - vectorA;

            float dotNormalCamera = normal.dot(cameraRay);

            if (dotNormalCamera <= 0)
            {
                continue;
            }
        }

        Triangle trig;

        Vector3 verts[3] = { vectorA, vectorB, vectorC };
        
        for (int j = 0; j < 3; j++)
        {
            //noktalari ekrana yansit
            Vector2 p = project(verts[j]);

            
            //ekrana ortala
            p.x += rcontext.WindowWidth / 2;
            p.y += rcontext.WindowHeight / 2;

            trig.points[j] = p;

            //Color_t color = 0xff00'0000;            
        }

       

        size_t triIndex = i / 3;

        int matID = shapes[0].mesh.material_ids[triIndex];
        
        if (materials.empty() || matID < 0)
        {
            trig.color = 0xff00'ff00;
        }
        else
        {
            tinyobj::material_t& mat = materials[matID];

            uint8_t r = mat.diffuse[0] * 255;
            uint8_t g = mat.diffuse[1] * 255;
            uint8_t b = mat.diffuse[2] * 255;

            //std::cout << mat.diffuse[0] << " " << mat.diffuse[1] << " " << mat.diffuse[2] << "\n";
            //std::cout << (int)r << " " << (int)g << " " << (int)b << "\n";

            trig.color =
                (0xFF << 24) |
                (r << 16) |
                (g << 8) |
                (b);
        }
                                
        trig.averageDepth = (vectorA.z + vectorB.z + vectorC.z) / 3;
        renderTrigs.emplace_back(trig);
    }    

    if (deptTest == DepthTest::PAINTER)
    {
        std::sort(renderTrigs.begin(), renderTrigs.end(),
            [](const Triangle& a, const Triangle& b)
            {
                return a.averageDepth > b.averageDepth;
            }
        );
    }

    alfa += donusHizi * dt;
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

    ImGui::RadioButton("Wireframe", (int*)&currentRenderMode, (int)RenderMode::RENDER_WIREFRAME);
    ImGui::RadioButton("vertex", (int*)&currentRenderMode, (int)RenderMode::RENDER_VERTEX);
    ImGui::RadioButton("filled", (int*)&currentRenderMode, (int)RenderMode::RENDER_FILLED);
    ImGui::RadioButton("vertexWireframe", (int*)&currentRenderMode, (int)RenderMode::RENDER_WIREFRAME_VERTEX);
    ImGui::RadioButton("cull", (int*)&cullmode, (int)CullMode::NONE);
    ImGui::RadioButton("cull active", (int*)&cullmode, (int)CullMode::ACTIVE);
    ImGui::RadioButton("deptTest.none", (int*)&deptTest, (int)DepthTest::NONE);
    ImGui::RadioButton("deptTest.Painter", (int*)&deptTest, (int)DepthTest::PAINTER);

    //ImGui::Checkbox("Kose noktalari", &f_crect);
    ImGui::Checkbox("Y dondur", &f_ralfa);  

    ImGui::SliderFloat("Donus hizi", &donusHizi, 0, 0.09f, "%.6f");

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

    for (size_t i = 0; i < renderTrigs.size(); i++)
    {
        switch (currentRenderMode)
        {
        case RenderMode::RENDER_FILLED:                                                  
            gp.drawFilledTriangle(
                renderTrigs[i].points[0].x,
                renderTrigs[i].points[0].y,
                renderTrigs[i].points[1].x,
                renderTrigs[i].points[1].y,
                renderTrigs[i].points[2].x,
                renderTrigs[i].points[2].y,
                renderTrigs[i].color
            );                           
            break;

        case RenderMode::RENDER_WIREFRAME:
            gp.drawTriangle(
                renderTrigs[i].points[0].x,
                renderTrigs[i].points[0].y,
                renderTrigs[i].points[1].x,
                renderTrigs[i].points[1].y,
                renderTrigs[i].points[2].x,
                renderTrigs[i].points[2].y,
                renderTrigs[i].color
            );
            break;

        case RenderMode::RENDER_VERTEX:
            gp.drawRectangle(renderTrigs[i].points[0].x, renderTrigs[i].points[0].y, 1, 1, Color::BLUE);
            gp.drawRectangle(renderTrigs[i].points[1].x, renderTrigs[i].points[1].y, 1, 1, Color::BLUE);
            gp.drawRectangle(renderTrigs[i].points[2].x, renderTrigs[i].points[2].y, 1, 1, Color::BLUE);
            break;

        case RenderMode::RENDER_WIREFRAME_VERTEX:
            gp.drawTriangle(
                renderTrigs[i].points[0].x,
                renderTrigs[i].points[0].y,
                renderTrigs[i].points[1].x,
                renderTrigs[i].points[1].y,
                renderTrigs[i].points[2].x,
                renderTrigs[i].points[2].y,
                renderTrigs[i].color
            );

            gp.drawRectangle(renderTrigs[i].points[0].x, renderTrigs[i].points[0].y, 1, 1, Color::BLUE);
            gp.drawRectangle(renderTrigs[i].points[1].x, renderTrigs[i].points[1].y, 1, 1, Color::BLUE);
            gp.drawRectangle(renderTrigs[i].points[2].x, renderTrigs[i].points[2].y, 1, 1, Color::BLUE);
            break;
        }

        

        
    }

    

    //gp.drawFilledTriangle(100, 100, 150, 130, 80, 150, Color::GREEN);

    

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