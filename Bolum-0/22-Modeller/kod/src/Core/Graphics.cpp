#include "Graphics.h"

Graphics::Graphics(RenderContext& context)
    :m_context(context)
{   
    
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
}

void Graphics::clearColorBuffer(Color_t color)
{
    ZoneScopedN(__FUNCTION__);

    const size_t totalPixels = static_cast<size_t>(m_context.fizikselGenislik) * m_context.fizikselYukseklik;
    std::fill(m_context.colorBuffer, m_context.colorBuffer + totalPixels, color);
}

void Graphics::clearZbuffer()
{
    ZoneScopedN(__FUNCTION__);

    const size_t totalPixels = static_cast<size_t>(m_context.fizikselGenislik) * m_context.fizikselYukseklik;
    std::fill(m_context.zbuffer, m_context.zbuffer + totalPixels, 1.0f);    
}

inline void Graphics::drawPixel(int x, int y, Color_t color)
{
    ZoneScopedN(__FUNCTION__);

    if (x >= 0 && x < m_context.fizikselGenislik && y >= 0 && y < m_context.fizikselYukseklik)
    {
        m_context.colorBuffer[y * m_context.fizikselGenislik + x] = color;
    }
}

inline void Graphics::drawPixel_zbuffer(int x, int y, Vector4 a, Vector4 b, Vector4 c, Color_t color)
{
    ZoneScopedN(__FUNCTION__);

    if (x < 0 || x >= m_context.fizikselGenislik ||
        y < 0 || y >= m_context.fizikselYukseklik)
    {
        return;
    }

    glm::vec2 p(x, y);

    glm::vec3 weight = barycentricWeights(
        glm::vec2(a.x, a.y),
        glm::vec2(b.x, b.y),
        glm::vec2(c.x, c.y),
        p);

    float alfa = weight.x;
    float beta = weight.y;
    float gamma = weight.z;

    float interpolated_w = (1 / a.w) * alfa + (1 / b.w) * beta + (1 / c.w) * gamma;

    interpolated_w = 1.0f - interpolated_w;

    if (interpolated_w < m_context.zbuffer[y * m_context.fizikselGenislik + x])
    {
        drawPixel(x, y, color);
        
        m_context.zbuffer[y * m_context.fizikselGenislik + x] = interpolated_w;        
    }
}

void Graphics::drawTexel(
    int x, int y, 
    Vector2 a, Vector2 b, Vector2 c,
    float u0, float v0,
    float u1, float v1,
    float u2, float v2)
{
    glm::vec2 p(x, y);

    glm::vec3 weight = barycentricWeights(
        glm::vec2(a.x, a.y),
        glm::vec2(b.x, b.y),
        glm::vec2(c.x, c.y),
        p);

    float alfa = weight.x;
    float beta = weight.y;
    float gamma = weight.z;

    float interpolated_u = u0 * alfa + u1 * beta + u2 * gamma;
    float interpolated_v = v0 * alfa + v1 * beta + v2 * gamma;

    //int tex_x = abs((int)(interpolated_u * m_textureMap[textureid].width));
    //int tex_y = abs((int)(interpolated_v * m_textureMap[textureid].height));

    int texWidth = m_textureMap[m_currentTextureId].width;
    int texHeight = m_textureMap[m_currentTextureId].height;


    int tex_x = std::clamp(static_cast<int>(interpolated_u * (texWidth - 1)), 0, texWidth - 1);
    int tex_y = std::clamp(static_cast<int>(interpolated_v * (texHeight - 1)), 0, texHeight - 1);

    //int tex_y = std::clamp(static_cast<int>(interpolated_v * (m_textureMap[textureid].height - 1)), 0, m_textureMap[textureid].height - 1);

    int coords = tex_y * texWidth + tex_x;
    
    drawPixel(x, y, m_textureMap[m_currentTextureId].data[coords]);    
}

