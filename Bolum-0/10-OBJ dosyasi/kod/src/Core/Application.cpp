#include "Application.h"

Application::Application()
    :gp(m_context)
{   
    olcek.x = 1;
    olcek.y = 1;
    olcek.z = 1;

    pozisyon.x = 0;
    pozisyon.y = 0;
    pozisyon.z = 0;

    ObjReader objreader;
    objreader.read("apple.obj");
    
    modelNoktalari = objreader.vertices;
    yuzeyListesi = objreader.faces;
    
    m_renderMod = RenderMod::RenderMod_Triangle_Filled;
    
}

Application::~Application()
{
}

void Application::run()
{
    //loadCube();

    initSDL();
    initImgui();   

    using Clock = std::chrono::steady_clock;
    using Duration = std::chrono::duration<float>;
    using TimePoint = std::chrono::steady_clock::time_point;

    TimePoint previousTime = Clock::now();

    while (f_running)
    {
        ZoneScoped;

        //------------Delta time--------------------//
        TimePoint currentTime = Clock::now();

        Duration deltaTime = currentTime - previousTime;

        previousTime = currentTime;

        dt = deltaTime.count();
        FPS = 1.0f / dt;
        //------------------------------------------//

        inputs();

        update(dt);

        SDL_RenderClear(m_context.renderer);

        draw();
        drawImgui();

        {
            //tampon bellekleri degistir
            ZoneScopedN("SDL_RenderPresent");
            SDL_RenderPresent(m_context.renderer);
        }
        FrameMark;
    }
}

void Application::initSDL()
{

#ifdef DEBUG_FULLSCREEN
    m_context.window = SDL_CreateWindow("TrabzonCaydanligi", m_context.fizikselGenislik, m_context.fizikselYukseklik, SDL_WINDOW_FULLSCREEN);
#else
    m_context.window = SDL_CreateWindow("TrabzonCaydanligi", m_context.fizikselGenislik, m_context.fizikselYukseklik, SDL_WINDOW_RESIZABLE);
#endif // DEBUG_FULLSCREEN


    if (m_context.window == nullptr)
    {
        std::cout << "HATA:: Pencere olusturulamadi\n";
        f_running = false;
    }

    m_context.renderer = SDL_CreateRenderer(m_context.window, NULL);

    if (m_context.renderer == nullptr)
    {
        std::cout << "HATA:: Renderer olusturulamadi\n";
        f_running = false;
    }

    createTexture();
    
}

void Application::initImgui()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
   
    // Enable Docking
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    ImGui_ImplSDL3_InitForSDLRenderer(m_context.window, m_context.renderer);
    ImGui_ImplSDLRenderer3_Init(m_context.renderer);
}

//=============================================================================//
//=============================================================================//
//=============================================================================//

void Application::update(float dt)
{            
    ZoneScopedN(__FUNCTION__);

    izdusumUcgenleri.clear();

    //ekranin ortasi
    float cx = m_context.fizikselGenislik / 2.0f;
    float cy = m_context.fizikselYukseklik / 2.0f;

    Matrix4x4 S  =  Matrix4x4::makeScaleMatrix(olcek);
    Matrix4x4 Rx =  Matrix4x4::makeRotateXMatrix(alfa.x);
    Matrix4x4 Ry =  Matrix4x4::makeRotateYMatrix(alfa.y);
    Matrix4x4 Rz =  Matrix4x4::makeRotateZMatrix(alfa.z);
    Matrix4x4 T  =  Matrix4x4::makeTranslationMatrix(pozisyon);
    
    Matrix4x4 worldMatrix = T * Rz * Ry * Rx * S;


    for (size_t i = 0; i < yuzeyListesi.size(); i++)
    {       
        Face yuz = yuzeyListesi[i];

        Vector3 yuzNoktalari[3];

        yuzNoktalari[0] = modelNoktalari[yuz.a];
        yuzNoktalari[1] = modelNoktalari[yuz.b];
        yuzNoktalari[2] = modelNoktalari[yuz.c];

        
        Vector3 transformedPoints[3];

        for (size_t j = 0; j < 3; j++)
        {
            Vector4 nokta = yuzNoktalari[j].toVec4();
            nokta = worldMatrix * nokta;

            Vector3 vertexWorldPos = nokta.toVec3();
            vertexWorldPos.z -= m_camera.m_position.z;

            transformedPoints[j] = vertexWorldPos;
        }

        Vector3 vectorA = transformedPoints[0];
        Vector3 vectorB = transformedPoints[1];
        Vector3 vectorC = transformedPoints[2];

        Vector3 vectorAB = vectorB - vectorA;
        Vector3 vectorAC = vectorC - vectorA;

        Vector3 normal = vectorAB.cross(vectorAC);
        normal.normalize();

        Vector3 cameraRay = m_camera.m_position - vectorA;

        float dotNormalCamera = normal.dot(cameraRay);

        if (m_cullmode == CullMod::ACTIVE)
        {
            if (dotNormalCamera <= m_cullValue)
            {
                continue;
            }
        }

        Triangle projectedTrig;

        for (size_t j = 0; j < 3; j++)
        {
            //ekrana yansit
            Vector2 projectedPoint = m_camera.project(transformedPoints[j]);

            //ekran koordinati  3 boyutta ise y+ yukari bundan dolayi -projectPoint.y yaziyoruz
            // --->x+
            // |
            // |
            // v
            // y+
            // 
            // y ters cevir boylece yukarsi y + asagi y - olur
            projectedPoint.y = -projectedPoint.y;
            projectedPoint.x += cx;
            projectedPoint.y += cy;

            projectedTrig.points[j] = projectedPoint;
        }
       
        izdusumUcgenleri.emplace_back(projectedTrig);
    }             
}

