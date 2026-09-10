#include "Graphics.h"

Graphics::Graphics(RenderContext& context)
    :m_context(context)
{
    f_textureActive = false;       
    m_TEST_tex_y = 0;
}

Graphics::~Graphics()
{
}

void Graphics::loadTexture(std::string id, std::string path)
{
    path = std::string(cmake_PROJECT_KAPLAMALAR) + path;

    std::cout << "kaplama yukleniyor yol: " << path << "\n";

    SDL_Surface* surface = IMG_Load(path.c_str());

    //std::cout << "format" << SDL_GetPixelFormatName(surface->format) << "\n";
    
    SDL_Surface* convertedSurface = SDL_ConvertSurface(surface, SDL_PIXELFORMAT_ARGB8888);

    SDL_DestroySurface(surface);
    //std::cout << "format" << SDL_GetPixelFormatName(surface->format) << "\n";

    TextureData texture;

    if (convertedSurface == nullptr)
    {
        std::cout << "HATA:: SDL_Surface yuklenemedi yol veya isim hatasi var" << path << "\n";
    }
    else
    {
        texture.width = convertedSurface->w;
        texture.height = convertedSurface->h;
        size_t totalPixels = texture.width * texture.height;
        
        texture.data = std::make_unique<Color_t[]>(totalPixels);
  
        std::memcpy(texture.data.get(), convertedSurface->pixels, totalPixels * sizeof(Color_t));
        
        SDL_DestroySurface(convertedSurface);

        m_textureMap[id] = std::move(texture);
    }
}

void Graphics::useTexture(std::string textureName)
{
    m_currentTextureId = textureName;
    f_textureActive = true;
}

void Graphics::unbindTexture()
{
    f_textureActive = false;
}

void Graphics::clearColorBuffer(Color_t color)
{
    ZoneScopedN(__FUNCTION__);

    //const size_t totalPixels = static_cast<size_t>(m_context.fizikselGenislik) * m_context.fizikselYukseklik;
    //std::fill(m_context.colorBuffer, m_context.colorBuffer + totalPixels, color);

    for (size_t i = 0; i < m_context.fizikselGenislik * m_context.fizikselYukseklik; i++)
    {
        m_context.colorBuffer[i] = color;
    }
}

void Graphics::drawPixel(int x, int y, Color_t color)
{
    ZoneScopedN(__FUNCTION__);

    if (x >= 0 && x < m_context.fizikselGenislik && y >= 0 && y < m_context.fizikselYukseklik)
    {
        m_context.colorBuffer[y * m_context.fizikselGenislik + x] = color;
    }
}

