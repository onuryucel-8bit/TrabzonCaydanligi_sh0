#pragma once

#include <iostream>
#include <numbers>
#include <vector>
#include <string>
#include <functional>
#include <algorithm>

#include "SDL3/SDL.h"

#include "imgui.h"
#include "backends/imgui_impl_sdl3.h"
#include "backends/imgui_impl_sdlrenderer3.h"

#include "Graphics.h"
#include "Defs.h"

#include "math/Vector2.h"
#include "math/Vector3.h"
#include "math/Vector4.h"
#include "math/Matrix4x4.h"

#include "ObjReader.h"
#include "Triangle.h"
#include "Gui.h"
#include "RenderSettings.h"
#include "Light.h"

#include "utils/Timer.h"



class Application
{
public:
    Application();
    ~Application();

    void run();

private:

   

    //====================================//
    Vector2 project(Vector3 vec);
    void loadObjModel(std::string model);
    void initSDL();
    void inputs(float dt);
    void update(float dt);
    void draw();
    //====================================//

    RenderContext rcontext;
    Graphics gp;
    AppState appState;
    ObjReader objr;
    Light light;

    Matrix4x4 projectionMatrix;

    bool f_running = true;

    float alfa = 0;    

    float dotNormalCamera;

    //====================================//
    float mouseX = 0, mouseY = 0;

    int screenWidth, screenHeight;
    //====================================//

    std::vector<Vector3> modelPoints;
    std::vector<Face> meshFaces;

    std::vector<Triangle> renderTrigs;

    std::vector<Vector3> transformedVertices;       
};