void Application::inputs()
{
    ZoneScopedN(__FUNCTION__);

    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        ImGui_ImplSDL3_ProcessEvent(&event);
       
        switch (event.key.key)
        {
        case SDLK_ESCAPE:
            f_running = false;
            break;        
        }

        switch (event.type)
        {
        case SDL_EVENT_WINDOW_RESIZED:
            applyWindowSize();
            break;

        case SDL_EVENT_QUIT:
            f_running = false;
            break;

        }
    }
}

void Application::draw()
{
    ZoneScopedN(__FUNCTION__);

    gp.clearColorBuffer(Color::BLACK);
    //------------------------------//    
    
    for (size_t i = 0; i < izdusumUcgenleri.size(); i++)
    {
        Triangle trig = izdusumUcgenleri[i];

        if ((m_renderMod & RenderMod::RenderMod_Vertex) == RenderMod::RenderMod_Vertex)
        {
            
            gp.drawFilledRectangle(trig.points[0].x, trig.points[0].y, 5, 5, Color::RED);
            gp.drawFilledRectangle(trig.points[1].x, trig.points[1].y, 5, 5, Color::RED);
            gp.drawFilledRectangle(trig.points[2].x, trig.points[2].y, 5, 5, Color::RED);
        }

        if ((m_renderMod & RenderMod::RenderMod_Triangle_Filled) == RenderMod::RenderMod_Triangle_Filled)
        {
           
            gp.drawFilledTriangle(
                trig.points[0].x, trig.points[0].y,
                trig.points[1].x, trig.points[1].y,
                trig.points[2].x, trig.points[2].y,
                m_color
            );
        }

        if ((m_renderMod & RenderMod::RenderMod_Triangle) == RenderMod::RenderMod_Triangle)
        {
            gp.drawTriangle(
                trig.points[0].x, trig.points[0].y,
                trig.points[1].x, trig.points[1].y,
                trig.points[2].x, trig.points[2].y,
                Color::RED
            );
        }
        
    }

    if (f_arkaPlan)
    {
        gp.drawLine(0, m_context.fizikselYukseklik / 2, m_context.fizikselGenislik, m_context.fizikselYukseklik / 2, Color::GREEN);

        gp.drawLine(m_context.fizikselGenislik / 2, 0, m_context.fizikselGenislik / 2, m_context.fizikselYukseklik, Color::GREEN);

        gp.drawDots(Color::GREEN);
    }    

    //------------------------------//
    gp.drawColorBuffer();
}

