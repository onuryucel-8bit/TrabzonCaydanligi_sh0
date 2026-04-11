![donenNokta](../resimler/donenNokta.gif)

#### [desmos ucgen ornegi](https://www.desmos.com/calculator/cemr6p7ctp)


**main.cpp**
```cpp

float x = 400, y = 400;
float alfa = 0;

void update()
{
    // float x2 = x * cos(theta) - y * sin(theta);
    // float y2 = x * sin(theta) + y * cos(theta);

    //ekranin ortasi
    float cx = rcontext.WindowWidth / 2.0f;
    float cy = rcontext.WindowHeight / 2.0f;

    //noktayi ekranin ortasina tasi
    float px = x - cx;
    float py = y - cy;

    //dondur
    float rx = px * cos(alfa) - py * sin(alfa);
    float ry = px * sin(alfa) + py * cos(alfa);

    //noktayi ekran uzayina geri donustur
    x = rx + cx;
    y = rx + cy;


    //usengeclikten f(x) => alfa++ => g(x)
    //normalde alfa += RADIAN_D90 gibi bisey yazilmasi daha iyi olur
    alfa = radToDeg(alfa);

    alfa += 0.00001;

    alfa = degToRad(alfa);
}
```
![donenUcgen](../resimler/donenUcgen.gif)

```cpp

Vector2 trig[3] = { {390,290} , {370,250}, {350,290} };

float alfa = 0;

void update()
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

void draw()
{

    //------------------------------//
    SDL_RenderClear(rcontext.renderer);

    gp.clearColorBuffer(Color::BLACK);

    gp.drawTriangle(
        trig[0].x, trig[0].y,
        trig[1].x, trig[1].y,
        trig[2].x, trig[2].y,
        Color::WHITE);

    gp.drawLine(0, rcontext.WindowHeight / 2, rcontext.WindowWidth, rcontext.WindowHeight / 2, Color::GREEN);

    gp.drawLine(rcontext.WindowWidth / 2,0, rcontext.WindowWidth / 2, rcontext.WindowHeight, Color::GREEN);

    gp.drawDots(Color::GREEN);

    gp.drawColorBuffer();

    //swap buffers
    SDL_RenderPresent(rcontext.renderer);
    //------------------------------//
}

```
