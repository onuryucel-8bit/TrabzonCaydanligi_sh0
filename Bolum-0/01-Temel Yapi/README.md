<h2>Piksel Cizimi</h2>

```cpp
using Color_t = uint32_t;
Color_t* colorBuffer = nullptr;
``` 
 Ekranda görüntülenecek piksellerin renk verisini tutacak bir boyutlu dizi oluşturalım. Bu dizideki verileri(renk degerlerini) alıp SDL kaplamasına(SDL_Texture) aktararak ekrana çizeceğiz



\
***main.cpp***
```cpp

#include <iostream>

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

...

int main()
{
    initSDL();
    initImgui();
  
    colorBuffer = new Color_t[WindowWidth * WindowHeight];

    while(f_running)
    {
        ...
        ...
    }
}

```

![renk](resimler/anim_t3.png)


***main.cpp***
```cpp

SDL_Texture* canvas = nullptr;

void initSDL()
{
    ...
    ...

    //RAM'de tuttuğumuz renk değerlerini(colorBuffer) ekrana çizmek için kaplama(texture) oluşturuyoruz
    canvas = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, WindowWidth, WindowHeight);

    if(canvas == nullptr)
    {
        std::cout << "Error:: Texture initializing failed\n";
        f_running = false;
    }

    //Kaplamanin piksel gorunume sahip olmasi icin
    SDL_SetTextureScaleMode(canvas, SDL_SCALEMODE_NEAREST);
}
```

**main.cpp**
```cpp
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

void draw()
{
    //maviye boya
    clearColorBuffer(0xff00'00ff);
       
    //ekrana cizdir
    drawColorBuffer();
}

int main()
{
    initSDL();
    initImgui();
  
    colorBuffer = new Color_t[WindowWidth * WindowHeight];

    while(f_running)
    {
        inputs();

        update();
        
        SDL_RenderClear(renderer);

        draw();
        drawImgui();

        //tampon bellekleri degistir
        SDL_RenderPresent(renderer);
    }
}
``` 

***main.cpp***
```cpp

//Hizlica ana renkleri yazmak icin 32bit renk enum
enum Color : uint32_t
{
    BLACK = 0x0000'0000,
    WHITE = 0xffff'ffff,
    RED   = 0xffff'0000,
    GREEN = 0xff00'ff00,
    BLUE  = 0xff00'00ff
};

int draw()
{
    //maviye boya
    clearColorBuffer(Color::BLUE);
        
    //ekrana cizdir
    drawColorBuffer();    
}
```

<== [onceki bolum](../00-Proje/README.md) 

[sonraki bolum](../02-Piksel%20Cizimleri/README.md) ==>