void Application::drawImgui()
{
    ZoneScopedN(__FUNCTION__);

    SDL_SetRenderLogicalPresentation(
        m_context.renderer,
        0,
        0,
        SDL_LOGICAL_PRESENTATION_DISABLED
    );


    ImGui_ImplSDLRenderer3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();
    //===================================================//
    //===================================================//
    //===================================================//    
    ImGui::Begin("Ucak Motoru");

    ImGui::Text("FPS %f", FPS);

    ImGui::RadioButton("DDA algoritmasi", (int*)&Graphics::m_lineAlgoType, (int)(LineAlgoType::DDA));
    ImGui::RadioButton("Brensham algoritmasi", (int*)&Graphics::m_lineAlgoType, (int)(LineAlgoType::Brensham));

    ImGui::RadioButton("Perspektif", (int*)&m_camera.m_projectMod, (int)(ProjectMod::Perspective));
    ImGui::RadioButton("Ortho", (int*)&m_camera.m_projectMod, (int)(ProjectMod::Ortho));

    ImGui::Text("pencere fiziksel  (Genislik %d, Yukseklik %d)", m_context.fizikselGenislik, m_context.fizikselYukseklik);
    ImGui::Text("pencere mantiksal (Genislik %d, Yukseklik %d)", m_context.mantiksalGenislik, m_context.mantiksalYukseklik);

    ImGui::Text("m_renderMod %d", m_renderMod);

    static bool showTriangles = false;

    if (ImGui::Checkbox("Ucgenler", &showTriangles))
    {
        if (showTriangles)
        {
            m_renderMod |= RenderMod::RenderMod_Triangle;
        }
        else
        {
            m_renderMod &= ~RenderMod::RenderMod_Triangle;
        }
    }

    static bool showFilledTrig = true;

    if (ImGui::Checkbox("Dolu ucgen", &showFilledTrig))
    {
        if (showFilledTrig)
        {
            m_renderMod |= RenderMod::RenderMod_Triangle_Filled;
        }
        else
        {
            m_renderMod &= ~RenderMod::RenderMod_Triangle_Filled;
        }
    }

    static bool showVertex = false;

    if (ImGui::Checkbox("Noktalar", &showVertex))
    {
        if (showVertex)
        {
            m_renderMod |= RenderMod::RenderMod_Vertex;
        }
        else
        {
            m_renderMod &= ~RenderMod::RenderMod_Vertex;
        }
    }

    ImGui::Checkbox("f_arkaPlan", &f_arkaPlan);

    ImGui::End();
    //===================================================//
    //===================================================//
    //===================================================//

#pragma region KontrolPaneli

    ImGui::Begin("Kontrol Paneli");

    if (ImGui::Combo("Modeller", &currentModel, models, IM_ARRAYSIZE(models)))
    {
        if (currentModel != lastModel)
        {
            loadObjModel(models[currentModel]);
            lastModel = currentModel;
        }
    }
    
    static float color[4] = { 0.0f, 1.0f, 0.0f, 1.0f };

    ImGui::ColorEdit4("Color", color);
    ImGui::ColorPicker4("Color Picker", color);

    uint8_t r = (uint8_t)(color[0] * 255);
    uint8_t g = (uint8_t)(color[1] * 255);
    uint8_t b = (uint8_t)(color[2] * 255);
    uint8_t a = (uint8_t)(color[3] * 255);

    m_color = (0xff << 24) | (r << 16) | (g << 8) | b;

    ImGui::Text("Kup renk 0x%08X", m_color);

    ImGui::NewLine();

    if (ImGui::Button("R##0"))
    {
        pozisyon.x = 0;
        pozisyon.y = 0;
        pozisyon.z = 0;
    }
    ImGui::SliderFloat("pozisyon.x", &pozisyon.x, -10, 10);
    ImGui::SliderFloat("pozisyon.y", &pozisyon.y, -10, 10);
    ImGui::SliderFloat("pozisyon.z", &pozisyon.z, -10, 10);

    if (ImGui::Button("R##1"))
    {
        olcek.x = 1;
        olcek.y = 1;
        olcek.z = 1;
    }
    ImGui::SliderFloat("olcek.x", &olcek.x, -10, 10);
    ImGui::SliderFloat("olcek.y", &olcek.y, -10, 10);
    ImGui::SliderFloat("olcek.z", &olcek.z, -10, 10);

    if (ImGui::Button("R##2"))
    {
        alfa.x = 0;
        alfa.y = 0;
        alfa.z = 0;
    }
    ImGui::SliderFloat("alfa.x", &alfa.x, 0, degToRad(360));
    ImGui::SliderFloat("alfa.y", &alfa.y, 0, degToRad(360));
    ImGui::SliderFloat("alfa.z", &alfa.z, 0, degToRad(360));
    

    ImGui::RadioButton("AC: Arka yuz eleme", (int*)&m_cullmode, (int)(CullMod::ACTIVE));
    ImGui::RadioButton("KAPAT: Arka yuz eleme", (int*)&m_cullmode, (int)(CullMod::NONE));
    
    ImGui::SliderFloat("m_cullValue", &m_cullValue, -10.0f, 10.0f);
    
    ImGui::End();

#pragma endregion

    //===================================================//
    //===================================================//
    //===================================================//
    ImGui::Render();
    ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), m_context.renderer);

    SDL_SetRenderLogicalPresentation(
        m_context.renderer,
        m_context.mantiksalGenislik,
        m_context.mantiksalYukseklik,
        SDL_LOGICAL_PRESENTATION_STRETCH
    );
}

