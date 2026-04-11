![donenNokta](../resimler/donenNokta.gif)

#### [desmos ucgen ornegi](https://www.desmos.com/calculator/cemr6p7ctp)


```cpp
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