#include "Application.h"

Application::Application()
    :
    gp(rcontext),
    light({0,0,-2})
{
    float aspx = (float)rcontext.WindowWidth / (float)rcontext.WindowHeight;
    float aspy = (float)rcontext.WindowHeight / (float)rcontext.WindowWidth;
    float fovy = std::numbers::pi / 3;
    float fovx = atan(tan(fovy / 2) * aspx) * 2;
    float znear = 0.1f;
    float zfar = 100.0f;

    appState.m_camera.init(fovx, fovy, znear, zfar, aspx, aspy);

    loadObjModel("kup.obj");
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
  
    
    uti::Timer timer;           
    while (f_running)
    {        
        float dt = timer.getElapsedTime_seconds();
        appState.m_FPS = 1.0f / dt;        
        timer.start();

        inputs(dt);

        update(dt);

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

void Application::initSDL()
{

#ifdef FULL_SCREEN
    rcontext.window = SDL_CreateWindow("TrabzonCaydanligi", rcontext.WindowWidth, rcontext.WindowHeight, SDL_WINDOW_FULLSCREEN);
#else
    rcontext.window = SDL_CreateWindow("TrabzonCaydanligi", rcontext.WindowWidth, rcontext.WindowHeight, NULL);
#endif // FULL_SCREEN


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

void Application::inputs(float dt)
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

        case SDLK_Q:
            appState.m_camera.pitchAngle += 0.1f;
            break;

        case SDLK_E:
            appState.m_camera.pitchAngle  -= 0.1f;
            break;

        case SDLK_UP:
            appState.m_camera.position.y += 10.0f * dt;
            break;

        case SDLK_DOWN:
            appState.m_camera.position.y -= 10.0f * dt;
            break;

        case SDLK_W:            
            appState.m_camera.velocity = appState.m_camera.direction * appState.m_camera.speed * dt;
            appState.m_camera.position = appState.m_camera.position + appState.m_camera.velocity;
            break;

        case SDLK_S:
            appState.m_camera.velocity = appState.m_camera.direction * appState.m_camera.speed * dt;
            appState.m_camera.position = appState.m_camera.position - appState.m_camera.velocity;
            break;

        case SDLK_P:
            appState.m_camera.speed += 0.01f;
            break;

        case SDLK_O:
            appState.m_camera.speed -= 0.01f;
            break;

        case SDLK_A:
            appState.m_camera.yawAngle -= 0.1f;
            break;

        case SDLK_D:
            appState.m_camera.yawAngle += 0.1f;
            break;

        }
    }

    SDL_GetMouseState(&mouseX, &mouseY);
}

