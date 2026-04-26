#include "Graphics.h"
#include "Graphics.h"
#include "Graphics.h"

Graphics::Graphics(RenderContext& context)
{
    m_context = &context;
}

Graphics::~Graphics()
{
}

void Graphics::clearColorBuffer(Color_t color)
{
    for (size_t i = 0; i < m_context->WindowWidth * m_context->WindowHeight; i++)
    {
        m_context->colorBuffer[i] = color;
    }
}

void Graphics::drawPixel(int x, int y, Color_t color)
{
    if (x >= 0 && x < m_context->WindowWidth && y >= 0 && y < m_context->WindowHeight)
    {
        m_context->colorBuffer[y * m_context->WindowWidth + x] = color;
    }
}


void Graphics::ddaLineAlgo(int x0, int y0, int x1, int y1, Color_t color)
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

void Graphics::drawLine(int x0, int y0, int x1, int y1, Color_t color)
{
    ddaLineAlgo(x0, y0, x1, y1, color);
}

void Graphics::drawTriangle(int x0, int y0, int x1, int y1, int x2, int y2, Color_t color)
{
    drawLine(x0, y0, x1, y1, color);
    drawLine(x1, y1, x2, y2, color);
    drawLine(x2, y2, x0, y0, color);
}

void Graphics::drawFilledTriangle(int x0, int y0, int x1, int y1, int x2, int y2, Color_t color)
{    
    //y0 < y1 < y2
    if (y0 > y1)
    {        
        swap(y0, y1);
        swap(x0, x1);
    }
    if (y1 > y2)
    {
        swap(y1, y2);
        swap(x1, x2);
    }
    if (y0 > y1)
    {
        swap(y0, y1);
        swap(x0, x1);
    }

    int my = y1;
    int mx = ((float)((x2 - x0) * (y1 - y0)) / (float)(y2 - y0)) + x0;

    fillFlatBottomTriangle(x0, y0, x1, y1, mx, my, color);

    fillFlatTopTriangle(x1, y1, mx, my, x2, y2, color);
}

/*
           x0,y0
             .
            / \
           /   \
    x1,y1.      .x2,y2


*/
void Graphics::fillFlatBottomTriangle(int x0, int y0, int x1, int y1, int x2, int y2, Color_t color)
{
    float invSlopeLeft = (float)(x1 - x0) / (y1 - y0);
    float invSlopeRight = (float)(x2 - x0) / (y2 - y0);

    float startx = x0;
    float endx = x0;

    for (int y = y0; y <= y2; y++)
    {
        drawLine(startx, y, endx, y, color);

        startx += invSlopeLeft;
        endx += invSlopeRight;
    }
}

void Graphics::fillFlatTopTriangle(int x0, int y0, int x1, int y1, int x2, int y2, Color_t color)
{
    float invSlopeLeft =  (float)(x2 - x0) / (y2 - y0);
    float invSlopeRight = (float)(x2 - x1) / (y2 - y1);

    float startx = x2;
    float endx = x2;

    for (int y = y2; y >= y0; y--)
    {
        drawLine(startx, y, endx, y, color);

        startx -= invSlopeLeft;
        endx -= invSlopeRight;
    }
}

void Graphics::drawTriangleFan(std::vector<Vector2>& pointList, Color_t color)
{
    if (pointList.size() < 6)
    {
        return;
    }

    int mx = pointList[0].x;
    int my = pointList[0].y;

    for(int i = 1; i < pointList.size() - 1; i++)
    {
        drawTriangle(
            mx, my, 
            pointList[i].x, pointList[i].y, 
            pointList[i + 1].x, pointList[i + 1].y, 
            color);    
    }
}

void Graphics::drawRectangle(int x, int y, int width, int height, Color_t color)
{
    drawLine(x, y, x + width, y, color);
    drawLine(x + width, y, x + width, y + height, color);
    drawLine(x + width, y + height, x, y + height, color);
    drawLine(x, y + height, x, y, color);
}

void Graphics::drawFilledRectangle(int x, int y, int width, int height, Color_t color)
{
    for (size_t posy = y; posy < height + y; posy++)
    {
        for (size_t posx = x; posx < width + x; posx++)
        {
            drawPixel(posx, posy, color);
        }
    }
}

void Graphics::drawGrid(Color_t color)
{
    for (size_t y = 0; y < m_context->WindowHeight; y++)
    {
        for (size_t x = 0; x < m_context->WindowWidth; x++)
        {
            if (x % 64 == 0 || y % 64 == 0)
            {
                m_context->colorBuffer[y * m_context->WindowWidth + x] = color;
            }
        }
    }
}

void Graphics::drawDots(Color_t color)
{
    for (size_t y = 0; y < m_context->WindowHeight; y+= 40)
    {
        for (size_t x = 0; x < m_context->WindowWidth; x+= 40)
        {
            m_context->colorBuffer[y * m_context->WindowWidth + x] = color;
        }
    }
}


void Graphics::swap(int& a, int& b)
{
    int temp = a;
    a = b;
    b = temp;
}

void Graphics::drawColorBuffer()
{
    //load colorbuffer
    SDL_UpdateTexture(m_context->canvas, NULL, m_context->colorBuffer, (int)(m_context->WindowWidth * sizeof(Color_t)));

    //render canvas
    SDL_RenderTexture(m_context->renderer, m_context->canvas, NULL, NULL);

}