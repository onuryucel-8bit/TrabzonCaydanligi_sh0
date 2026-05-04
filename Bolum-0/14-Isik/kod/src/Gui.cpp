#include "Gui.h"

Gui::Gui(SDL_Window* window, SDL_Renderer* renderer, AppState* appState, std::function<void(std::string)> objLoader)
    :
    m_renderer(*renderer),
    m_appState(*appState),
    m_loadObjModel(objLoader)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    io = &ImGui::GetIO();

    // Enable Docking
    io->ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io->FontGlobalScale = 1;

    ImGui_ImplSDL3_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer3_Init(renderer);
}


void Gui::draw()
{
    ImGui_ImplSDLRenderer3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();
    //------------------------------------------------------------------//
    //------------------------------------------------------------------//
    //------------------------------------------------------------------//
    ImGui::Begin("Isik");

    ImGui::RadioButton("Isik Kapat", (int*)&m_appState.m_lightMod, (int)LightMod::NONE);
    ImGui::RadioButton("Duz Isik", (int*)&m_appState.m_lightMod, (int)LightMod::FLAT);

    if (ImGui::Button("R##3"))
    {
        m_appState.m_lightPos.x = 0;
        m_appState.m_lightPos.y = 0;
        m_appState.m_lightPos.z = 0;
    }
    ImGui::SliderFloat("pozisyon.x", &m_appState.m_lightPos.x, -10, 10);
    ImGui::SliderFloat("pozisyon.y", &m_appState.m_lightPos.y, -10, 10);
    ImGui::SliderFloat("pozisyon.z", &m_appState.m_lightPos.z, -10, 10);

    ImGui::End();
    //------------------------------------------------------------------//
    //------------------------------------------------------------------//
    //------------------------------------------------------------------//

    ImGui::Begin("Kamera");

    ImGui::RadioButton("Perspektif", (int*)&m_appState.m_projectMod, (int)ProjectMod::Perspective);
    ImGui::RadioButton("Ortografik", (int*)&m_appState.m_projectMod, (int)ProjectMod::Ortho);

   // ImGui::Text("Ekran Boyutu (%i, %i)", screenWidth, screenHeight);

    ImGui::NewLine();
    //ImGui::Text("Fare fx,fy (%f, %f)", mouseX, mouseY);

    ImGui::SliderFloat("FOV", &m_appState.camera.FOV_factor, 0, 600);

    ImGui::End();
    //------------------------------------------------------------------//
    //------------------------------------------------------------------//
    //------------------------------------------------------------------//

    ImGui::Begin("Kontrol Paneli");

    if (ImGui::Combo("Modeller", &currentModel, models, IM_ARRAYSIZE(models)))
    {
        if (currentModel != lastModel)
        {
            m_loadObjModel(models[currentModel]);
            lastModel = currentModel;
        }
    }

    ImGui::SliderFloat("Arka Yuz Eleme Degeri", &m_appState.m_cullValue, -5, 5);

    static bool cullModecb = true;

    if (ImGui::Checkbox("Arka Yuz Eleme", &cullModecb))
    {
        if (cullModecb)
        {
            m_appState.m_cullMod = CullMod::ACTIVE;
        }
        else
        {
            m_appState.m_cullMod = CullMod::NONE;
        }
    }

    static bool showFilledTriangles = false;
    static bool showTriangles = true;

    if (ImGui::Checkbox("Dolu Ucgen", &showFilledTriangles))
    {
        if (showFilledTriangles)
        {
            m_appState.m_renderMod &= 0x0;
            showTriangles = false;

            m_appState.m_renderMod |= RenderMod::RenderMode_Triangle_Filled;
        }
        else
        {
            m_appState.m_renderMod &= ~RenderMod::RenderMode_Triangle_Filled;
        }
    }



    if (ImGui::Checkbox("Ucgenler", &showTriangles))
    {
        if (showTriangles)
        {
            m_appState.m_renderMod &= 0x0;
            showFilledTriangles = false;
            m_appState.m_renderMod |= RenderMod::RenderMode_Triangle;
        }
        else
        {
            m_appState.m_renderMod &= ~RenderMod::RenderMode_Triangle;
        }
    }

    static bool showVertex = false;

    if (ImGui::Checkbox("Noktalar", &showVertex))
    {
        if (showVertex)
        {
            m_appState.m_renderMod |= RenderMod::RenderMode_Vertex;
        }
        else
        {
            m_appState.m_renderMod &= ~RenderMod::RenderMode_Vertex;
        }
    }

    if (ImGui::Button("R##0"))
    {
        m_appState.m_position.x = 0;
        m_appState.m_position.y = 0;
        m_appState.m_position.z = 0;
    }
    ImGui::SliderFloat("pozisyon.x", &m_appState.m_position.x, -20, 20);
    ImGui::SliderFloat("pozisyon.y", &m_appState.m_position.y, -20, 20);
    ImGui::SliderFloat("pozisyon.z", &m_appState.m_position.z, -20, 20);

    if (ImGui::Button("R##1"))
    {
        m_appState.m_scale.x = 1.0f;
        m_appState.m_scale.y = 1.0f;
        m_appState.m_scale.z = 1.0f;
    }
    ImGui::SliderFloat("boyut.x", &m_appState.m_scale.x, -2, 5);
    ImGui::SliderFloat("boyut.y", &m_appState.m_scale.y, -2, 5);
    ImGui::SliderFloat("boyut.z", &m_appState.m_scale.z, -2, 5);

    if (ImGui::Button("R##2"))
    {
        m_appState.m_rotate.x = 0;
        m_appState.m_rotate.y = 0;
        m_appState.m_rotate.z = 0;
    }
    ImGui::SliderFloat("dondurme.x", &m_appState.m_rotate.x, -4, 4);
    ImGui::SliderFloat("dondurme.y", &m_appState.m_rotate.y, -4, 4);
    ImGui::SliderFloat("dondurme.z", &m_appState.m_rotate.z, -4, 4);

    ImGui::End();

    ImGui::Render();
    ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), &m_renderer);
}

Gui::~Gui()
{
}