void Graphics::drawTexel(
    int x, int y, 
    Vector2 a, Vector2 b, Vector2 c,
    float u0, float v0,
    float u1, float v1,
    float u2, float v2,
    std::string textureid)
{
    Vector2 p(x,y);

    Vector3 weight = barycentricWeights(a, b, c, p);

    float alfa = weight.x;
    float beta = weight.y;
    float gamma = weight.z;

    float interpolated_u = u0 * alfa + u1 * beta + u2 * gamma;
    float interpolated_v = v0 * alfa + v1 * beta + v2 * gamma;

    int tex_x = abs((int)(interpolated_u * m_textureMap[textureid].width));
    int tex_y = abs((int)(interpolated_v * m_textureMap[textureid].height));

    int coords = tex_y * m_textureMap[textureid].width + tex_x;

    drawPixel(x, y, m_textureMap[textureid].data[coords]);
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

void Graphics::bresenhamLineAlgo(int x0, int y0, int x1, int y1, Color_t color)
{
    int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
    int dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
    int err = dx + dy, e2; /* error value e_xy */

    for (;;) {  /* loop */
        drawPixel(x0, y0, color);
        if (x0 == x1 && y0 == y1) break;
        e2 = 2 * err;
        if (e2 >= dy) { err += dy; x0 += sx; } /* e_xy+e_x > 0 */
        if (e2 <= dx) { err += dx; y0 += sy; } /* e_xy+e_y < 0 */
    }
}

void Graphics::drawLine(int x0, int y0, int x1, int y1, Color_t color)
{
    ZoneScopedN(__FUNCTION__);

    if (m_lineAlgoType == LineAlgoType::DDA)
    {
        ddaLineAlgo(x0, y0, x1, y1, color);
    }
    else
    {
        bresenhamLineAlgo(x0, y0, x1, y1, color);
    }
}

void Graphics::drawTriangle(int x0, int y0, int x1, int y1, int x2, int y2, Color_t color)
{
    drawLine(x0, y0, x1, y1, color);
    drawLine(x1, y1, x2, y2, color);
    drawLine(x2, y2, x0, y0, color);
}

void Graphics::drawTriangleFan(std::vector<int>& pointList, Color_t color)
{
    if (pointList.size() < 6)
    {
        return;
    }

    int mx = pointList[0];
    int my = pointList[1];

    for(int i = 2; i < pointList.size() - 2; i+= 2)
    {
        drawTriangle(mx, my, pointList[i], pointList[i + 1], pointList[i + 2], pointList[i + 3], color);    
    }
}


/*
             (x, y).---------.(x + width, y)
                   |         |
                   |         |
                   |         |
                   |         |
    (x, y + height).---------.(x + width, y + height)
*/
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
    for (size_t y = 0; y < m_context.fizikselYukseklik; y++)
    {
        for (size_t x = 0; x < m_context.fizikselGenislik; x++)
        {
            if (x % 64 == 0 || y % 64 == 0)
            {
                m_context.colorBuffer[y * m_context.fizikselGenislik + x] = color;
            }
        }
    }
}

void Graphics::drawDots(Color_t color)
{
    for (size_t y = 0; y < m_context.fizikselYukseklik; y+= 40)
    {
        for (size_t x = 0; x < m_context.fizikselGenislik; x+= 40)
        {
            m_context.colorBuffer[y * m_context.fizikselGenislik + x] = color;
        }
    }
}

