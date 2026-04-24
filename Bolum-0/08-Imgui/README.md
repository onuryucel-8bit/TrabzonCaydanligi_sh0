<h2> imgui kurulumu</h2>

https://github.com/ocornut/imgui/releases/tag/v1.92.7-docking

imgui indirip libsLocal icine yerlestiriyoruz

```
|-libsLocal
    |-imgui-docking
        |-imgui
        |-CMakeLists.txt
```

**imgui-docking/CMakeLists.txt**

Olusturmus oldugumuz bu cmake dosyasini imgui-docking/ icine yerlestirip ana cmake dosyasindan cagriyoruz

```cmake
cmake_minimum_required(VERSION 3.15...4.0)
project(imgui)

add_library(imgui)
target_sources(imgui PRIVATE 
"${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_demo.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_draw.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_tables.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_widgets.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends/imgui_impl_sdl3.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends/imgui_impl_sdlrenderer3.cpp"
)

target_include_directories(imgui PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/imgui")

target_link_libraries(imgui PUBLIC SDL3::SDL3)

```

***Proje/CMakeLists.txt***

imgui kutuphanesini projeye ekliyoruz

```cmake
# 3.15 - 4.0 versiyon araliginda cmake varsa calistir
cmake_minimum_required(VERSION 3.15...4.0)

# projenin ismi
project(TrabzonCaydanligi)

# cmake degiskenleri tanimlaniyor

# c++ versiyonu ayarlaniyor
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# kutuphanelerin dosya yolu
set(LIBS_LOCAL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libsLocal)


#====================LIBS=============================================#
#=====================================================================#

add_subdirectory(${LIBS_LOCAL_DIR}/SDL3 SDL_build)
add_subdirectory(${LIBS_LOCAL_DIR}/imgui-docking imgui_build)

#=====================================================================#
#=====================================================================#

# supurge teknigi ile proje dosyalari MY_SOURCES icerisine atiliyor
file(GLOB_RECURSE MY_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")


add_executable(${PROJECT_NAME} ${MY_SOURCES})

#====================LIBS=============================================#
#=====================================================================#

target_link_libraries(${PROJECT_NAME}
    PRIVATE
    SDL3::SDL3
    imgui
)

#=====================================================================#
#=====================================================================#
```

<h2> Imgui deneme</h2>

```cpp
int main()
{
    
    initSDL();
    initImgui();

    rcontext.colorBuffer = new Color_t[rcontext.WindowWidth * rcontext.WindowHeight];
    
    while(f_running)
    {
        inputs();

        update();

        SDL_RenderClear(rcontext.renderer);
        drawImgui();
        draw();
        //swap buffers
        SDL_RenderPresent(rcontext.renderer);
    }
}
```

```cpp
void initImgui()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

     io = &ImGui::GetIO();

    // Enable Docking
    io->ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io->FontGlobalScale = 1;

    ImGui_ImplSDL3_InitForSDLRenderer(rcontext.window, rcontext.renderer);
    ImGui_ImplSDLRenderer3_Init(rcontext.renderer);
}
```

```cpp
void drawImgui()
{
    ImGui_ImplSDLRenderer3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();
    
    ImGui::Begin("txt");
    ImGui::Text("hmm..");
    ImGui::End();

    ImGui::Render();
    ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), rcontext.renderer);
}

```

```cpp
void inputs()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        ImGui_ImplSDL3_ProcessEvent(&event);

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
}
```

<h2>Kontrol Paneli </h2>

![](resimler/2026-04-13%2013-49-30.png)


```
Vector2 trigStatic[3] = { {390,290} , {370,250}, {350,290} };
Vector2 trig[3] = { {390,290} , {370,250}, {350,290} };

Vector2 trigTranslate(0, 0);
Vector2 trigScale(1.0f, 1.0f);
```

```cpp
void drawImgui()
{
    ImGui_ImplSDLRenderer3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();
    
    ImGui::Begin("Dondurme Zimbirtisi");

    ImGui::NewLine();
    ImGui::SliderFloat("dondurme", &alfa, 0, std::numbers::pi * 2);

    ImGui::NewLine();
    ImGui::SliderFloat("x", &trigTranslate.x, -rcontext.WindowWidth, rcontext.WindowWidth);

    ImGui::NewLine();
    ImGui::SliderFloat("y", &trigTranslate.y, -rcontext.WindowHeight, rcontext.WindowHeight);

    ImGui::NewLine();
    ImGui::SliderFloat("sx", &trigScale.x, -5, 5);

    ImGui::NewLine();
    ImGui::SliderFloat("sy", &trigScale.y, -5, 5);

    ImGui::End();

    ImGui::Render();
    ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), rcontext.renderer);
}
```

```cpp
void update()
{ 

    //ekranin ortasi
    float cx = rcontext.WindowWidth / 2.0f;
    float cy = rcontext.WindowHeight / 2.0f;

    for (size_t i = 0; i < 3; i++)
    {
        //noktayi ekranin ortasina tasi
        float px = trigStatic[i].x - cx;
        float py = trigStatic[i].y - cy;

        //buyut/kucult
        px *= trigScale.x;
        py *= trigScale.y;

        //dondur
        float rx = px * cos(alfa) - py * sin(alfa);
        float ry = px * sin(alfa) + py * cos(alfa);

        //tasi
        rx += trigTranslate.x;
        ry += trigTranslate.y;

        //noktayi ekran uzayina geri donustur
        trig[i].x = rx + cx;
        trig[i].y = ry + cy;
    }   
}
```
