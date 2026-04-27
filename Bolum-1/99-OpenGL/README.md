
<h2>Kurulum</h2>

[glfw](https://www.glfw.org)

[glad](https://glad.dav1d.de)

- glad kurulumu ekle

- cmake imgui/glad/glfw

<h2> </h2>

![openglCizgiCizimi](resimler/2026-04-22%2008-58-03.png)

- cizgi cizdirme

```cpp
void draw()
{
    //------------------------------//    
   
    glad_glBegin(GL_LINES);

    glad_glColor3f(1.0f, 0.0f, 0.0f);

    glad_glVertex2f(-0.5f, 0.5f);
    glad_glVertex2f(0.0f, 0.0f);

    glad_glEnd();
    //------------------------------//
    
}
```

<h2> </h2>

- ici bos ucgen cizdirme

![ucgen](resimler/2026-04-22%2009-04-29.png)

```cpp
void draw()
{
    //------------------------------//   
    glad_glBegin(GL_LINE_LOOP);

    glad_glColor3f(1.0f, 0.0f, 0.0f);

    glad_glVertex2f(-0.5f, -0.5f);
    glad_glVertex2f(0.5f, -0.5f);
    glad_glVertex2f(0.0f, 0.5f);

    glad_glEnd();
    //------------------------------//    
}
```