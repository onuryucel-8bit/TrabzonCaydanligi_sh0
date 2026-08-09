#include "Application.h"

Application::Application()
    :gp(rcontext)
{
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
    rcontext.window = SDL_CreateWindow("TrabzonCaydanligi", rcontext.WindowWidth, rcontext.WindowHeight, SDL_WINDOW_FULLSCREEN);

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
    // float x2 = x * cos(theta) - y * sin(theta);
    // float y2 = x * sin(theta) + y * cos(theta);

    //ekranin ortasi
    float cx = rcontext.WindowWidth / 2.0f;
    float cy = rcontext.WindowHeight / 2.0f;

    for (size_t i = 0; i < 3; i++)
    {
        //noktayi ekranin ortasina tasi
        float px = trig[i].x - cx;
        float py = trig[i].y - cy;

        //dondur
        float rx = px * cos(alfa) - py * sin(alfa);
        float ry = px * sin(alfa) + py * cos(alfa);

        //noktayi ekran uzayina geri donustur
        trig[i].x = rx + cx;
        trig[i].y = ry + cy;
    }

    //usengeclikten f(x) => alfa++ => g(x)
    //normalde alfa += RADIAN_D90 gibi bisey yazilmasi daha iyi olur
    alfa = radToDeg(alfa);

    alfa += 0.00001f;

    alfa = degToRad(alfa);
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

    gp.drawTriangle(
        trig[0].x, trig[0].y,
        trig[1].x, trig[1].y,
        trig[2].x, trig[2].y,
        Color::WHITE);




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

    ImGui::End();
    //===================================================//
    //===================================================//
    //===================================================//

    ImGui::Render();
    ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), rcontext.renderer);
}