void Graphics::drawTexel_persCorrect(
    int x, int y, 
    Vector4 a, Vector4 b, Vector4 c, 
    float u0, float v0, 
    float u1, float v1, 
    float u2, float v2)
{
    glm::vec2 p(x, y);

    glm::vec3 weight = barycentricWeights(
        glm::vec2(a.x, a.y),
        glm::vec2(b.x, b.y),
        glm::vec2(c.x, c.y),
        p);

    float alfa = weight.x;
    float beta = weight.y;
    float gamma = weight.z;

    float interpolated_u;
    float interpolated_v;
    float interpolated_w;

    interpolated_u = (u0 / a.w) * alfa + (u1 / b.w) * beta + (u2 / c.w) * gamma;
    interpolated_v = (v0 / a.w) * alfa + (v1 / b.w) * beta + (v2 / c.w) * gamma;

    interpolated_w = (1 / a.w) * alfa + (1 / b.w) * beta + (1 / c.w) * gamma;

    interpolated_u /= interpolated_w;
    interpolated_v /= interpolated_w;

    //int tex_x = abs((int)(interpolated_u * m_textureMap[textureid].width));
    //int tex_y = abs((int)(interpolated_v * m_textureMap[textureid].height));

    interpolated_w = 1.0f - interpolated_w;

    if (interpolated_w < m_context.zbuffer[y * m_context.fizikselGenislik + x])
    {
        int texWidth = m_textureMap[m_currentTextureId].width;
        int texHeight = m_textureMap[m_currentTextureId].height;

        int tex_x = std::clamp(static_cast<int>(interpolated_u * (texWidth - 1)), 0, texWidth - 1);
        int tex_y = std::clamp(static_cast<int>(interpolated_v * (texHeight - 1)), 0, texHeight - 1);

        //int tex_y = std::clamp(static_cast<int>(interpolated_v * (m_textureMap[textureid].height - 1)), 0, m_textureMap[textureid].height - 1);
        int coords = tex_y * texWidth + tex_x;


        drawPixel(x, y, m_textureMap[m_currentTextureId].data[coords]);

        m_context.zbuffer[y * m_context.fizikselGenislik + x] = interpolated_w;
    }    
}

