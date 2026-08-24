#include "Application.h"

Application::Application()
    :gp(rcontext)
{
    nokta.x = 500;
    nokta.y = 300;
    FPS = 0;
    dt = 0;
}

Application::~Application()
{
}

void Application::run()
{
    initSDL();
    initImgui();

    setup();

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

        SDL_RenderClear(rcontext.renderer);

        draw();
        drawImgui();

        //tampon bellekleri degistir
        SDL_RenderPresent(rcontext.renderer);
    }
}

void Application::initSDL()
{

#ifdef DEBUG_FULLSCREEN
    rcontext.window = SDL_CreateWindow("TrabzonCaydanligi", rcontext.WindowWidth, rcontext.WindowHeight, SDL_WINDOW_FULLSCREEN);
#else
    rcontext.window = SDL_CreateWindow("TrabzonCaydanligi", rcontext.WindowWidth, rcontext.WindowHeight, NULL);
#endif // DEBUG_FULLSCREEN


    if (rcontext.window == nullptr)
    {
        std::cout << "HATA:: Pencere olusturulamadi\n";
        f_running = false;
    }

    rcontext.renderer = SDL_CreateRenderer(rcontext.window, NULL);

    if (rcontext.renderer == nullptr)
    {
        std::cout << "HATA:: Renderer olusturulamadi\n";
        f_running = false;
    }

    //RAM'de tuttuðumuz renk deðerlerini(colorBuffer) ekrana çizmek için kaplama(texture) oluþturuyoruz
    rcontext.canvas = SDL_CreateTexture(rcontext.renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, rcontext.WindowWidth, rcontext.WindowHeight);

    if (rcontext.canvas == nullptr)
    {
        std::cout << "Error:: Texture initializing failed\n";
        f_running = false;
    }

    //Kaplamanin piksel gorunume sahip olmasi icin
    SDL_SetTextureScaleMode(rcontext.canvas, SDL_SCALEMODE_NEAREST);
}

void Application::initImgui()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    //io = &ImGui::GetIO();

    // Enable Docking
    //io->ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    //io->FontGlobalScale = 1;

    ImGui_ImplSDL3_InitForSDLRenderer(rcontext.window, rcontext.renderer);
    ImGui_ImplSDLRenderer3_Init(rcontext.renderer);
}

void Application::setup()
{
    rcontext.colorBuffer = new Color_t[rcontext.WindowWidth * rcontext.WindowHeight];
}

//=============================================================================//
//=============================================================================//
//=============================================================================//

void Application::update(float dt)
{
    if (f_merkezDonus)
    {
        float merkezx = (float)rcontext.WindowWidth / 2.0f;
        float merkezy = (float)rcontext.WindowHeight / 2.0f;

        //noktayi ekranin ortasina tasi
        float px = nokta.x - merkezx;
        float py = nokta.y - merkezy;

        //dondur 
        float rx = px * cos(alfa) - py * sin(alfa);
        float ry = px * sin(alfa) + py * cos(alfa);

        //noktayi ekran uzayina geri donustur
        nokta.x = rx + merkezx;
        nokta.y = ry + merkezy;
    }
    else
    {       
        //dondur
        float rx = nokta.x * cos(alfa) - nokta.y * sin(alfa);
        float ry = nokta.x * sin(alfa) + nokta.y * cos(alfa);

        nokta.x = rx;
        nokta.y = ry;
    }
        
    //usengeclikten f(x) => alfa++ => g(x)
    //normalde alfa += RADIAN_D90 gibi bisey yazilmasi daha iyi olur
    alfa = radToDeg(alfa);

    alfa += 0.00001f;

    alfa = degToRad(alfa);

    
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

        if (event.type == SDL_EVENT_QUIT)
        {
            f_running = false;
        }

        switch (event.key.key)
        {
        case SDLK_ESCAPE:
            f_running = false;
            break;
        }
    }
}

void Application::draw()
{
    gp.clearColorBuffer(Color::BLACK);
    //------------------------------//    

    gp.drawPixel(nokta.x, nokta.y, Color::GREEN);

    gp.drawLine(0, rcontext.WindowHeight / 2, rcontext.WindowWidth, rcontext.WindowHeight / 2, Color::GREEN);

    gp.drawLine(rcontext.WindowWidth / 2, 0, rcontext.WindowWidth / 2, rcontext.WindowHeight, Color::GREEN);

    gp.drawDots(Color::GREEN);

    //------------------------------//
    gp.drawColorBuffer();
}

void Application::drawImgui()
{
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

    if (ImGui::Button("merkez donus"))
    {
        f_merkezDonus = !f_merkezDonus;
        if (f_merkezDonus)
        {
            nokta.x = 500;
            nokta.y = 300;
        }
    }    

    ImGui::Text("nokta (x,y) (%f, %f)", nokta.x, nokta.y);
    ImGui::Text("alfa %f", radToDeg(alfa));

    ImGui::End();
    //===================================================//
    //===================================================//
    //===================================================//

    ImGui::Render();
    ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), rcontext.renderer);
}
