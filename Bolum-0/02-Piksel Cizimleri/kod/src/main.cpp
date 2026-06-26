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

void xorKaplama();
void sinx();
void sqrtx();
void logxy();
void logxcosy();
void logxory();

using voidFuncPtr = void (*)();
voidFuncPtr exFunc = xorKaplama;
voidFuncPtr selFunc = nullptr;

struct Example
{
    const char* name;
    voidFuncPtr funcAdr;
};

std::vector<Example> ornekler = 
{
    {"xorKaplama", xorKaplama},
    {"sinxy", sinx},
    {"sqrtx", sqrtx},
    {"logxy", logxy},
    {"logxcosy", logxcosy},
    {"logxory", logxory}
};

//Hizlica ana renkleri yazmak icin 32bit renk enum
enum Color : uint32_t
{
    BLACK = 0x0000'0000,
    WHITE = 0xffff'ffff,
    RED = 0xffff'0000,
    GREEN = 0xff00'ff00,
    BLUE = 0xff00'00ff
};

int valx;
int valy;
int valsx = 1;
int valsy = 1;
float valef = 1.0f;

void drawPixel(int x, int y, Color_t color);

void xorKaplama()
{
    for (size_t x = 0; x < WindowWidth; x++)
    {
        for (size_t y = 0; y < WindowHeight; y++)
        {
            uint8_t xorColor = ((x + valx) * valsx )^ ((y + valy) * valsy);

            //0xAARR_GGBB

            Color_t color = (0xff << 24) | (xorColor << 16) | (xorColor << 8) | xorColor;

            drawPixel(x, y, color);
        }
    }
}

void sinx()
{
    for (size_t x = 0; x < WindowWidth; x++)
    {
        for (size_t y = 0; y < WindowHeight; y++)
        {
            float val = (x + valx) / valef;

            uint8_t c = std::sin(val) * valy;
            
            //0xAARR_GGBB

            //0xffcc_cccc
            //cc = xorcolor
            Color_t color = (0x61 << 24) | (0 << 16) | (0 << 8) | c;

            drawPixel(x, y, color);
        }
    }
}

void sqrtx()
{
    for (int y = 0; y < WindowHeight; y++)
    {
        for (int x = 0; x < WindowWidth; x++)
        {
            float val = (x + valx) / valef;

            Color_t color = cos(sqrt((val + valx) * valy) + valsx) * Color::GREEN;
            drawPixel(x, y, color);
        }
    }
}

void logxy()
{
    for (int y = 0; y < WindowHeight; y++)
    {
        for (int x = 0; x < WindowWidth; x++)
        {
            Color_t color = log(x ^ y) * Color::GREEN;
            drawPixel(x, y, color);
        }
    }
}

void logxcosy()
{
    for (int y = 0; y < WindowHeight; y++)
    {
        for (int x = 0; x < WindowWidth; x++)
        {
            Color_t color = log(x) * cos(y) * Color::GREEN;
            drawPixel(x, y, color);
        }
    }
}

void logxory()
{
    for (int y = 0; y < WindowHeight; y++)
    {
        for (int x = 0; x < WindowWidth; x++)
        {
            Color_t color = log((x + valx) * valsx | (y + valy) * valsy) * Color::GREEN;
            drawPixel(x, y, color);
        }
    }
}
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
    //2 boyutlu dizideki piksel koordinatlarini 1 boyuta donusturup icine renk degerini atiyoruz
    //Eger bu satir anlasilmadiysa, y * w + x donusumu ile iliskili processing ornegine bakiniz              
    colorBuffer[y * WindowWidth + x] = color;
}

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

    if (ImGui::BeginCombo("Ornekler", ornekler[current].name))
    {
        for (int i = 0; i < ornekler.size(); i++)
        {
            bool selected = current == i;

            if (ImGui::Selectable(ornekler[i].name, selected))
            {
                current = i;
                exFunc = ornekler[current].funcAdr;
            }

            if (selected)
            {
                ImGui::SetItemDefaultFocus();
            }
        }

        ImGui::EndCombo();
    }

    ImGui::SliderInt("valx", &valx, 0, 255);
    ImGui::SliderInt("valy", &valy, 0, 255);

    ImGui::SliderInt("valsx", &valsx, 0, 255);
    ImGui::SliderInt("valsy", &valsy, 0, 255);

    ImGui::SliderFloat("valef", &valef, 0, 500);

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
    clearColorBuffer(Color::BLUE);
       
    
    exFunc();

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