void Graphics::drawFilledTriangle(int x0, int y0, int x1, int y1, int x2, int y2, Color_t color)
{    
    ZoneScopedN(__FUNCTION__);

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

void Graphics::drawTexturedTriangle(
    int x0, int y0, float u0, float v0,
    int x1, int y1, float u1, float v1,
    int x2, int y2, float u2, float v2,
    std::string textureId)
{
    ZoneScopedN(__FUNCTION__);

    //y0 < y1 < y2
    if (y0 > y1)
    {
        swap(y0, y1);
        swap(x0, x1);

        swap(u0, u1);
        swap(v0, v1);
    }
    if (y1 > y2)
    {
        swap(y1, y2);
        swap(x1, x2);

        swap(u1, u2);
        swap(v1, v2);
    }
    if (y0 > y1)
    {
        swap(y0, y1);
        swap(x0, x1);

        swap(u0, u1);
        swap(v0, v1);
    }


    /*
           x0,y0
             .
            / \
           /   \
    x1,y1.      .x2,y2


    */


    //-----------------------------Ust--------------------------------//
    float invSlopeLeft = 0;
    float invSlopeRight = 0;

    if (y1 - y0 != 0)
    {
        invSlopeLeft  = (float)(x1 - x0) / abs(y1 - y0);
    }
    if (y2 - y0 != 0)
    {
        invSlopeRight = (float)(x2 - x0) / abs(y2 - y0);
    }

    

   /* std::cout << "v0: " << v0
        << " v1: " << v1
        << "\ny0: " << y0
        << " y1: " << y1
        << "\ndelta_v: " << delta_v
        << "\n======================\n"
        << "u0 " << u0 << " v0 " << v0 << "\n"
        << "u1 " << u1 << " v1 " << v1 << "\n"
        << "u2 " << u2 << " v2 " << v2
        << "\n";*/
    //float deltay = (y1 - y0);
    //if(deltay > 0.0f)
    //  delta_v = (v1 - v0) / deltay
    //else
    //  delta_v = 0.0f
    //float delta_v = (deltay > 0.0f) ? (v1 - v0) / deltay : 0.0f;

    float current_u = u0;
    float current_v = v0;
    
    //float dx10 = x1 - x0;
    //float dx20 = x2 - x0;
    float dy10 = y1 - y0;
    float dy20 = y2 - y0;

    
    float delta_vSol = (v1 - v0) / dy10;
    float delta_vSag = (v2 - v0) / dy20;

    //std::cout << "delta_vSag" << delta_vSag << "\n";

    //bacaklar
    for (int y = y0; y < y1; y++)
    {
        //y = ax + b dogrusal denklem
        float startx = (y - y1) * invSlopeLeft + x1;
        float endx   = (y - y0) * invSlopeRight + x0;
                

        float deltax = endx - startx;

        float delta_uSol = (u2 - u1) / deltax;
        //float delta_uSag = (u2 - u0) / deltax;

        //bu deltalar kullanim icin guvenlimi (delta != nan, inf, -inf)
        if (!std::isfinite(delta_uSol))
        {
            delta_uSol = 0;
            //delta_uSag = 0;
        }
        
       // spdlog::info("delta_uSag {}", delta_uSag);

        if (endx < startx)
        {
            swap(startx, endx);

        }

        int tex_y = (int)(current_v * (m_textureMap[textureId].height - 1));
        int tex_x = 0;
        
        //tex_y = m_TEST_tex_y;

        //float deltax = (startx - endx);
        //float delta_u = (deltax > 0.0f) ? (u1 - u2) / deltax : 0.0f;
        

        //ara bolge
        for (int x = startx; x < endx; x++)
        {                                
            tex_x = 0; //(int)(current_u* (m_textureMap[textureId].width - 1));

            tex_x = std::clamp(tex_x, 0, (m_textureMap[textureId].width - 1));
            tex_y = std::clamp(tex_y, 0, (m_textureMap[textureId].height - 1));

            drawPixel(x, y, m_textureMap[textureId].data[tex_y * m_textureMap[textureId].width + tex_x]);           

            //spdlog::info("current_u {}", current_u);
            current_u += delta_uSol;
            
        }
        
        current_v += delta_vSol;

        
    }

    //-------------------------------Alt--------------------------------//
    invSlopeLeft = 0;
    invSlopeRight = 0;

    if (y2 - y1 != 0)
    {
        invSlopeLeft = (float)(x2 - x1) / abs(y2 - y1);
    }
    if (y2 - y0 != 0)
    {
        invSlopeRight = (float)(x2 - x0) / abs(y2 - y0);
    }

    for (int y = y1; y < y2; y++)
    {
        float startx = (y - y1) * invSlopeLeft  + x1;
        float endx   = (y - y0) * invSlopeRight + x0;        

        if (endx < startx)
        {
            swap(startx, endx);
        }

        for (int x = startx; x < endx; x++)
        {
            drawPixel(x, y, Color::RED);
        }
    }

}

void Graphics::drawTexturedTriangle_Barycentric(
    int x0, int y0, float u0, float v0,
    int x1, int y1, float u1, float v1,
    int x2, int y2, float u2, float v2,
    std::string textureId)
{
    ZoneScopedN(__FUNCTION__);

    //y0 < y1 < y2
    if (y0 > y1)
    {
        swap(y0, y1);
        swap(x0, x1);

        swap(u0, u1);
        swap(v0, v1);
    }
    if (y1 > y2)
    {
        swap(y1, y2);
        swap(x1, x2);

        swap(u1, u2);
        swap(v1, v2);
    }
    if (y0 > y1)
    {
        swap(y0, y1);
        swap(x0, x1);

        swap(u0, u1);
        swap(v0, v1);
    }


    /*
           x0,y0
             .
            / \
           /   \
    x1,y1.      .x2,y2


    */

    Vector2 a(x0, y0);
    Vector2 b(x1, y1);
    Vector2 c(x2, y2);



    //-----------------------------Ust--------------------------------//
    float invSlopeLeft = 0;
    float invSlopeRight = 0;

    if (y1 - y0 != 0)
    {
        invSlopeLeft = (float)(x1 - x0) / abs(y1 - y0);
    }
    if (y2 - y0 != 0)
    {
        invSlopeRight = (float)(x2 - x0) / abs(y2 - y0);
    }       

    //bacaklar
    for (int y = y0; y < y1; y++)
    {
        //y = ax + b dogrusal denklem
        float startx = (y - y1) * invSlopeLeft + x1;
        float endx = (y - y0) * invSlopeRight + x0;        

        if (endx < startx)
        {
            swap(startx, endx);

        }
       
        //ara bolge
        for (int x = startx; x < endx; x++)
        {           
            //drawPixel(x, y, Color::BLUE);
            drawTexel(x, y, a, b, c, u0, v0, u1, v1, u2, v2, textureId);
        }
    }

    //-------------------------------Alt--------------------------------//
    invSlopeLeft = 0;
    invSlopeRight = 0;

    if (y2 - y1 != 0)
    {
        invSlopeLeft = (float)(x2 - x1) / abs(y2 - y1);
    }
    if (y2 - y0 != 0)
    {
        invSlopeRight = (float)(x2 - x0) / abs(y2 - y0);
    }

    for (int y = y1; y < y2; y++)
    {
        float startx = (y - y1) * invSlopeLeft + x1;
        float endx = (y - y0) * invSlopeRight + x0;

        if (endx < startx)
        {
            swap(startx, endx);
        }

        for (int x = startx; x < endx; x++)
        {
            drawTexel(x, y, a, b, c, u0, v0, u1, v1, u2, v2, textureId);
        }
    }

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
    float invSlopeLeft = (float)(x2 - x0) / (y2 - y0);
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



void Graphics::setLineAlgo(LineAlgoType lineAlgoType)
{
    m_lineAlgoType = lineAlgoType;
}

void Graphics::drawColorBuffer()
{
    ZoneScopedN(__FUNCTION__);

    //load colorbuffer
    SDL_UpdateTexture(m_context.canvas, NULL, m_context.colorBuffer, (int)(m_context.fizikselGenislik * sizeof(Color_t)));

    //render canvas
    SDL_RenderTexture(m_context.renderer, m_context.canvas, NULL, NULL);

}

Vector3 Graphics::barycentricWeights(Vector2 a, Vector2 b, Vector2 c, Vector2 p)
{
    Vector2 ac = c - a;
    Vector2 ab = b - a;
    Vector2 pc = c - p;
    Vector2 pb = b - p;
    Vector2 ap = p - a;

    //z ekrana dik 
    float areaABC = ac.x * ab.y - ac.y * ab.x;  // ||AC x AB ||

    /*
              || PC x PB ||
      alfa = -------------
              || AC x AB ||
    */
    float alfa = (pc.x * pb.y - pc.y * pb.x) / areaABC;

    /*
              || AC x AP ||
       beta = -------------
              || AC x AB ||
    */
    float beta = (ac.x * ap.y - ac.y * ap.x) / areaABC;

    /*
        alfa + beta + gamma = 1

        gamma = 1 - alfa - beta
    */

    float gamma = 1.0f - alfa - beta;

    Vector3 weight{ alfa, beta, gamma };

    return weight;

}

void Graphics::swap(int& a, int& b)
{
    int temp = a;
    a = b;
    b = temp;
}

void Graphics::swap(float& a, float& b)
{
    float temp = a;
    a = b;
    b = temp;
}