inline void Graphics::bresenhamLineAlgo(int x0, int y0, int x1, int y1, Color_t color)
{
    ZoneScopedN(__FUNCTION__);

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

void Graphics::drawTriangle(int x0, int y0, int x1, int y1, int x2, int y2, Color_t color)
{
    bresenhamLineAlgo(x0, y0, x1, y1, color);
    bresenhamLineAlgo(x1, y1, x2, y2, color);
    bresenhamLineAlgo(x2, y2, x0, y0, color);
}

void Graphics::drawTriangle_zbuffer(
    glm::ivec4 v0,
    glm::ivec4 v1,
    glm::ivec4 v2,
    Color_t color)
{
    ZoneScopedN(__FUNCTION__);

    //y0 < y1 < y2
    if (v0.y > v1.y)
    {
        std::swap(v0, v1);
    }
    if (v1.y > v2.y)
    {
        std::swap(v1, v2);
    }
    if (v0.y > v1.y)
    {
        std::swap(v0, v1);
    }

    //====================================================//

    float invSlopeLeft = 0;
    float invSlopeRight = 0;

    if (v1.y - v0.y != 0)
    {
        invSlopeLeft = (float)(v1.x - v0.x) / abs(v1.y - v0.y);
    }
    if (v2.y - v0.y != 0)
    {
        invSlopeRight = (float)(v2.x - v0.x) / abs(v2.y - v0.y);
    }

    //bacaklar
    for (int y = v0.y; y < v1.y; y++)
    {
        //y = ax + b dogrusal denklem
        float startx = (y - v1.y) * invSlopeLeft + v1.x;
        float endx = (y - v0.y) * invSlopeRight + v0.x;

        if (endx < startx)
        {
            std::swap(startx, endx);
        }

        //ara bolge
        for (int x = startx; x < endx; x++)
        {
            //drawPixel(x, y, Color::BLUE);
            //drawPixel_zbuffer(x, y, a, b, c, color);

            if (x < 0 || x >= m_context.fizikselGenislik ||
                y < 0 || y >= m_context.fizikselYukseklik)
            {
                continue;
            }

            glm::vec2 p(x, y);

            glm::vec3 weight = barycentricWeights(v0, v1, v2, p);      

            float interpolated_w = (1.0f / v0.w) * weight.x + (1.0f / v1.w) * weight.y + (1.0f / v2.w) * weight.z;

            interpolated_w = 1.0f - interpolated_w;

            if (interpolated_w < m_context.zbuffer[y * m_context.fizikselGenislik + x])
            {
                //yukarda sinir kontrolu yapilmakta
                m_context.colorBuffer[y * m_context.fizikselGenislik + x] = color;

                m_context.zbuffer[y * m_context.fizikselGenislik + x] = interpolated_w;
            }
        }
    }

    //-------------------------------Alt--------------------------------//
    invSlopeLeft = 0;
    invSlopeRight = 0;

    if (v2.y - v1.y != 0)
    {
        invSlopeLeft = (float)(v2.x - v1.x) / abs(v2.y - v1.y);
    }
    if (v2.y - v0.y != 0)
    {
        invSlopeRight = (float)(v2.x - v0.x) / abs(v2.y - v0.y);
    }

    for (int y = v1.y; y < v2.y; y++)
    {
        float startx = (y - v1.y) * invSlopeLeft + v1.x;
        float endx = (y - v0.y) * invSlopeRight + v0.x;

        if (endx < startx)
        {
            std::swap(startx, endx);
        }

        for (int x = startx; x < endx; x++)
        {
            if (x <= 0 || x > m_context.fizikselGenislik ||
                y <= 0 || y > m_context.fizikselYukseklik)
            {
                continue;
            }

            glm::vec2 p(x, y);

            glm::vec3 weight = barycentricWeights(v0, v1, v2, p);     

            float interpolated_w = (1.0f / v0.w) * weight.x + (1.0f / v1.w) * weight.y + (1.0f / v2.w) * weight.z;

            interpolated_w = 1.0f - interpolated_w;

            if (interpolated_w < m_context.zbuffer[y * m_context.fizikselGenislik + x])
            {
                //yukarda sinir kontrolu yapilmakta
                m_context.colorBuffer[y * m_context.fizikselGenislik + x] = color;
                m_context.zbuffer[y * m_context.fizikselGenislik + x] = interpolated_w;
            }
        }
    }
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
    bresenhamLineAlgo(x, y, x + width, y, color);
    bresenhamLineAlgo(x + width, y, x + width, y + height, color);
    bresenhamLineAlgo(x + width, y + height, x, y + height, color);
    bresenhamLineAlgo(x, y + height, x, y, color);
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

void Graphics::drawTexturedTriangle_Barycentric(
    int x0, int y0, float u0, float v0,
    int x1, int y1, float u1, float v1,
    int x2, int y2, float u2, float v2)
{
    ZoneScopedN(__FUNCTION__);

    //y0 < y1 < y2
    if (y0 > y1)
    {
        std::swap(y0, y1);
        std::swap(x0, x1);

        std::swap(u0, u1);
        std::swap(v0, v1);
    }
    if (y1 > y2)
    {
        std::swap(y1, y2);
        std::swap(x1, x2);

        std::swap(u1, u2);
        std::swap(v1, v2);
    }
    if (y0 > y1)
    {
        std::swap(y0, y1);
        std::swap(x0, x1);

        std::swap(u0, u1);
        std::swap(v0, v1);
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
            std::swap(startx, endx);
        }
       
        //ara bolge
        for (int x = startx; x < endx; x++)
        {           
            //drawPixel(x, y, Color::BLUE);
            drawTexel(x, y, a, b, c, u0, v0, u1, v1, u2, v2);
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
            std::swap(startx, endx);
        }

        for (int x = startx; x < endx; x++)
        {
            drawTexel(x, y, a, b, c, u0, v0, u1, v1, u2, v2);
        }
    }

    //spdlog::info("ucgen gazeteye sariliyor...");

}

void Graphics::drawTexturedTriangle_Barycentric_PerspectiveCorrect(
    int x0, int y0, int z0, float w0, float u0, float v0, 
    int x1, int y1, int z1, float w1, float u1, float v1, 
    int x2, int y2, int z2, float w2, float u2, float v2)
{
    ZoneScopedN(__FUNCTION__);

    //y0 < y1 < y2
    if (y0 > y1)
    {
        std::swap(y0, y1);
        std::swap(x0, x1);

        std::swap(z0, z1);
        std::swap(w0, w1);

        std::swap(u0, u1);
        std::swap(v0, v1);
    }
    if (y1 > y2)
    {
        std::swap(y1, y2);
        std::swap(x1, x2);

        std::swap(z1, z2);
        std::swap(w1, w2);

        std::swap(u1, u2);
        std::swap(v1, v2);
    }
    if (y0 > y1)
    {
        std::swap(y0, y1);
        std::swap(x0, x1);

        std::swap(z0, z1);
        std::swap(w0, w1);

        std::swap(u0, u1);
        std::swap(v0, v1);
    }


    /*
           x0,y0
             .
            / \
           /   \
    x1,y1.      .x2,y2


    */

    Vector4 a(x0, y0, z0, w0);
    Vector4 b(x1, y1, z1, w1);
    Vector4 c(x2, y2, z2, w2);



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
            std::swap(startx, endx);
        }

        //ara bolge
        for (int x = startx; x < endx; x++)
        {
            //drawPixel(x, y, Color::BLUE);
            drawTexel_persCorrect(x, y, a, b, c, u0, v0, u1, v1, u2, v2);
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
            std::swap(startx, endx);
        }

        for (int x = startx; x < endx; x++)
        {
            drawTexel_persCorrect(x, y, a, b, c, u0, v0, u1, v1, u2, v2);
        }
    }
}


void Graphics::drawColorBuffer()
{
    ZoneScopedN(__FUNCTION__);

    //load colorbuffer
    SDL_UpdateTexture(m_context.canvas, NULL, m_context.colorBuffer, (int)(m_context.fizikselGenislik * sizeof(Color_t)));

    //render canvas
    SDL_RenderTexture(m_context.renderer, m_context.canvas, NULL, NULL);

}

inline glm::vec3 Graphics::barycentricWeights(glm::vec2 a, glm::vec2 b, glm::vec2 c, glm::vec2 p)
{
    glm::vec2 ac = c - a;
    glm::vec2 ab = b - a;
    glm::vec2 pc = c - p;
    glm::vec2 pb = b - p;
    glm::vec2 ap = p - a;

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

    glm::vec3 weight{ alfa, beta, gamma };

    return weight;

}

