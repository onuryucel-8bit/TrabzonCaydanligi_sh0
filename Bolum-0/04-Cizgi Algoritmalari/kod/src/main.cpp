#include <iostream>
#include <chrono>
#include <cstdint>
#include <vector>

#include "SDL3/SDL.h"

#include "imgui.h"

#include "backends/imgui_impl_sdl3.h"
#include "backends/imgui_impl_sdlrenderer3.h"

const int WindowWidth = 800;
const int WindowHeight = 600;
SDL_Window* window = nullptr;

SDL_Renderer* renderer = nullptr;
bool f_running = true;

ImGuiIO* io;

using Color_t = uint32_t;
Color_t* colorBuffer = nullptr;

SDL_Texture* canvas = nullptr;

int px0 = 100, py0 = 100;
int px1 = 200, py1 = 200;


//Hizlica ana renkleri yazmak icin 32bit renk enum
enum Color : uint32_t
{
    BLACK = 0x0000'0000,
    WHITE = 0xffff'ffff,
    RED = 0xffff'0000,
    GREEN = 0xff00'ff00,
    BLUE = 0xff00'00ff
};

/*
* @brief Renk tamponundaki tum pikseller belirtilen renk ile temizlenir
* @param color renk degeri(uint32_t)
*/
void clearColorBuffer(Color_t color)
{
    for (size_t i = 0; i < WindowWidth * WindowHeight; i++)
    {
        colorBuffer[i] = color;
    }
}

void drawColorBuffer()
{
    //Hazirladigimiz piksel dizisini ekrana cizmek icin kaplamaya(SDL_Texture* canvas) kopyaliyoruz

    //SDL_Texture*    : Guncellenecek hedef kaplama
    //const SDL_Rect* : Guncellenecek alan, NULL ise alanin hepsi guncellenir
    //const void*     : Piksel verisinin adresi
    //int             : Piksel verisinin bayt cinsinden satir uzunlugu
    SDL_UpdateTexture(canvas, NULL, colorBuffer, (int)(WindowWidth * sizeof(Color_t)));

    //Kaplamayi cizdir    
    SDL_RenderTexture(renderer, canvas, NULL, NULL);
}

void drawPixel(int x, int y, Color_t color)
{
    if (x < 0 || x >= WindowWidth || y < 0 || y >= WindowHeight)
    {
        return;
    }

    //2 boyutlu dizideki piksel koordinatlarini 1 boyuta donusturup icine renk degerini atiyoruz
    //Eger bu satir anlasilmadiysa, y * w + x donusumu ile iliskili processing ornegine bakiniz              
    colorBuffer[y * WindowWidth + x] = color;
}

void ddaLineAlgo(int x0, int y0, int x1, int y1, Color_t color)
{
    int deltaX = x1 - x0;
    int deltaY = y1 - y0;

    /*
        =========================
        if |deltaX| >= |deltaY|
            sideLength = |deltaX|

        else
            sideLength = |deltaY|
        =========================

        veya

        =========================
        sideLength = max( |deltax|, |deltay|)
        =========================
    */
    //uzun kenari aliyoruz
    int sideLength = abs(deltaX) >= abs(deltaY) ? abs(deltaX) : abs(deltaY);

    //x,y koordinatlarinda adim sayisini hesapliyoruz
    float incX = deltaX / (float)sideLength;
    float incY = deltaY / (float)sideLength;

    //Cizginin baslangic noktasi
    float currentX = x0;
    float currentY = y0;

    for (size_t i = 0; i <= sideLength; i++)
    {
        drawPixel(round(currentX), round(currentY), color);
        currentX += incX;
        currentY += incY;
    }
}

void drawLine(int x0, int y0, int x1, int y1, Color_t color)
{
    ddaLineAlgo(x0, y0, x1, y1, color);
}

//===========================================================================//
//===========================================================================//
//===========================================================================//

void initImgui()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    io = &ImGui::GetIO();

    // Enable Docking
    io->ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io->FontGlobalScale = 1;

    ImGui_ImplSDL3_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer3_Init(renderer);
}

void initSDL()
{
    window = SDL_CreateWindow("TrabzonCaydanligi", WindowWidth, WindowHeight, SDL_WINDOW_FULLSCREEN);

    if (window == nullptr)
    {
        std::cout << "HATA:: Pencere olusturulamadi\n";
        f_running = false;
    }

    renderer = SDL_CreateRenderer(window, NULL);

    if (renderer == nullptr)
    {
        std::cout << "HATA:: Renderer olusturulamadi\n";
        f_running = false;
    }    
   
    //RAM'de tuttuğumuz renk değerlerini(colorBuffer) ekrana çizmek için kaplama(texture) oluşturuyoruz
    canvas = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, WindowWidth, WindowHeight);

    if (canvas == nullptr)
    {
        std::cout << "Error:: Texture initializing failed\n";
        f_running = false;
    }

    //Kaplamanin piksel gorunume sahip olmasi icin
    SDL_SetTextureScaleMode(canvas, SDL_SCALEMODE_NEAREST);
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
        }
    }
}

void drawImgui()
{
    ImGui_ImplSDLRenderer3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();
    //===================================================//
    //===================================================//
    //===================================================//

    ImGui::Begin("yazi");

    static int current = 0;
    
    ImGui::SliderInt("x0", &px0, 0, WindowWidth);
    ImGui::SliderInt("y0", &py0, 0, WindowHeight);

    ImGui::SliderInt("x1", &px1, 0, WindowWidth);
    ImGui::SliderInt("y1", &py1, 0, WindowHeight);
    
    ImGui::End();


    //===================================================//
    //===================================================//
    //===================================================//

    ImGui::Render();
    ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), renderer);
}

void draw()
{
    //maviye boya
    clearColorBuffer(Color::BLACK);
       
    
    drawLine(px0, py0, px1, py1, Color::GREEN);

    //ekrana cizdir
    drawColorBuffer();
}

void update(float dt)
{

}

int main()
{
    initSDL();
    initImgui();
  
    colorBuffer = new Color_t[WindowWidth * WindowHeight];

    using Clock = std::chrono::steady_clock;
    using Duration = std::chrono::duration<float>;
    using TimePoint = std::chrono::steady_clock::time_point;

    TimePoint previousTime = Clock::now();

    while(f_running)
    {
        //------------Delta time--------------------//
        TimePoint currentTime = Clock::now();

        Duration deltaTime = currentTime - previousTime;

        previousTime = currentTime;

        float dt = deltaTime.count();
        float fps = 1.0f / dt;
        //------------------------------------------//

        inputs();

        update(dt);
        
        SDL_RenderClear(renderer);

        draw();
        drawImgui();

        //tampon bellekleri degistir
        SDL_RenderPresent(renderer);
    }
}