![nokta bulutu](../resimler/2026-04-20%2021-34-37.png)


nokta koordinalari yukleniyor
```cpp
void loadCube()
{
    float incz = 0.20f;
    float incy = 0.20f;
    float incx = 0.20f;

    for (float z = -1; z <= 1.0f; z += incz)
    {
        for (float y = -1; y <= 1.0f; y += incy)
        {
            for (float x = -1; x <= 1.0f; x+= incx)
            {
                Vector3 vec(x, y, z);

                modelPoints.push_back(vec);
            }
        }
    }    
}
```

Blender perspektif ornegi ekle


```cpp
Vector2 projectOrtho(Vector3 vec)
{
    return Vector2
    { 
        vec.x * FOV_factor,
        vec.y * FOV_factor
    };
}

Vector2 projectPerspective(Vector3 vec)
{
    return Vector2
    {
        (vec.x * FOV_factor) / vec.z,
        (vec.y * FOV_factor) / vec.z
    };
}

Vector2 project(Vector3 vec)
{
    if (f_proj)
    {
        return projectPerspective(vec);
    }
    return projectOrtho(vec);
}

void drawImgui()
{
    ImGui_ImplSDLRenderer3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();
    
    ImGui::Begin("Kontrol Paneli");
        
    ImGui::Checkbox("Perspektif", &f_proj);

    ImGui::NewLine();
    ImGui::Text("Ekran mx,my %f , %f",mouseX , mouseY);
  
    ImGui::End();

    ImGui::Render();
    ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), rcontext.renderer);
}
```

```cpp
initSDL()
{
    ...
    ...

    loadCube();
    projectedPoints.resize(modelPoints.size());

}
```

```cpp
void draw()
{
    //------------------------------//    

    gp.clearColorBuffer(Color::BLACK);    

    //gp.drawDots(Color::GREEN);

    for (size_t i = 0; i < projectedPoints.size(); i++)
    {
        gp.drawFilledRectangle(
            projectedPoints[i].x,
            projectedPoints[i].y,
            3,
            3,
            Color::GREEN
        );
    }

    gp.drawColorBuffer();
    
    //------------------------------//
} 
```

<h2> </h2>

dondurme kodunu bir sonraki basliga koy

- dondurme
- indeks tamponu ekle
- obj dosya okuyucu
- main yapisini tasi
- derinlik siralamasi
- eski legacy opengl ile baglantiyi anlat