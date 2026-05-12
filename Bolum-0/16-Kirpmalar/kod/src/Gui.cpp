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

    ImGui::SliderFloat("FOVy", &m_appState.m_camera.FOVy, 0, std::numbers::pi * 2);
    ImGui::SliderFloat("pozisyon.x", &m_appState.m_camera.eye.x, -20, 20);
    ImGui::SliderFloat("pozisyon.y", &m_appState.m_camera.eye.y, -20, 20);
    ImGui::SliderFloat("pozisyon.z", &m_appState.m_camera.eye.z, -20, 20);

    ImGui::SliderFloat("hedef.x", &m_appState.m_camera.target.x, -20, 20);
    ImGui::SliderFloat("hedef.y", &m_appState.m_camera.target.y, -20, 20);
    ImGui::SliderFloat("hedef.z", &m_appState.m_camera.target.z, -20, 20);

    ImGui::SliderFloat("up.x", &m_appState.m_camera.up.x, -20, 20);
    ImGui::SliderFloat("up.y", &m_appState.m_camera.up.y, -20, 20);
    ImGui::SliderFloat("up.z", &m_appState.m_camera.up.z, -20, 20);

    ImGui::SliderFloat("zfar", &m_appState.m_camera.zfar, 60, 100);
    ImGui::SliderFloat("znear", &m_appState.m_camera.znear, 1, 50);
    
    if (ImGui::BeginTable("Noktalar", 4, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg))
    {
        ImGui::TableSetupColumn("X");
        ImGui::TableSetupColumn("Y");
        ImGui::TableSetupColumn("Z");
        ImGui::TableSetupColumn("W");
        ImGui::TableHeadersRow();

        for (size_t i = 0; i < 4; i++)
        {
            ImGui::TableNextRow();
            for (size_t j = 0; j < 4; j++)
            {
                ImGui::TableSetColumnIndex(j);
                ImGui::Text("%.3f", m_appState.m_viewMatrix.mat[i][j]);
            }            
        }

        ImGui::EndTable();
    }

    const char* planeNames[] =
    {
        "Left",
        "Right",
        "Top",
        "Bottom",
        "Near",
        "Far"
    };

    if (ImGui::BeginTable("Frustum", 7, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg))
    {
        ImGui::TableSetupColumn("plane");
        ImGui::TableSetupColumn("point.x");
        ImGui::TableSetupColumn("point.y");
        ImGui::TableSetupColumn("point.z");
        ImGui::TableSetupColumn("normal.x");
        ImGui::TableSetupColumn("normal.y");
        ImGui::TableSetupColumn("normal.z");
        ImGui::TableHeadersRow();

        for (size_t i = 0; i < 6; i++)
        {
            ImGui::TableNextRow();
            //for (size_t j = 0; j < 4; j++)
            {

                ImGui::TableSetColumnIndex(0);
                ImGui::Text("%s", planeNames[i]);

                ImGui::TableSetColumnIndex(1);
                ImGui::Text("%.3f", m_appState.m_camera.frustumPlanes[i].point.x);
                ImGui::TableSetColumnIndex(2);
                ImGui::Text("%.3f", m_appState.m_camera.frustumPlanes[i].point.y);
                ImGui::TableSetColumnIndex(3);
                ImGui::Text("%.3f", m_appState.m_camera.frustumPlanes[i].point.z);

                ImGui::TableSetColumnIndex(4);
                ImGui::Text("%.3f", m_appState.m_camera.frustumPlanes[i].normal.x);
                ImGui::TableSetColumnIndex(5);
                ImGui::Text("%.3f", m_appState.m_camera.frustumPlanes[i].normal.y);
                ImGui::TableSetColumnIndex(6);
                ImGui::Text("%.3f", m_appState.m_camera.frustumPlanes[i].normal.z);
            }
        }

        ImGui::EndTable();
    }

    ImGui::Text("vec A %f, %f, %f", m_appState.veca.x, m_appState.veca.y, m_appState.veca.z);
    ImGui::Text("vec B %f, %f, %f", m_appState.vecb.x, m_appState.vecb.y, m_appState.vecb.z);
    ImGui::Text("vec C %f, %f, %f", m_appState.vecc.x, m_appState.vecc.y, m_appState.vecc.z);

    ImGui::SliderInt("testvar", &m_appState.testVar, 0, 12);

    ImGui::End();
    //------------------------------------------------------------------//
    //------------------------------------------------------------------//
    //------------------------------------------------------------------//

    ImGui::Begin("Kontrol Paneli");

    ImGui::Text("FPS %f", m_appState.m_FPS);
    ImGui::Text("Cizilen Ucgen Sayisi %i", m_appState.m_renderTrigSize);

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

    ImGui::RadioButton("Derinlik.Ressam.kapali", (int*)&m_appState.m_depthTest, (int)DepthTest::NONE);
    ImGui::RadioButton("Derinlik.Ressam.Ortalama", (int*)&m_appState.m_depthTest, (int)DepthTest::PAINTER_AVERAGE);
    ImGui::RadioButton("Derinlik.Ressam.Uzaklik", (int*)&m_appState.m_depthTest, (int)DepthTest::PAINTER_DISTANCE);

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