void Application::update(float dt)
{
    renderTrigs.clear();
    transformedVertices.clear();

    Matrix4x4 scaleMatrix       = Matrix4x4::makeScaleMatrix(appState.m_scale);
    Matrix4x4 rotateXMatrix     = Matrix4x4::makeRotateXMatrix(appState.m_rotate.x);
    Matrix4x4 rotateYMatrix     = Matrix4x4::makeRotateYMatrix(appState.m_rotate.y);
    Matrix4x4 rotateZMatrix     = Matrix4x4::makeRotateZMatrix(appState.m_rotate.z);
    Matrix4x4 translationMatrix = Matrix4x4::makeTranslationMatrix(appState.m_position);

    Vector3 upDirection = { 0,1,0 };

     Vector3 target = { 0,0,1};
    Matrix4x4 rotateYCamera = Matrix4x4::makeRotateYMatrix(appState.m_camera.yawAngle);
    Matrix4x4 rotateXCamera = Matrix4x4::makeRotateXMatrix(appState.m_camera.pitchAngle);

    Matrix4x4 cameraRotation;

    cameraRotation = rotateYCamera * rotateXCamera;
    appState.m_camera.direction = (rotateCamera * target.toVec4()).toVec3();


    target = appState.m_camera.position + appState.m_camera.direction;


    appState.m_viewMatrix = Matrix4x4::lookAt(appState.m_camera.position, target, appState.m_camera.up);

    float fov = appState.m_camera.FOVy;
    float aspect = (float)rcontext.WindowWidth / (float)rcontext.WindowHeight;
    float znear = 0.1f;
    float zfar = 100.0f;

    projectionMatrix = Matrix4x4::makePerspectiveMatrix(fov, aspect, znear, zfar);

    for (size_t i = 0; i < meshFaces.size(); i++)
    {
       /* if (i != appState.testVar)
        {
            continue;
        }*/

        Face face = meshFaces[i];

        Vector3 faceVertices[3];

        faceVertices[0] = modelPoints[face.a];
        faceVertices[1] = modelPoints[face.b];
        faceVertices[2] = modelPoints[face.c];


        Vector4 transformedPoints[3];

        for (size_t j = 0; j < 3; j++)
        {
            Vector4 point = faceVertices[j].toVec4();

            //boyut
            point = scaleMatrix * point;
            //dondur
            point = rotateXMatrix * point;
            point = rotateYMatrix * point;
            point = rotateZMatrix * point;
            //tasi
            point = translationMatrix * point;
                        
            point = appState.m_viewMatrix * point;
            //point.z = point.z - appState.m_camera.eye.z;

            transformedPoints[j] = point;           
        }

        Vector3 vectorA = transformedPoints[0].toVec3();
        Vector3 vectorB = transformedPoints[1].toVec3();
        Vector3 vectorC = transformedPoints[2].toVec3();

        appState.veca = vectorA;
        appState.vecb = vectorB;
        appState.vecc = vectorC;

        Vector3 vectorAB = vectorB - vectorA;
        Vector3 vectorAC = vectorC - vectorA;

        Vector3 normal = vectorAB.cross(vectorAC);
        normal.normalize();

        Vector3 cameraRay = appState.m_camera.eye - vectorA;

        dotNormalCamera = normal.dot(cameraRay);

        if (appState.m_cullMod == CullMod::ACTIVE)
        {
            if (dotNormalCamera <= appState.m_cullValue)
            {
                continue;
            }
        }

        //------clip-------

        Polygon polygon = appState.m_camera.createPolygon(transformedPoints[0].toVec3(), transformedPoints[1].toVec3(), transformedPoints[2].toVec3());

        appState.m_camera.clip(polygon);

        std::vector<Triangle> clippedTriangle;
        clippedTriangle.reserve(30);

        //Polygon ===> Triangle
        
        for (int i = 0; i < polygon.vertices.size() - 2 && !polygon.vertices.empty(); i++)
        {
            int index0 = 0;
            int index1 = i + 1;
            int index2 = i + 2;

            Triangle trig;
            trig.points[0] = polygon.vertices[index0].toVec4();
            trig.points[1] = polygon.vertices[index1].toVec4();
            trig.points[2] = polygon.vertices[index2].toVec4();

            clippedTriangle.push_back(trig);
        }


        //---------------------                
        Triangle projectedTrig;
        for (size_t k = 0; k < clippedTriangle.size(); k++)
        {


            for (size_t j = 0; j < 3; j++)
            {
                //ekrana yansit
                Vector4 projectedPoint = projectionMatrix.multiplyProject(clippedTriangle[k].points[j]);

                //ekran koordinati  3 boyutta ise y+ yukari bundan dolayi -projectPoint.y yaziyoruz
                // --->x+
                // |
                // |
                // v
                // y+
                // 
                // y ters cevir boylece yukarsi y + asagi y - olur
                projectedPoint.y = -projectedPoint.y;

                projectedPoint.x *= rcontext.WindowWidth / 2.0f;
                projectedPoint.y *= rcontext.WindowHeight / 2.0f;


                projectedPoint.x += rcontext.WindowWidth / 2.0f;
                projectedPoint.y += rcontext.WindowHeight / 2.0f;


                projectedTrig.points[j] = projectedPoint;
            }


            //--isik---
            //-------------------------------//

            light.m_direction = appState.m_lightPos;

            light.m_direction.normalize();

            float intensityFactor = normal.dot(light.m_direction);



            if (appState.m_lightMod == LightMod::FLAT)
            {
                projectedTrig.color = light.applyLighting(0xff00'ff00, intensityFactor);
            }
            else if (appState.m_lightMod == LightMod::NONE)
            {
                projectedTrig.color = 0xff00'ff00;
            }
            //-------------------------------//


            //---Derinlik---/
            if (appState.m_depthTest == DepthTest::PAINTER_AVERAGE)
            {
                projectedTrig.depthTestValue = (vectorA.z + vectorB.z + vectorC.z) / 3;
            }
            else if (appState.m_depthTest == DepthTest::PAINTER_DISTANCE)
            {
                Vector3 trigCenter;
                trigCenter.x = (vectorA.x + vectorB.x + vectorC.x) / 3;
                trigCenter.y = (vectorA.y + vectorB.y + vectorC.y) / 3;
                trigCenter.z = (vectorA.z + vectorB.z + vectorC.z) / 3;

                Vector3 distance;

                distance = trigCenter - appState.m_camera.eye;

                projectedTrig.depthTestValue = distance.x * distance.x + distance.y * distance.y + distance.z * distance.z;
            }

            renderTrigs.push_back(projectedTrig);
        }
    }

    if (appState.m_depthTest == DepthTest::PAINTER_AVERAGE ||
        appState.m_depthTest == DepthTest::PAINTER_DISTANCE)
    {
        std::sort(renderTrigs.begin(), renderTrigs.end(),
            [](const Triangle& a, const Triangle& b)
            {
                return a.depthTestValue > b.depthTestValue;
            }
        );
    }
}

void Application::draw()
{
    //------------------------------//    

    gp.clearColorBuffer(Color::BLACK);

    appState.m_renderTrigSize = renderTrigs.size();

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

    gp.drawColorBuffer();

    //------------------------------//

    renderTrigs.clear();
}