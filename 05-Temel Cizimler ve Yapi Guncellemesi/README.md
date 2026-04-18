<h2>Yapi Guncellemesi</h2>

Simdi su cizim fonksiyonlarini alip ayri bir dosya icerisine atalim ki kod okurken ve ekleme yaparken kafa agrisi vermesin.

***Graphics.h***
```cpp
class Graphics
{
public:
	Graphics(RenderContext& context);
	~Graphics();

	void clearColorBuffer(Color_t color);
	void drawColorBuffer();

	/*
	* @brief Renk tamponundaki tum pikseller belirtilen renk ile temizlenir
	* @param color renk degeri(uint32_t)
	*/
	void drawPixel(int x, int y, Color_t color);
    ...
    ...
}
```

Pencere, renderer, kaplama ve diger bilesenleri paketleyip iki yapi arasindaki iletisimi saglayacak paket/yapi olusturalim

***Defs.h***
```cpp
struct RenderContext
{
	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;
	SDL_Texture* canvas = nullptr;

	Color_t* colorBuffer = nullptr;
	
	const int WindowWidth = 800;
	const int WindowHeight = 600;
};
```
- Main: Pencere yonetimi, girdi islemleri ve pozisyon guncelleme gibi fonksiyonlarin tutuldugu ana yapi olacaktir

- Graphics: piksel, cizgi cizme, ucgen cizme ve benzeri grafiksel fonksiyonlarin bulundugu yapi olacaktir

\
![yapi](../resimler/yapi.png)


<h2>Cizimler</h2>

![ucgen](../resimler/2026-04-10%2021-16-43.png)

```cpp
/*
*        (x0, y0).
*               / \
*              /   \
*             /     \
*            /       \
*           /         \
*  (x2, y2).-----------.(x1, y1)
*
*/
void Graphics::drawTriangle(int x0, int y0, int x1, int y1, int x2, int y2, Color_t color)
{
    drawLine(x0, y0, x1, y1, color);
    drawLine(x1, y1, x2, y2, color);
    drawLine(x2, y2, x0, y0, color);
}

```

![kare](../resimler/2026-04-10%2021-29-09.png)

```cpp
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
```

![ici dolu kare](../resimler/2026-04-10%2021-35-36.png
)

```cpp

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

```

![yelpaze](../resimler/2026-04-18%2015-32-08.png)

```cpp

//main.cpp / draw()
 std::vector<int> pt =
 {
     //--merkez--//
     400,100,
     //----------//

     450, 50,
     450, 100,
     450, 200,
     300, 200
     
 };

 gp.drawTriangleFan(pt, Color::GREEN);

/*
*	Merkez([x0][y0]) ucgenNoktalari( [x1][y1] [x2][y2] .... [xn][yn] )
*/
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

```
