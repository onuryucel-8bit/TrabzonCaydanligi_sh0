- DDA
- Bresenham
- Xiaolin Wu
- Gupta-Sproull 


```cpp
void drawPixel(int x, int y, Color_t color)
{
    colorBuffer[y * WindowWidth + x] = color;
}

void drawLine(int x0, int y0, int x1, int y1, Color_t color)
{
    ddaLineAlgo(x0, y0, x1, y1, color);
}
```

<h2> DDA algoritmasi </h2>

![cizgi](../resimler/2026-04-10%2020-28-49.png)

```cpp

void ddaLineAlgo(int x0, int y0, int x1, int y1, Color_t color)
{
    int deltaX = x1 - x0;
    int deltaY = y1 - y0;

    /*
        if |deltaX| >= |deltaY|
            sideLength = |deltaX|

        else
            sideLength = |deltaY|
    */
    int sideLength = abs(deltaX) >= abs(deltaY) ? abs(deltaX) : abs(deltaY);

    float incX = deltaX / (float)sideLength;
    float incY = deltaY / (float)sideLength;

    float currentX = x0;
    float currentY = y0;

    for (size_t i = 0; i <= sideLength; i++)
    {
        drawPixel(round(currentX), round(currentY), color);
        currentX += incX;
        currentY += incY;
    }
}
```



```cpp

void draw()
{
    //------------------------------//
    SDL_RenderClear(renderer);

    clearColorBuffer(Color::BLACK);

    drawLine(100, 100, 200, 200, Color::GREEN);

    drawColorBuffer();

    //swap buffers
    SDL_RenderPresent(renderer);
    //------------------------------//
}

```

