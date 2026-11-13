#include "pch.h"
#include "makroVeTanimlar/TracyLib.h"
#include "Shader.h"
#include "Mesh.h"
#include "MeshMatrix.h"

#define u_transform "u_transform"

Shader shader;
GLFWwindow* window = nullptr;
float dt;
float FPS;
int width = 800;
int height = 600;
GLuint VBO, VAO;

float r = 0.1f;
float g = 0.1f;
float b = 0.1f;
float a = 1.0f;

glm::vec3 alfa = glm::vec3(0);
glm::vec3 olcek = glm::vec3(0.3f);
glm::vec3 pozisyon = glm::vec3(0, 0, -5.0f);

std::vector<glm::vec3> modelNoktalari;


std::vector<Mesh> meshList;
std::vector<MeshMatrix> meshMatrixList;

std::vector<Mesh> loadObj(std::string model)
{
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


    std::vector<Mesh> retMeshList;
    //dosya icindeki butun parcalari gez
    for (const auto& shape : shapes)
    {
        Mesh mesh;
        mesh.vertices.reserve(shape.mesh.indices.size());

        //indeks ile yuklenen nokta dizisinden pozisyon ve kaplama koordinatlarini al
        for (const tinyobj::index_t& idx : shape.mesh.indices)
        {
            Vertex vertex;

            //pozisyonu al
            vertex.pos =
            {
                attrib.vertices[3 * idx.vertex_index + 0],
                attrib.vertices[3 * idx.vertex_index + 1],
                attrib.vertices[3 * idx.vertex_index + 2],
                1.0f
            };

            //dik vektor varmi?
            if (idx.normal_index >= 0)
            {
                vertex.normal =
                {
                    attrib.normals[3 * idx.normal_index + 0],
                    attrib.normals[3 * idx.normal_index + 1],
                    attrib.normals[3 * idx.normal_index + 2]
                };
            }

            //kaplama varmi?
            if (idx.texcoord_index >= 0)
            {
                int textureIndex = idx.texcoord_index;

                vertex.uv =
                {
                    attrib.texcoords[2 * textureIndex + 0],
                    attrib.texcoords[2 * textureIndex + 1],
                };
            }

            //noktalari yukle
            mesh.vertices.push_back(vertex);
        }

        //parcayi yukle
        retMeshList.push_back(mesh);
    }

    //gonder
    return retMeshList;
}

void loadModel(std::string model)
{
    model = cmake_PROJECT_MODELLER + model;

    std::vector<Mesh> obj = loadObj(model);

    for (Mesh& mesh : obj)
    {
        meshList.push_back(mesh);

        MeshMatrix meshMatrix;
        meshMatrix.m_index = meshList.size() - 1;

        meshMatrixList.push_back(meshMatrix);
    }
}

void updateCubeVBO()
{
    glad_glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glad_glBufferData(
        GL_ARRAY_BUFFER,
        modelNoktalari.size() * sizeof(glm::vec3),
        modelNoktalari.data(),
        GL_STATIC_DRAW
    );

    glad_glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glad_glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}

void initImgui()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    // Enable Docking
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    
    ImGui_ImplGlfw_InitForOpenGL(window, true);    
    ImGui_ImplOpenGL3_Init("#version 330");
}

bool initGLFW()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //saydam arka plan ekle
    glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);

    window = glfwCreateWindow(width, height, "Trabzon Caydanligi", NULL, NULL);

    if (window == NULL)
    {
        spdlog::error("Failed to create GLFW window");
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(window);

    return true;
}

void drawImgui()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();

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

    
    ImGui::SliderFloat("R", &r, 0, 1.0f);
    ImGui::SliderFloat("G", &g, 0, 1.0f);
    ImGui::SliderFloat("B", &b, 0, 1.0f);
    ImGui::SliderFloat("A", &a, 0, 1.0f);
    
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
    glad_glClearColor(r, g, b, a);
    glad_glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glad_glUseProgram(shader.m_programId);
    glad_glBindVertexArray(VAO);

    glad_glDrawArrays(GL_TRIANGLES, 0, 36);
}

int main()
{   

#pragma region Init_

    if (!initGLFW())
    {
        return -1;
    }

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        spdlog::error("Failed to initialize GLAD");
        return -1;
    }

    glad_glViewport(0, 0, width, height);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    initImgui();

#pragma endregion

    shader.createProgram();
    shader.compile(GL_VERTEX_SHADER, "vertexShader.glsl");
    shader.compile(GL_FRAGMENT_SHADER, "fragShader.glsl");
    shader.link();


    shader.addUniform(u_transform);
    

    float vertices[] = 
    {
        // Back face
        -1,-1,-1,   -1, 1,-1,    1, 1,-1,
        -1,-1,-1,    1, 1,-1,    1,-1,-1,

        // Front face
        -1,-1, 1,    1,-1, 1,    1, 1, 1,
        -1,-1, 1,    1, 1, 1,   -1, 1, 1,

        // Left face
        -1,-1,-1,   -1,-1, 1,   -1, 1, 1,
        -1,-1,-1,   -1, 1, 1,   -1, 1,-1,

        // Right face
         1,-1,-1,    1, 1,-1,    1, 1, 1,
         1,-1,-1,    1, 1, 1,    1,-1, 1,

         // Bottom face
         -1,-1,-1,    1,-1,-1,    1,-1, 1,
         -1,-1,-1,    1,-1, 1,   -1,-1, 1,

         // Top face
         -1, 1,-1,   -1, 1, 1,    1, 1, 1,
         -1, 1,-1,    1, 1, 1,    1, 1,-1
    };   

    glad_glGenVertexArrays(1, &VAO);
    glad_glGenBuffers(1, &VBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glad_glBindVertexArray(VAO);

    glad_glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glad_glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glad_glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glad_glEnableVertexAttribArray(0);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glad_glBindBuffer(GL_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glad_glBindVertexArray(0);
 
    //z tamponunu ac
    glad_glEnable(GL_DEPTH_TEST);
    //arka yuz elemeyi ac
    glad_glEnable(GL_CULL_FACE);
    glad_glCullFace(GL_BACK);
    glad_glFrontFace(GL_CCW);
   
    glm::mat4 projection;

    float znear = 0.1f;
    float zfar = 100.0f;
    projection = glm::perspective(glm::radians(45.0f), static_cast<float>(width) / static_cast<float>(height), znear, zfar);


    using Clock = std::chrono::steady_clock;
    using Duration = std::chrono::duration<float>;
    using TimePoint = std::chrono::steady_clock::time_point;

    TimePoint previousTime = Clock::now();

    while (!glfwWindowShouldClose(window))
    {        
        //------------Delta time--------------------//
        TimePoint currentTime = Clock::now();

        Duration deltaTime = currentTime - previousTime;

        previousTime = currentTime;

        dt = deltaTime.count();
        FPS = 1.0f / dt;
        //------------------------------------------//

        processInput(window);
        
        
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

        draw();
        drawImgui();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}