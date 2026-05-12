#include "Application.h"

Application::Application()
    :
    gp(rcontext),
    light({0,0,-2})
{
}

Application::~Application()
{
}

void Application::run()
{
    initSDL();

    Gui gui
    (
         rcontext.window, 
         rcontext.renderer, 
         &appState, 
         [&](std::string model)
         {
             loadObjModel(model);
         }
    );

    rcontext.colorBuffer = new Color_t[rcontext.WindowWidth * rcontext.WindowHeight];

    while (f_running)
    {
        inputs();

        update();

        SDL_RenderClear(rcontext.renderer);
        gui.draw();
        draw();
        //swap buffers
        SDL_RenderPresent(rcontext.renderer);

    }
}

void Application::loadObjModel(std::string model)
{
    objr.read(cmake_PROJECT_RES + model);

    modelPoints = objr.vertices;
    meshFaces = objr.faces;

    renderTrigs.clear();
    renderTrigs.resize(meshFaces.size());

}

Vector2 Application::project(Vector3 vec)
{
    if (appState.m_projectMod == ProjectMod::Perspective)
    {
        return appState.camera.projectPerspective(vec);
    }
    return appState.camera.projectOrtho(vec);
}

void Application::initSDL()
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

    if (rcontext.canvas == nullptr)
    {
        std::cout << "Error:: Texture initializing failed\n";
        //return false;
    }

    //make it pixaled
    SDL_SetTextureScaleMode(rcontext.canvas, SDL_SCALEMODE_NEAREST);



    renderTrigs.resize(100);
}

void Application::inputs()
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

void Application::update()
{
    renderTrigs.clear();
    transformedVertices.clear();

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
            point.x = point.x * appState.m_scale.x;
            point.y = point.y * appState.m_scale.y;
            point.z = point.z * appState.m_scale.z;

            //dondurme
            point = point.rotateX(appState.m_rotate.x);
            point = point.rotateY(appState.m_rotate.y);
            point = point.rotateZ(appState.m_rotate.z);

            //tasima
            point = point + appState.m_position;

            point.z -= appState.camera.position.z;

            transformedPoints[j] = point;

            transformedVertices.emplace_back(point);
        }

        Vector3 vectorA = transformedPoints[0];
        Vector3 vectorB = transformedPoints[1];
        Vector3 vectorC = transformedPoints[2];

        Vector3 vectorAB = vectorB - vectorA;
        Vector3 vectorAC = vectorC - vectorA;

        Vector3 normal = vectorAB.cross(vectorAC);
        normal.normalize();

        Vector3 cameraRay = appState.camera.position - vectorA;

        dotNormalCamera = normal.dot(cameraRay);

        if (appState.m_cullMod == CullMod::ACTIVE)
        {
            if (dotNormalCamera <= appState.m_cullValue)
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

        //--isik---

        light.m_direction = appState.m_lightPos;

        light.m_direction.normalize();

        float intensityFactor = normal.dot(light.m_direction);

        //---------
        
        if (appState.m_lightMod == LightMod::FLAT)
        {
            projectedTrig.color = light.applyLighting(0xff00'ff00, intensityFactor);
        }
        else if (appState.m_lightMod == LightMod::NONE)
        {
            projectedTrig.color = 0xff00'ff00;
        }
        


        renderTrigs.emplace_back(projectedTrig);
    }



}

void Application::draw()
{
    //------------------------------//    

    gp.clearColorBuffer(Color::BLACK);

    for (size_t i = 0; i < renderTrigs.size(); i++)
    {
        Triangle trig = renderTrigs[i];

        if ((appState.m_renderMod & RenderMod::RenderMode_Triangle_Filled) == RenderMod::RenderMode_Triangle_Filled)
        {
            gp.drawFilledTriangle(
                trig.points[0].x, trig.points[0].y,
                trig.points[1].x, trig.points[1].y,
                trig.points[2].x, trig.points[2].y,
                trig.color
            );
        }

        if ((appState.m_renderMod & RenderMod::RenderMode_Vertex) == RenderMod::RenderMode_Vertex)
        {
            gp.drawFilledRectangle(trig.points[0].x, trig.points[0].y, 5, 5, Color::BLUE);
            gp.drawFilledRectangle(trig.points[1].x, trig.points[1].y, 5, 5, Color::BLUE);
            gp.drawFilledRectangle(trig.points[2].x, trig.points[2].y, 5, 5, Color::BLUE);
        }

        if ((appState.m_renderMod & RenderMod::RenderMode_Triangle) == RenderMod::RenderMode_Triangle)
        {
            gp.drawTriangle(
                trig.points[0].x, trig.points[0].y,
                trig.points[1].x, trig.points[1].y,
                trig.points[2].x, trig.points[2].y,
                trig.color
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