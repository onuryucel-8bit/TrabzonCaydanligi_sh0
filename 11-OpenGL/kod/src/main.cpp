#include <iostream>
#include <numbers>
#include <vector>
#include <string>

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"


GLFWwindow* window;
ImGuiIO* io;

void initImgui()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

     io = &ImGui::GetIO();

    // Enable Docking
    io->ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io->FontGlobalScale = 1;

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
}

bool init()
{           
    if (!glfwInit())
    {
        std::cout << "ERROR:: Failed to init GLFW\n";
        return false;
    }

    window = glfwCreateWindow(800, 600, "Trabzon Caydanligi", nullptr, nullptr);

    glfwMakeContextCurrent(window);

    if (!window)
    {
        std::cout << "ERROR:: Failed to create window\n";
        return false;
    }

    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "ERROR:: Failed to init GLAD\n";
        return false;
    }


    glfwMakeContextCurrent(window);

    return true;
}

void update()
{        
    
}

void drawImgui()
{
    //ImGui_ImplSDLRenderer3_NewFrame();
    //ImGui_ImplSDL3_NewFrame();
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();

    ImGui::NewFrame();
    
    ImGui::Begin("Kontrol Paneli");
    
    

    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    //ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), rcontext.renderer);
}

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

void keyInputs(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}

int main()
{        
    if (!init())
    {
        return -1;
    }
    initImgui();

    glfwSetKeyCallback(window, keyInputs);
    
    while (!glfwWindowShouldClose(window))
    {                
        glfwPollEvents();

        //------------------------------//

        glad_glClear(GL_COLOR_BUFFER_BIT);

        draw();
        drawImgui();

        glfwSwapBuffers(window);

        //------------------------------//
    }
}