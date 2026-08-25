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

}

Application::~Application()
{
}

void Application::run()
{
    loadCube(0.25f);

    initSDL();
    initImgui();   

    using Clock = std::chrono::steady_clock;
    using Duration = std::chrono::duration<float>;
    using TimePoint = std::chrono::steady_clock::time_point;

    TimePoint previousTime = Clock::now();

    while (f_running)
    {
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

        //tampon bellekleri degistir
        SDL_RenderPresent(m_context.renderer);
    }
}

void Application::initSDL()
{

#define DEBUG_FULLSCREEN

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

    //io = &ImGui::GetIO();

    // Enable Docking
    //io->ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    //io->FontGlobalScale = 1;

    ImGui_ImplSDL3_InitForSDLRenderer(m_context.window, m_context.renderer);
    ImGui_ImplSDLRenderer3_Init(m_context.renderer);
}

//=============================================================================//
//=============================================================================//
//=============================================================================//

void Application::update(float dt)
{        
    izdusumNoktalari.clear();

    //ekranin ortasi
    float cx = m_context.fizikselGenislik / 2.0f;
    float cy = m_context.fizikselYukseklik / 2.0f;

    Matrix4x4 S  =  Matrix4x4::makeScaleMatrix(olcek);
    Matrix4x4 Rx =  Matrix4x4::makeRotateXMatrix(alfa.x);
    Matrix4x4 Ry =  Matrix4x4::makeRotateYMatrix(alfa.y);
    Matrix4x4 Rz =  Matrix4x4::makeRotateZMatrix(alfa.z);
    Matrix4x4 T  =  Matrix4x4::makeTranslationMatrix(pozisyon);
    
    Matrix4x4 worldMatrix = T * Rz * Ry * Rx * S;


    for (size_t i = 0; i < modelNoktalari.size(); i++)
    {       
        Vector4 p = modelNoktalari[i].toVec4();
        
        p = worldMatrix * p;

        Vector3 nokta = p.toVec3();
        
        //noktalari kameradan uzaklastir
        nokta.z -= m_camera.m_pos.z;

        //izdusum
        Vector2 izdusum = m_camera.project(nokta);

        //noktalari ekranin ortasina getir
        izdusum.x += cx;
        izdusum.y += cy;

        izdusumNoktalari.push_back(izdusum);        
    }             
}

void Application::inputs()
{
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
    gp.clearColorBuffer(Color::BLACK);
    //------------------------------//    
    
    for (size_t i = 0; i < izdusumNoktalari.size(); i++)
    {
        //Triangle trig = izdusumUcgenleri[i];

        gp.drawFilledRectangle(
            izdusumNoktalari[i].x, 
            izdusumNoktalari[i].y,
            3,
            3,
            cubeColor);
    }

    

    gp.drawLine(0, m_context.fizikselYukseklik / 2, m_context.fizikselGenislik, m_context.fizikselYukseklik / 2, Color::GREEN);

    gp.drawLine(m_context.fizikselGenislik / 2, 0, m_context.fizikselGenislik / 2, m_context.fizikselYukseklik, Color::GREEN);

    gp.drawDots(Color::GREEN);

    //------------------------------//
    gp.drawColorBuffer();
}

void Application::drawImgui()
{
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

    ImGui::End();
    //===================================================//
    //===================================================//
    //===================================================//

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
    ImGui::SliderFloat("olcek.x", &olcek.x, 1, 10);
    ImGui::SliderFloat("olcek.y", &olcek.y, 1, 10);
    ImGui::SliderFloat("olcek.z", &olcek.z, 1, 10);

    if (ImGui::Button("R##2"))
    {
        alfa.x = 0;
        alfa.y = 0;
        alfa.z = 0;
    }
    ImGui::SliderFloat("alfa.x", &alfa.x, 0, degToRad(360));
    ImGui::SliderFloat("alfa.y", &alfa.y, 0, degToRad(360));
    ImGui::SliderFloat("alfa.z", &alfa.z, 0, degToRad(360));
    
    ImGui::End();
    //===================================================//
    //===================================================//
    //===================================================//

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

void Application::loadCube(float inc)
{
    modelNoktalari.clear();

    float incz = inc;
    float incy = inc;
    float incx = inc;

    for (float z = -1; z <= 1.0f; z += incz)
    {
        for (float y = -1; y <= 1.0f; y += incy)
        {
            for (float x = -1; x <= 1.0f; x += incx)
            {
                Vector3 vec(x, y, z);

                modelNoktalari.push_back(vec);
            }
        }
    }
}