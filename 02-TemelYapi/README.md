<h2>Piksel Cizimi</h2>

```cpp
using Color_t = uint32_t;
Color_t* colorBuffer = nullptr;
```
- pikselleri koyup ekrana yollamak icn bir boyurtlu dizi olusturalim




\
***main.cpp***
```cpp

#include <iostream>
#include <cstdint>

#include "SDL3/SDL.h"

const int WindowWidth = 800;
const int WindowHeight = 600;
SDL_Window* window = nullptr;
...
...

using Color_t = uint32_t;

Color_t* colorBuffer = nullptr;

...

int main()
{
    initSDL();

    colorBuffer = new Color_t[WindowWidth * WindowHeight];

    ...
}

```


***main.cpp***

```cpp

void clearColorBuffer(Color_t color)
{
    for (size_t i = 0; i < WindowWidth * WindowHeight; i++)
    {
        colorBuffer[i] = color;
    }
}

```

***main.cpp***
```cpp

SDL_Texture* canvas = nullptr;

void initSDL()
{
    ...
    ...

    canvas = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, WindowWidth, WindowHeight);

    if(canvas == nullptr)
    {
        std::cout << "Error:: Texture initializing failed\n";
        //return false;
    }
}
```


```cpp
void drawColorBuffer()
{
    //load colorbuffer
    SDL_UpdateTexture(canvas, NULL, colorBuffer, (int)(WindowWidth * sizeof(Color_t)));
    
    //render canvas
    SDL_RenderTexture(renderer, canvas, NULL, NULL);
}

void drawPixel(int x, int y, Color_t color)
{
    colorBuffer[y * WindowWidth + x] = color;
}
```


```cpp
int main()
{
    initSDL();

    colorBuffer = new Color_t[WindowWidth * WindowHeight];

    SDL_Event event;
    while(f_running)
    {
        while (SDL_PollEvent(&event))
        {
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

        //------------------------------//
        SDL_RenderClear(renderer);

        clearColorBuffer(0xff00'00ff);

        drawColorBuffer();

        //swap buffers
        SDL_RenderPresent(renderer);
        //------------------------------//
    }
}
```

***main.cpp***
```cpp
enum Color : uint32_t
{
    BLACK = 0x0000'0000,
    WHITE = 0xffff'ffff,
    RED   = 0xffff'0000,
    GREEN = 0xff00'ff00,
    BLUE  = 0xff00'00ff
};

int main()
{
    while(f_running)
    {
        ...
        clearColorBuffer(Color::GREEN);
    }
}
```


```cpp
#include <iostream>
#include <cstdint>

#include "SDL3/SDL.h"

const int WindowWidth = 800;
const int WindowHeight = 600;

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;
bool f_running = true;

using Color_t = uint32_t;

Color_t* colorBuffer = nullptr;

SDL_Texture* canvas = nullptr;

enum Color : uint32_t
{
    BLACK = 0x0000'0000,
    WHITE = 0xffff'ffff,
    RED   = 0xffff'0000,
    GREEN = 0xff00'ff00,
    BLUE  = 0xff00'00ff
};

void clearColorBuffer(Color_t color)
{
    for (size_t i = 0; i < WindowWidth * WindowHeight; i++)
    {
        colorBuffer[i] = color;
    }
}

void drawColorBuffer()
{
    //load colorbuffer
    SDL_UpdateTexture(canvas, NULL, colorBuffer, (int)(WindowWidth * sizeof(Color_t)));
    
    //render canvas
    SDL_RenderTexture(renderer, canvas, NULL, NULL);

}

void initSDL()
{
    window = SDL_CreateWindow("TrabzonCaydanligi", WindowWidth, WindowHeight, NULL);

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

    canvas = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, WindowWidth, WindowHeight);

    if(canvas == nullptr)
    {
        std::cout << "Error:: Texture initializing failed\n";
        //return false;
    }

    //make it pixaled
    //SDL_SetTextureScaleMode(canvas, SDL_SCALEMODE_NEAREST);
}

int main()
{
    initSDL();

    colorBuffer = new Color_t[WindowWidth * WindowHeight];

    SDL_Event event;
    while(f_running)
    {
        while (SDL_PollEvent(&event))
        {
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

        SDL_RenderClear(renderer);

        clearColorBuffer(Color::GREEN);
        
        drawColorBuffer();

        //swap buffers
        SDL_RenderPresent(renderer);
    }
}
```