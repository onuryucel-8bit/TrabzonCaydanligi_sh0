#include "Application.h"

Application::Application()
    :gp(m_context)
{
    FPS = 0;
    dt = 0;

    olcek.x = 1;
    olcek.y = 1;

    pozisyon.x = 0;
    pozisyon.y = 0;

    //                                     x,       y,     z
    modelNoktalari.push_back(Vector3{   0.0f,  -40.0f,  0.0f }); 
    modelNoktalari.push_back(Vector3{  30.0f,   30.0f,  0.0f });
    modelNoktalari.push_back(Vector3{ -30.0f,   30.0f,  0.0f });

    //modelNoktalari.push_back(Vector3{ 90.0f,  -40.0f,  0.0f });
    //modelNoktalari.push_back(Vector3{ 120.0f,   30.0f,  0.0f });
    //modelNoktalari.push_back(Vector3{ 60.0f,   30.0f,  0.0f });
    
    
}

Application::~Application()
{
}

void Application::run()
{
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

//#define DEBUG_FULLSCREEN

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

    Matrix2x2 olcekMatriks = Matrix2x2::makeScaleMatrix(olcek);
    Matrix2x2 dondurmeMatriks = Matrix2x2::makeRotateMatrix(alfa);
    Matrix3x3 tasimaMatriks = Matrix3x3::makeTranslationMatrix(pozisyon);


    for (size_t i = 0; i < modelNoktalari.size(); i++)
    {       
        Vector2 p = modelNoktalari[i].toVec2();

        //kucultme/buyultme
        p = olcekMatriks * p;
                       
        //dondur
        p = dondurmeMatriks * p;

        //tasima
        p = tasimaMatriks * p;
       
        Vector2 nokta;
        
        nokta.x = p.x + cx;
        nokta.y = p.y + cy;
        
        izdusumNoktalari.push_back(nokta);
    }
    
    //alfa += degToRad(0.05f);   

    alfa = alfa - 2 * std::numbers::pi * (int)(alfa / (2 * std::numbers::pi));

    if (alfa < 0)
    {
        alfa += 2 * std::numbers::pi;
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
    
    for (size_t i = 0; i < izdusumNoktalari.size(); i+= 3)
    {
        //Triangle trig = izdusumUcgenleri[i];

        gp.drawTriangle(
            izdusumNoktalari[i].x, izdusumNoktalari[i].y,
            izdusumNoktalari[i + 1].x, izdusumNoktalari[i + 1].y,
            izdusumNoktalari[i + 2].x, izdusumNoktalari[i + 2].y,
            Color::WHITE);
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

    ImGui::End();
    //===================================================//
    //===================================================//
    //===================================================//

    ImGui::Begin("Kontrol Paneli");

    ImGui::RadioButton("DDA algoritmasi", (int*)&Graphics::m_lineAlgoType, (int)(LineAlgoType::DDA));
    ImGui::RadioButton("Brensham algoritmasi", (int*)&Graphics::m_lineAlgoType, (int)(LineAlgoType::Brensham));
    ImGui::Text("alfa %f", radToDeg(alfa));

    ImGui::Text("pencere fiziksel  (Genislik %d, Yukseklik %d)", m_context.fizikselGenislik, m_context.fizikselYukseklik);
    ImGui::Text("pencere mantiksal (Genislik %d, Yukseklik %d)", m_context.mantiksalGenislik, m_context.mantiksalYukseklik);

    ImGui::SliderFloat("alfa", &alfa, 0, degToRad(360));
    ImGui::SliderFloat("olcek.x", &olcek.x, 1, 10);
    ImGui::SliderFloat("olcek.y", &olcek.y, 1, 10);
    ImGui::SliderFloat("pozisyon.x", &pozisyon.x, -m_context.fizikselGenislik /2, m_context.fizikselGenislik /2);
    ImGui::SliderFloat("pozisyon.y", &pozisyon.y, -m_context.fizikselYukseklik /2, m_context.fizikselYukseklik /2);



    ImGui::End();
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