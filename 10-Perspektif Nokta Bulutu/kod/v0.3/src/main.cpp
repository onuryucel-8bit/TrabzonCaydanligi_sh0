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
bool f_lines = true;
bool f_rotateCenter = false;

//====================================//
float mouseX = 0, mouseY = 0;

int screenWidth, screenHeight;
//====================================//

std::vector<Vector3> modelPoints;
std::vector<Vector3> axisPoints;

std::vector<Vector2> projectedPoints;
std::vector<Vector2> axisProjectPoints;
 
float FOV_factor = 300;

Vector3 position(0, 0, 0);
Vector3 scale(1.0f, 1.0f, 1.0f);
Vector3 rotate(0, 0, 0);
Vector3 center(-3.0f, -3.0f, -3.0f);

std::vector<std::pair<int,int>> edges;

void loadCube()
{    
    /*
    *           ^ Y+
    *           |
    *           |
              [0,1,-1]7		[1,1,-1]6
		        *-------------*
	           /|			 /|
	          /	|			/ |
	         /  |		   /  |
	[1,0,0]4*-------------* [1,1,0]5
	        |	|		  |   |
	        |	|-------------| [1,0,-1]2 --->X+
	        |   *[0,0,-1]3|   /
	        |  /		  |  /
	        | /			  | /
	        |/			  |/
	        *-------------*[1,0,0]1
        [0,0,0]0
          /
         /
        /
       Z-


    */
    edges.emplace_back(0, 1);
    edges.emplace_back(1, 2);
    edges.emplace_back(2, 3);
    edges.emplace_back(3, 0);

    edges.emplace_back(0, 4);
    edges.emplace_back(1, 5);
    edges.emplace_back(2, 6);
    edges.emplace_back(3, 7);

    edges.emplace_back(4, 5);
    edges.emplace_back(5, 6);
    edges.emplace_back(6, 7);
    edges.emplace_back(7, 4);

    /*
        [0,1] [1,2] [2,3] [3,0]
        [0,4] [1,5] [2,6] [3,7]
        [4,5] [5,6] [6,7] [7,4]
    */
    modelPoints.emplace_back(0, 0, 0);
    modelPoints.emplace_back(1, 0, 0);
    modelPoints.emplace_back(1, 0, -1);
    modelPoints.emplace_back(0, 0, -1);

    modelPoints.emplace_back(0, 1, 0);
    modelPoints.emplace_back(1, 1, 0);
    modelPoints.emplace_back(1, 1, -1);
    modelPoints.emplace_back(0, 1, -1);

    
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

void loadCubeCloud(Vector3 position, float inc)
{
    for (float z = position.z; z <= 1.0f; z += inc)
    {
        for (float y = position.y; y <= 1.0f; y += inc)
        {
            for (float x = position.x; x <= 1.0f; x+= inc)
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
    loadAxis();
    axisProjectPoints.resize(axisPoints.size());
    projectedPoints.resize(modelPoints.size());

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
    for (size_t i = 0; i < modelPoints.size(); i++)
    {
        Vector3 point = modelPoints[i];
        
        //vektor carpimi ekle a * b operator*(...);
        point.x = point.x * scale.x;
        point.y = point.y * scale.y;
        point.z = point.z * scale.z;
       
        if (f_rotateCenter)
        {            
            point = point - center;
        }

        point = point.rotateX(rotate.x);
        point = point.rotateY(rotate.y);
        point = point.rotateZ(rotate.z);
        
        if (f_rotateCenter)
        {
            point = point + center;
        }
        

        point = point + position;
      
        point.z -= camera.position.z;

        /*
        * point = scale(point);
        * point = rotate(point);
        * point = translate(point);
        */

        //ekrana zimbala
        projectedPoints[i] = project(point);



        projectedPoints[i].y = -projectedPoints[i].y; //y ters cevir boylece yukarsi y+ asagi y- olur
        projectedPoints[i].x += rcontext.WindowWidth / 2;
        projectedPoints[i].y += rcontext.WindowHeight / 2;
    }
    
    for (size_t i = 0; i < axisPoints.size(); i++)
    {
        Vector3 point = axisPoints[i];

        point.z -= camera.position.z;

        Vector2 projectedp = project(point);

        projectedp.x += rcontext.WindowWidth / 2;
        projectedp.y += rcontext.WindowHeight / 2;

        gp.drawFilledRectangle(
            projectedp.x,
            projectedp.y,
            2,
            2,
            Color::WHITE
        );

        axisProjectPoints[i] = projectedp;
    }

}

void drawImgui()
{
    ImGui_ImplSDLRenderer3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();
    
    ImGui::Begin("Kontrol Paneli");
    
    ImGui::Checkbox("Perspektif", &f_proj);
    ImGui::Checkbox("Cizgiler", &f_lines);
    ImGui::Checkbox("Merkezi Donus", &f_rotateCenter);

    ImGui::NewLine();
    ImGui::Text("fare mx,my %f , %f",mouseX , mouseY);

    ImGui::SliderFloat("FOV_factor", &FOV_factor, 0, 600);
    
   

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
    ImGui::SliderFloat("boyut.x", &scale.x, -2, 2);
    ImGui::SliderFloat("boyut.y", &scale.y, -2, 2);
    ImGui::SliderFloat("boyut.z", &scale.z, -2, 2);

   

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
            5,
            5,
            Color::BLUE
        );

       
    }

    /*
       [0,1] [1,2] [2,3] [3,0]
       [0,4] [1,5] [2,6] [3,7]
       [4,5] [5,6] [6,7] [7,4]
    */

    if (f_lines)
    {
        for (size_t i = 0; i < edges.size(); i++)
        {
            int a = edges[i].first;
            int b = edges[i].second;

            Vector2 p0 = projectedPoints[a];
            Vector2 p1 = projectedPoints[b];

            gp.drawLine(p0.x, p0.y, p1.x, p1.y, Color::GREEN);
        }


    }


    

    for (size_t i = 0; i < axisProjectPoints.size(); i+= 2)
    {
        

        gp.drawFilledRectangle(
            axisProjectPoints[i].x,
            axisProjectPoints[i].y,
            2,
            2,
            Color::WHITE
        );

        gp.drawFilledRectangle(
            axisProjectPoints[i + 1].x,
            axisProjectPoints[i + 1].y,
            2,
            2,
            Color::WHITE
        );

        gp.drawLine(
            axisProjectPoints[i].x,
            axisProjectPoints[i].y,
            axisProjectPoints[i + 1].x,
            axisProjectPoints[i + 1].y,
            Color::RED
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