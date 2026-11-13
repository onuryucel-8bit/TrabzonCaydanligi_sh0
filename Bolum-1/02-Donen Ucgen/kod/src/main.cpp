#include "pch.h"
#include "makroVeTanimlar/TracyLib.h"
#include "Shader.h"

#define u_transform "u_transform"

glm::vec3 alfa = glm::vec3(0);
glm::vec3 olcek = glm::vec3(1);
glm::vec3 pozisyon = glm::vec3(0);

Shader shader;
GLuint VBO, VAO;

//Pencere
//====================================================================================//
SDL_Window* window = nullptr;
SDL_GLContext glContext = nullptr;
int width = 800;
int height = 600;
bool f_running = true;

float FPS = 0;
float dt = 0;
//====================================================================================//

float r = 0.1f;
float g = 0.1f;
float b = 0.1f;


//====================================================================================//
//====================================================================================//
//====================================================================================//

#pragma region INIT

void initImgui()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    // Enable Docking
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;



    ImGui_ImplSDL3_InitForOpenGL(window, glContext);
    ImGui_ImplOpenGL3_Init("#version 330 core");
}

bool initSDL()
{
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(
        SDL_GL_CONTEXT_PROFILE_MASK,
        SDL_GL_CONTEXT_PROFILE_CORE
    );

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    //saydam arka plan ekle

    window = SDL_CreateWindow("Trabzon Caydanligi", 800, 600, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

    if (window == NULL)
    {
        spdlog::error("Pencere acilamadi");
        return false;
    }

    glContext = SDL_GL_CreateContext(window);

    if (glContext == NULL)
    {
        spdlog::error("Context zimbirtisi olusturulamadi");
        return false;
    }

    //SDL_GL_SetSwapInterval(-1);

    return true;
}

bool init()
{
    if (!initSDL())
    {
        return false;
    }

    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
    {
        spdlog::error("Failed to initialize GLAD");
        return false;
    }

    glad_glViewport(0, 0, width, height);

    initImgui();

    return true;
}

#pragma endregion

//====================================================================================//
//====================================================================================//
//====================================================================================//

void drawImgui()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();

    //===================================================//
    //===================================================//
    //===================================================//

    ImGui::Begin("Kontrol Paneli");
    ImGui::SliderFloat("R", &r, 0, 1.0f);
    ImGui::SliderFloat("G", &g, 0, 1.0f);
    ImGui::SliderFloat("B", &b, 0, 1.0f);    

    if (ImGui::Button("R##0"))
    {
        pozisyon = glm::vec3(0);
    }

    ImGui::SliderFloat("pozisyon.x", &pozisyon.x, -10, 10);
    ImGui::SliderFloat("pozisyon.y", &pozisyon.y, -10, 10);
    ImGui::SliderFloat("pozisyon.z", &pozisyon.z, -10, 10);

    if (ImGui::Button("R##1"))
    {
        olcek = glm::vec3(1);
    }

    ImGui::SliderFloat("olcek.x", &olcek.x, -10, 10);
    ImGui::SliderFloat("olcek.y", &olcek.y, -10, 10);
    ImGui::SliderFloat("olcek.z", &olcek.z, -10, 10);

    if (ImGui::Button("R##2"))
    {
        alfa = glm::vec3(0);
    }

    ImGui::SliderFloat("alfa.x", &alfa.x, -360.0f, 360.0f);
    ImGui::SliderFloat("alfa.y", &alfa.y, -360.0f, 360.0f);
    ImGui::SliderFloat("alfa.z", &alfa.z, -360.0f, 360.0f);


    ImGui::End();

    //===================================================//
    //===================================================//
    //===================================================//
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void draw()
{
    glad_glClearColor(r, g, b, 1.0f);
    glad_glClear(GL_COLOR_BUFFER_BIT);

    glad_glUseProgram(shader.m_programId);
    glad_glBindVertexArray(VAO);

    glad_glDrawArrays(GL_TRIANGLES, 0, 3);        
}

void update(float dt)
{

}

void inputs(float dt)
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

//====================================================================================//
//====================================================================================//
//====================================================================================//

int main()
{   

    if (!init())
    {
        return -1;
    }

    using Clock = std::chrono::steady_clock;
    using Duration = std::chrono::duration<float>;
    using TimePoint = std::chrono::steady_clock::time_point;

    TimePoint previousTime = Clock::now();

    shader.createProgram();
    shader.compile(GL_VERTEX_SHADER, "vertexShader.glsl");
    shader.compile(GL_FRAGMENT_SHADER, "fragShader.glsl");
    shader.link();


    shader.addUniform(u_transform);  

    float vertices[] =
    {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f
    };

    //vao vbo olustur
    glad_glGenVertexArrays(1, &VAO);
    glad_glGenBuffers(1, &VBO);

    //vao => vbo bagla
    glad_glBindVertexArray(VAO);

    //vbo ya ne oldugu soyle 
    glad_glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //vbo ya veriyi yolla
    glad_glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //nokta verisinin bellekte nasil tutuldugunu bildir
    glad_glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    //vertex attrib sifiri aktif hale getir

    //vertexShader icindeki location = 0 konumuna x,y,z pozisyon bilgileri buray gidicek diyoruz 
    //layout (location = 0) in vec3 aPos;
    glad_glEnableVertexAttribArray(0);

    //vbo baglantisini kes
    glad_glBindBuffer(GL_ARRAY_BUFFER, 0);
    //vao baglantisini kes
    glad_glBindVertexArray(0);
 
   

    while (f_running)
    {
        //------------Delta time--------------------//
        TimePoint currentTime = Clock::now();

        Duration deltaTime = currentTime - previousTime;

        previousTime = currentTime;

        dt = deltaTime.count();
        FPS = 1.0f / dt;
        //------------------------------------------//

        inputs(dt);
        update(dt);

        draw();
        drawImgui();

        SDL_GL_SwapWindow(window);
    }
}