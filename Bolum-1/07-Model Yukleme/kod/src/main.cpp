#include "pch.h"
#include "makroVeTanimlar/TracyLib.h"
#include "Shader/Shader.h"
#include "Pencere/Window.h"

#include "buffer/VertexBufferObject.h"
#include "buffer/VertexArrayObject.h"

#define u_transform "u_transform"

Window window;
glm::vec3 alfa = glm::vec3(0);
glm::vec3 olcek = glm::vec3(0.3f);
glm::vec3 pozisyon = glm::vec3(0, 0, -0.2f);

glm::mat4 projection;
float FOV = glm::radians(45.0f);
float znear = 0.1f;
float zfar = 100.0f;

Shader shader;
GLuint VBO, VAO;

//Pencere
//====================================================================================//

bool f_running = true;

float FPS = 0;
float dt = 0;
//====================================================================================//

float r = 0.1f;
float g = 0.1f;
float b = 0.1f;

VertexArrayObject vao;
VertexBufferObject vbo;

//====================================================================================//
//====================================================================================//
//====================================================================================//

std::vector<glm::vec3> modelNoktalari;


ImGui::FileBrowser fileDialog;

std::vector<std::string> modeller = 
{ 
    "apple.obj",
    "bottle_02.obj",    
};

int currentItemIndex = 0;

void loadObj(std::string model, bool fromFile = false)
{
    if (!fromFile)
    {
        model = cmake_PROJECT_MODELLER + model;
    }

    spdlog::info("Model yukleniyor = {}", model);

    tinyobj::attrib_t attrib;

    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;

    std::string err;

    bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, model.c_str(), cmake_PROJECT_MODELLER);

    if (!err.empty())
    {
        spdlog::debug("{}", err);
        //return std::vector<Mesh>();
    }

    if (ret == true)
    {
        spdlog::debug("{} ret => true", __FUNCTION__);
    }


    modelNoktalari.clear();
    //dosya icindeki butun parcalari gez
    for (const auto& shape : shapes)
    {
        //Mesh mesh;
        //mesh.vertices.reserve(shape.mesh.indices.size());
        modelNoktalari.reserve(shape.mesh.indices.size());

        //indeks ile yuklenen nokta dizisinden pozisyon ve kaplama koordinatlarini al
        for (const tinyobj::index_t& idx : shape.mesh.indices)
        {
            glm::vec3 vertex;

            //pozisyonu al
            vertex =
            {
                attrib.vertices[3 * idx.vertex_index + 0],
                attrib.vertices[3 * idx.vertex_index + 1],
                attrib.vertices[3 * idx.vertex_index + 2]               
            };

            //dik vektor varmi?
            if (idx.normal_index >= 0)
            {
               /* vertex.normal =
                {
                    attrib.normals[3 * idx.normal_index + 0],
                    attrib.normals[3 * idx.normal_index + 1],
                    attrib.normals[3 * idx.normal_index + 2]
                };*/
            }

            //kaplama varmi?
            if (idx.texcoord_index >= 0)
            {
                /*int textureIndex = idx.texcoord_index;

                vertex.uv =
                {
                    attrib.texcoords[2 * textureIndex + 0],
                    attrib.texcoords[2 * textureIndex + 1],
                };*/
            }

            //noktalari yukle
            modelNoktalari.push_back(vertex);
        }

        //parcayi yukle
        //retMeshList.push_back(mesh);
    }

    //gonder
    //return retMeshList;
}

void updateVBO()
{
    glad_glBindBuffer(GL_ARRAY_BUFFER, vbo.m_id);

    glad_glBufferData(
        GL_ARRAY_BUFFER,
        modelNoktalari.size() * sizeof(glm::vec3),
        modelNoktalari.data(),
        GL_STATIC_DRAW
    );

    glad_glBindBuffer(GL_ARRAY_BUFFER, 0);
}

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

    ImGui_ImplSDL3_InitForOpenGL(window.m_window, window.m_glContext);
    ImGui_ImplOpenGL3_Init("#version 330 core");
}

bool init()
{
    if (!window.init())
    {
        return false;
    }

    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
    {
        spdlog::error("Failed to initialize GLAD");
        return false;
    }

    glad_glViewport(0, 0, window.width, window.height);

    initImgui();

    //z tamponunu ac
    glad_glEnable(GL_DEPTH_TEST);
    //arka yuz elemeyi ac
    glad_glEnable(GL_CULL_FACE);
    glad_glCullFace(GL_BACK);
    glad_glFrontFace(GL_CCW);

    return true;
}

void initBuffer()
{
    vao.genBuffer();
    vbo.genBuffer();

    vao.bind();
    //boyut, adres
    vbo.loadData(sizeof(glm::vec3) * modelNoktalari.size(), modelNoktalari.data());
    //loc, veri miktari, dizinin uzunlugu
    vao.loadAttrib(0, 3, sizeof(glm::vec3));

    vbo.unbind();
    vao.unbind();
}
#pragma endregion