//=============================================================================//
//=============================================================================//
//=============================================================================//

void Application::applyWindowSize()
{    
    SDL_GetWindowSize(m_context.window, &m_context.fizikselGenislik, &m_context.fizikselYukseklik);

    //p_context->mantiksalGenislik = p_context->fizikselGenislik;
    //p_context->mantiksalYukseklik = p_context->fizikselYukseklik;

    SDL_SetRenderLogicalPresentation(
        m_context.renderer,
        m_context.mantiksalGenislik,
        m_context.mantiksalYukseklik,
        SDL_LOGICAL_PRESENTATION_STRETCH
    );

    createTexture();
}

void Application::createTexture()
{
    SDL_DestroyTexture(m_context.canvas);
    delete[] m_context.colorBuffer;

    m_context.colorBuffer = new Color_t[m_context.fizikselGenislik * m_context.fizikselYukseklik];

    //RAM'de tuttuðumuz renk deðerlerini(colorBuffer) ekrana çizmek için kaplama(texture) oluþturuyoruz
    m_context.canvas = SDL_CreateTexture(m_context.renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, m_context.fizikselGenislik, m_context.fizikselYukseklik);

    if (m_context.canvas == nullptr)
    {
        std::cout << "Error:: Texture initializing failed\n";
        f_running = false;
    }

    //Kaplamanin piksel gorunume sahip olmasi icin
    SDL_SetTextureScaleMode(m_context.canvas, SDL_SCALEMODE_NEAREST);
}

void Application::loadCube()
{
    /*
    
                y+
                ^
                |
                |
                |
            (-1,1,1) ----------- (1,1,1)
               /|                  /|
              / |                 / |
             /  |                /  |
    (-1,1,-1)-----------(1,1,-1)    |
            |   |               |   |
            |   |               |   |
            |   |               |   |
            |   |               |   |
            |  (-1,-1,1)--------|---(1,-1,1)------------> x+
            |  /                |  /
            | /                 | /
            |/                  |/
    (-1,-1,-1) ----------- (1,-1,-1)
           /
          /
         /
        z-


            6 ----------- 4
           /|            /|
          / |           / |
         1 ----------- 2  |
         |  |          |  |
         |  7 ---------|--5
         | /           | /
         |/            |/
         0 ----------- 3

    */

    modelNoktalari.emplace_back(-1, -1, -1);  // 0
    modelNoktalari.emplace_back(-1, 1, -1);   // 1
    modelNoktalari.emplace_back(1, 1, -1);    // 2
    modelNoktalari.emplace_back(1, -1, -1);   // 3
    modelNoktalari.emplace_back(1, 1, 1);     // 4
    modelNoktalari.emplace_back(1, -1, 1);    // 5
    modelNoktalari.emplace_back(-1, 1, 1);    // 6
    modelNoktalari.emplace_back(-1, -1, 1);   // 7

    // front
    yuzeyListesi.emplace_back(0, 1, 2);
    yuzeyListesi.emplace_back(0, 2, 3);

    // right
    yuzeyListesi.emplace_back(3, 2, 4);
    yuzeyListesi.emplace_back(3, 4, 5);

    // back
    yuzeyListesi.emplace_back(5, 4, 6);
    yuzeyListesi.emplace_back(5, 6, 7);

    // left
    yuzeyListesi.emplace_back(7, 6, 1);
    yuzeyListesi.emplace_back(7, 1, 0);

    // top
    yuzeyListesi.emplace_back(1, 6, 4);
    yuzeyListesi.emplace_back(1, 4, 2);

    // bottom
    yuzeyListesi.emplace_back(5, 7, 0);
    yuzeyListesi.emplace_back(5, 0, 3);
}

void Application::loadObjModel(std::string model)
{
    m_objReader.read(model);

    modelNoktalari = m_objReader.vertices;
    yuzeyListesi = m_objReader.faces;

    izdusumUcgenleri.clear();
    izdusumUcgenleri.resize(yuzeyListesi.size());
}