//====================================================================================//
//====================================================================================//
//====================================================================================//

#pragma region ISLEM

void updateProjection(int width, int height)
{
    if (height == 0)
    {
        return;
    }

    float aspect = static_cast<float>(width) / static_cast<float>(height);

    projection = glm::perspective(FOV, aspect, znear, zfar);
}

void drawImgui()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();

    //===================================================//
    //===================================================//
    //===================================================//
    ImGui::Begin("Ucak Motoru");

    ImGui::Text("FPS %f", FPS);

    ImGui::End();
    //===================================================//
    //===================================================//
    //===================================================//
    
    
    ImGui::Begin("Kontrol Paneli");
    if (ImGui::Button("Bos Ucgen"))
    {
        glad_glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }

    if (ImGui::Button("Dolu Ucgen"))
    {
        glad_glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    if (ImGui::Button("Z tamponu aktif"))
    {
        //z tamponunu ac
        glad_glEnable(GL_DEPTH_TEST);
    }
    if (ImGui::Button("Z tamponu kapat"))
    {
        glad_glDisable(GL_DEPTH_TEST);
    }
    if (ImGui::Button("Arka Yuz eleme ac"))
    {
        glad_glEnable(GL_CULL_FACE);
        glad_glCullFace(GL_BACK);
        glad_glFrontFace(GL_CCW);
    }
    if (ImGui::Button("Arka Yuz eleme kapat"))
    {
        glad_glDisable(GL_CULL_FACE);
    }

    if (ImGui::Button("Model sec"))
    {
        fileDialog.Open();
    }

    fileDialog.Display();

    if (fileDialog.HasSelected())
    {
        loadObj(fileDialog.GetSelected().string(), true);
        updateVBO();
        fileDialog.ClearSelected();
    }

    //model bulamadiysa bos yazdir
    const char* preview = modeller.empty()
        ? "HATA: Model Yukleme"
        : modeller[currentItemIndex].c_str();

    if (ImGui::BeginCombo("##combo", preview))
    {        
        for (int n = 0; n < static_cast<int>(modeller.size()); n++)
        {
            
            bool is_selected = (currentItemIndex == n);

            //secilen objenin indeksini al
            if (ImGui::Selectable(modeller[n].c_str(), is_selected))
            {
                currentItemIndex = n;
                loadObj(modeller[currentItemIndex]);
                updateVBO();
            }

            //modellerden hangisi secildiyse ona odaklan
            if (is_selected)
            {
                ImGui::SetItemDefaultFocus();
            }
        }

        ImGui::EndCombo();
    }
    
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
    glad_glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glad_glUseProgram(shader.m_programId);
    glad_glBindVertexArray(vao.m_id);

    glad_glDrawArrays(GL_TRIANGLES, 0, modelNoktalari.size());
}

void inputs(float dt)
{
    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        ImGui_ImplSDL3_ProcessEvent(&event);

        switch (event.type)
        {
        case SDL_EVENT_QUIT:
            f_running = false;
            break;

        case SDL_EVENT_WINDOW_RESIZED:
        {
            int width = event.window.data1;
            int height = event.window.data2;

            glad_glViewport(0, 0, width, height);
            updateProjection(width, height);
            break;
        }
        }

        switch (event.key.key)
        {
        case SDLK_ESCAPE:
            f_running = false;
            break;
        }
    }
}

void update(float dt)
{
    //------------------------------------------//
    //T * Rz * Ry * Rx * S

    //tasima
    glm::mat4x4 model = glm::translate(glm::mat4x4(1.0f), pozisyon);

    //dondurme
    model = glm::rotate(model, glm::radians(alfa.z), glm::vec3(0, 0, 1));
    model = glm::rotate(model, glm::radians(alfa.y), glm::vec3(0, 1, 0));
    model = glm::rotate(model, glm::radians(alfa.x), glm::vec3(1, 0, 0));

    //olcekleme
    model = glm::scale(model, olcek);

    //glm::mat4 MVP = projeksiyon * view * model;
    glm::mat4 MVP = projection * model;

    shader.setUniform(u_transform, MVP);
    //------------------------------------------//
}

#pragma endregion

//====================================================================================//
//====================================================================================//
//====================================================================================//


int main()
{   

    if (!init())
    {
        return -1;
    }

    shader.createProgram();
    shader.compile(GL_VERTEX_SHADER, "vertexShader.glsl");
    shader.compile(GL_FRAGMENT_SHADER, "fragShader.glsl");
    shader.link();

    shader.addUniform(u_transform);
      
    loadObj("apple.obj");
    
    initBuffer();
                    
    projection = glm::perspective(glm::radians(45.0f), static_cast<float>(window.width) / static_cast<float>(window.height), znear, zfar);


    using Clock = std::chrono::steady_clock;
    using Duration = std::chrono::duration<float>;
    using TimePoint = std::chrono::steady_clock::time_point;

    TimePoint previousTime = Clock::now();

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

        SDL_GL_SwapWindow(window.m_window);
    }
}