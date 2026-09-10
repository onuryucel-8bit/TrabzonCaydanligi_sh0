#include "Application.h"

Application::Application()
    :gp(m_context),
    light(m_lightDirection)
{   
    olcek.x = 1;
    olcek.y = 1;
    olcek.z = 1;

    pozisyon.x = 0;
    pozisyon.y = 0;
    pozisyon.z = 0;

    m_lightDirection.z = -5.0f;

    
    loadObjModel("ucgenZpozitif.obj");
            
    m_renderMod = RenderMod::RenderMod_Triangle_Filled;

    
    gp.loadTexture("dama", "dama.png");
    gp.loadTexture("cizgi", "cizgi.png");
    gp.loadTexture("tombili","tombili.jpg");    
    gp.loadTexture("colorstone","colorstone.png");
    gp.loadTexture("container","container.jpg");
    gp.loadTexture("graystone","graystone.png");
    gp.loadTexture("redbrick","redbrick.png");
    gp.loadTexture("wall","wall.jpg");
    gp.loadTexture("wood", "wood.png");
 
    m_currentTextureId = "dama";
    currentTexture = 0;

    gp.useTexture(m_currentTextureId);

}

Application::~Application()
{
}

void Application::run()
{   
    initSDL();
    initImgui();   

    using Clock = std::chrono::steady_clock;
    using Duration = std::chrono::duration<float>;
    using TimePoint = std::chrono::steady_clock::time_point;

    TimePoint previousTime = Clock::now();

    while (f_running)
    {
        ZoneScoped;

        //------------Delta time--------------------//
        TimePoint currentTime = Clock::now();

        Duration deltaTime = currentTime - previousTime;

        previousTime = currentTime;

        dt = deltaTime.count();
        FPS = 1.0f / dt;
        //------------------------------------------//

        inputs();

        update(dt);

        SDL_RenderClear(m_context.renderer);

        draw();
        drawImgui();

        {
            //tampon bellekleri degistir
            ZoneScopedN("SDL_RenderPresent");
            SDL_RenderPresent(m_context.renderer);
        }
        FrameMark;
    }
}

void Application::initSDL()
{

#ifdef DEBUG_FULLSCREEN
    m_context.window = SDL_CreateWindow("TrabzonCaydanligi", m_context.fizikselGenislik, m_context.fizikselYukseklik, SDL_WINDOW_FULLSCREEN);
#else
    m_context.window = SDL_CreateWindow("TrabzonCaydanligi", m_context.fizikselGenislik, m_context.fizikselYukseklik, SDL_WINDOW_RESIZABLE);
#endif // DEBUG_FULLSCREEN


    if (m_context.window == nullptr)
    {
        std::cout << "HATA:: Pencere olusturulamadi\n";
        f_running = false;
    }

    m_context.renderer = SDL_CreateRenderer(m_context.window, NULL);

    if (m_context.renderer == nullptr)
    {
        std::cout << "HATA:: Renderer olusturulamadi\n";
        f_running = false;
    }

    createTexture();
    
}

void Application::initImgui()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
   
    // Enable Docking
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    ImGui_ImplSDL3_InitForSDLRenderer(m_context.window, m_context.renderer);
    ImGui_ImplSDLRenderer3_Init(m_context.renderer);
}

//=============================================================================//
//=============================================================================//
//=============================================================================//

void Application::update(float dt)
{            
    ZoneScopedN(__FUNCTION__);

    izdusumUcgenleri.clear();
    transformedVertices.clear();

    //ekranin ortasi
    float cx = m_context.fizikselGenislik / 2.0f;
    float cy = m_context.fizikselYukseklik / 2.0f;

    Matrix4x4 S  =  Matrix4x4::makeScaleMatrix(olcek);
    Matrix4x4 Rx =  Matrix4x4::makeRotateXMatrix(alfa.x);
    Matrix4x4 Ry =  Matrix4x4::makeRotateYMatrix(alfa.y);
    Matrix4x4 Rz =  Matrix4x4::makeRotateZMatrix(alfa.z);
    Matrix4x4 T  =  Matrix4x4::makeTranslationMatrix(pozisyon);
    
    Matrix4x4 worldMatrix = T * Rz * Ry * Rx * S;

    // Loop over shapes
    for (size_t s = 0; s < m_shapes.size(); s++)
    {
        // Loop over faces(polygon)
        size_t index_offset = 0;
        for (size_t f = 0; f < m_shapes[s].mesh.num_face_vertices.size(); f++)
        {
            size_t fv = size_t(m_shapes[s].mesh.num_face_vertices[f]);

            // Loop over vertices in the face.
            for (size_t v = 0; v < fv; v++)
            {
                // access to vertex
                tinyobj::index_t idx = m_shapes[s].mesh.indices[index_offset + v];

                tinyobj::real_t vx = m_vertexAttributes.vertices[3 * size_t(idx.vertex_index) + 0];
                tinyobj::real_t vy = m_vertexAttributes.vertices[3 * size_t(idx.vertex_index) + 1];
                tinyobj::real_t vz = m_vertexAttributes.vertices[3 * size_t(idx.vertex_index) + 2];

                Vector3 pos(vx, vy, vz);

                // Check if `normal_index` is zero or positive. negative = no normal data
                /*if (idx.normal_index >= 0)
                {
                    tinyobj::real_t nx = m_vertexAttributes.normals[3 * size_t(idx.normal_index) + 0];
                    tinyobj::real_t ny = m_vertexAttributes.normals[3 * size_t(idx.normal_index) + 1];
                    tinyobj::real_t nz = m_vertexAttributes.normals[3 * size_t(idx.normal_index) + 2];
                }*/

                Vector2 texcoords;

                // Check if `texcoord_index` is zero or positive. negative = no texcoord data
                if (idx.texcoord_index >= 0)
                {
                    tinyobj::real_t tx = m_vertexAttributes.texcoords[2 * size_t(idx.texcoord_index) + 0];
                    tinyobj::real_t ty = m_vertexAttributes.texcoords[2 * size_t(idx.texcoord_index) + 1];

                    texcoords.x = tx;
                    texcoords.y = ty;
                }
                
                // Optional: vertex colors
                // tinyobj::real_t red   = attrib.colors[3*size_t(idx.vertex_index)+0];
                // tinyobj::real_t green = attrib.colors[3*size_t(idx.vertex_index)+1];
                // tinyobj::real_t blue  = attrib.colors[3*size_t(idx.vertex_index)+2];

                //m_vertices.push_back({ pos, texcoords });



                //oclek-tasima-dondurme islemlerini yap
                Vector4 nokta = pos.toVec4();
                nokta = worldMatrix * nokta;

                //vec4 => vec3
                Vector3 vertexWorldPos = nokta.toVec3();

                //noktalari kameradan .z kadar uzaklastir
                vertexWorldPos.z -= m_camera.m_position.z;

                //noktalari kaydet
                transformedVertices.push_back({ vertexWorldPos, texcoords });

            }
            index_offset += fv;

            // per-face material
            //m_shapes[s].mesh.material_ids[f];
        }
    }
     
    for (size_t i = 0; i < transformedVertices.size(); i += 3)
    {
        Vector3 vectorA = transformedVertices[i + 0].pos;
        Vector3 vectorB = transformedVertices[i + 1].pos;
        Vector3 vectorC = transformedVertices[i + 2].pos;

        ////---Arka yuz eleme------//
        Vector3 vectorAB = vectorB - vectorA;
        Vector3 vectorAC = vectorC - vectorA;

        Vector3 normal = vectorAB.cross(vectorAC);
        normal.normalize();
                      
        if (m_cullmode == CullMod::ACTIVE)
        {            
            Vector3 cameraRay = m_camera.m_position - vectorA;

            float dotNormalCamera = normal.dot(cameraRay);
        
            if (dotNormalCamera <= m_cullValue)
            {
                continue;
            }
        }
        //------------------------//

        Triangle projectedTrig;

        projectedTrig.texcoords[0] = transformedVertices[i + 0].texCoords;
        projectedTrig.texcoords[1] = transformedVertices[i + 1].texCoords;
        projectedTrig.texcoords[2] = transformedVertices[i + 2].texCoords;        

        Vector3 verts[3] = { vectorA, vectorB, vectorC };

        //-----Izdusum--------//
        for (size_t j = 0; j < 3; j++)
        {
            //ekrana yansit
            Vector2 projectedPoint = m_camera.project(verts[j]);

            //ekran koordinati  3 boyutta ise y+ yukari bundan dolayi -projectPoint.y yaziyoruz
            // --->x+
            // |
            // |
            // v
            // y+
            // 
            // y ters cevir boylece yukarsi y + asagi y - olur
            projectedPoint.y = -projectedPoint.y;
            projectedPoint.x += cx;
            projectedPoint.y += cy;



            projectedTrig.points[j] = projectedPoint;
        }
                       
        //-----------------------------------------------//
        //-----------------------------------------------//
        //--------isik-----------

        light.m_direction = m_lightDirection;
        
        light.m_direction.normalize();

        float intensityFactor = normal.dot(light.m_direction);
        
        if (m_lightMod == LightMod::FLAT)
        {
            projectedTrig.color = light.applyLighting(m_color, intensityFactor);
        }
        else if (m_lightMod == LightMod::NONE)
        {
            projectedTrig.color = 0xff00'ff00;
        }

        //-----------------------------------------------//
        //-----------------------------------------------//


        //-----------------------------------------------//
        //-----------------------------------------------//
        //---Derinlik---/
        if (m_depthTest == DepthTest::PAINTER_AVERAGE)
        { 
            projectedTrig.depthTestValue = (vectorA.z + vectorB.z + vectorC.z) / 3;
        }
        else if (m_depthTest == DepthTest::PAINTER_DISTANCE)
        {
            Vector3 trigCenter;
            trigCenter.x = (vectorA.x + vectorB.x + vectorC.x) / 3;
            trigCenter.y = (vectorA.y + vectorB.y + vectorC.y) / 3;
            trigCenter.z = (vectorA.z + vectorB.z + vectorC.z) / 3;

            Vector3 distance;

            distance = trigCenter - m_camera.m_position;

            projectedTrig.depthTestValue = distance.x * distance.x + distance.y * distance.y + distance.z * distance.z;
        }
        //-----------------------------------------------//
        //-----------------------------------------------//

        izdusumUcgenleri.emplace_back(projectedTrig);
    }             


    //derinlik icin siralama yap
    if (m_depthTest == DepthTest::PAINTER_AVERAGE ||
        m_depthTest == DepthTest::PAINTER_DISTANCE)
    {
        std::sort(izdusumUcgenleri.begin(), izdusumUcgenleri.end(),
            [](const Triangle& a, const Triangle& b)
            {
                return a.depthTestValue > b.depthTestValue;
            }
        );
    }

}

void Application::inputs()
{
    ZoneScopedN(__FUNCTION__);

    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        ImGui_ImplSDL3_ProcessEvent(&event);
       
        switch (event.key.key)
        {
        case SDLK_ESCAPE:
            f_running = false;
            break;        
        }

        switch (event.type)
        {
        case SDL_EVENT_WINDOW_RESIZED:
            applyWindowSize();
            break;

        case SDL_EVENT_QUIT:
            f_running = false;
            break;

        }
    }
}

void Application::draw()
{
    ZoneScopedN(__FUNCTION__);

    gp.clearColorBuffer(Color::BLACK);
    //------------------------------//    
    
    for (size_t i = 0; i < izdusumUcgenleri.size(); i++)
    {
        Triangle trig = izdusumUcgenleri[i];

        if ((m_renderMod & RenderMod::RenderMod_Vertex) == RenderMod::RenderMod_Vertex)
        {            
            gp.drawFilledRectangle(trig.points[0].x, trig.points[0].y, 5, 5, Color::RED);
            gp.drawFilledRectangle(trig.points[1].x, trig.points[1].y, 5, 5, Color::RED);
            gp.drawFilledRectangle(trig.points[2].x, trig.points[2].y, 5, 5, Color::RED);
        }

        if ((m_renderMod & RenderMod::RenderMod_Textured) == RenderMod::RenderMod_Textured)
        {           
            gp.drawTexturedTriangle_Barycentric(
                trig.points[0].x   , trig.points[0].y,
                trig.texcoords[0].x, trig.texcoords[0].y,

                trig.points[1].x, trig.points[1].y,
                trig.texcoords[1].x, trig.texcoords[1].y,

                trig.points[2].x, trig.points[2].y,
                trig.texcoords[2].x, trig.texcoords[2].y              
            );
        }

        if ((m_renderMod & RenderMod::RenderMod_Triangle_Filled) == RenderMod::RenderMod_Triangle_Filled)
        {           
            gp.drawFilledTriangle(
                trig.points[0].x, trig.points[0].y,
                trig.points[1].x, trig.points[1].y,
                trig.points[2].x, trig.points[2].y,
                trig.color
            );
        }

        if ((m_renderMod & RenderMod::RenderMod_Triangle) == RenderMod::RenderMod_Triangle)
        {
            gp.drawTriangle(
                trig.points[0].x, trig.points[0].y,
                trig.points[1].x, trig.points[1].y,
                trig.points[2].x, trig.points[2].y,
                Color::GREEN
            );
        }
        
    }

    if (f_arkaPlan)
    {
        gp.drawLine(0, m_context.fizikselYukseklik / 2, m_context.fizikselGenislik, m_context.fizikselYukseklik / 2, Color::GREEN);

        gp.drawLine(m_context.fizikselGenislik / 2, 0, m_context.fizikselGenislik / 2, m_context.fizikselYukseklik, Color::GREEN);

        gp.drawDots(Color::GREEN);
    }    

    //------------------------------//
    gp.drawColorBuffer();
}

void Application::drawImgui()
{
    ZoneScopedN(__FUNCTION__);

    SDL_SetRenderLogicalPresentation(
        m_context.renderer,
        0,
        0,
        SDL_LOGICAL_PRESENTATION_DISABLED
    );


    ImGui_ImplSDLRenderer3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();
    //===================================================//
    //===================================================//
    //===================================================//
    ImGui::Begin("Isik");

    static float color[4] = { 0.0f, 1.0f, 0.0f, 1.0f };

    ImGui::ColorEdit4("Color", color);
    ImGui::ColorPicker4("Color Picker", color);

    uint8_t r = (uint8_t)(color[0] * 255);
    uint8_t g = (uint8_t)(color[1] * 255);
    uint8_t b = (uint8_t)(color[2] * 255);
    uint8_t a = (uint8_t)(color[3] * 255);

    m_color = (0xff << 24) | (r << 16) | (g << 8) | b;

    ImGui::RadioButton("Isik kapat", (int*)&m_lightMod, (int)(LightMod::NONE));
    ImGui::RadioButton("Duz isik", (int*)&m_lightMod, (int)(LightMod::FLAT));

    ImGui::Text("Isik renk 0x%08X", m_color);

    ImGui::SliderFloat("isik.x", &m_lightDirection.x, -10, 10);
    ImGui::SliderFloat("isik.y", &m_lightDirection.y, -10, 10);
    ImGui::SliderFloat("isik.z", &m_lightDirection.z, -10, 10);

    ImGui::End();
    //===================================================//
    //===================================================//   
    //===================================================//    

#pragma region Ucak Motoru

    ImGui::Begin("Ucak Motoru");

    ImGui::Text("FPS %f", FPS);   
    ImGui::Text("Gorunen Ucgen Adeti %d", izdusumUcgenleri.size());

    ImGui::SliderInt("Graphics::m_TEST_tex_y", &Graphics::m_TEST_tex_y, 0, 15);

    ImGui::RadioButton("DDA algoritmasi", (int*)&Graphics::m_lineAlgoType, (int)(LineAlgoType::DDA));
    ImGui::RadioButton("Brensham algoritmasi", (int*)&Graphics::m_lineAlgoType, (int)(LineAlgoType::Brensham));

    ImGui::RadioButton("Derinlik kapat", (int*)&m_depthTest, (int)(DepthTest::NONE));
    ImGui::RadioButton("Derinlik ressam ortalama", (int*)&m_depthTest, (int)(DepthTest::PAINTER_AVERAGE));
    ImGui::RadioButton("Derinlik ressam uzaklik", (int*)&m_depthTest, (int)(DepthTest::PAINTER_DISTANCE));
    

    ImGui::RadioButton("Perspektif", (int*)&m_camera.m_projectMod, (int)(ProjectMod::Perspective));
    ImGui::RadioButton("Ortho", (int*)&m_camera.m_projectMod, (int)(ProjectMod::Ortho));

    ImGui::Text("pencere fiziksel  (Genislik %d, Yukseklik %d)", m_context.fizikselGenislik, m_context.fizikselYukseklik);
    ImGui::Text("pencere mantiksal (Genislik %d, Yukseklik %d)", m_context.mantiksalGenislik, m_context.mantiksalYukseklik);

    ImGui::Text("m_renderMod %d", m_renderMod);

    static bool showTriangles = false;

    if (ImGui::Checkbox("Ucgenler", &showTriangles))
    {
        if (showTriangles)
        {
            m_renderMod |= RenderMod::RenderMod_Triangle;
        }
        else
        {
            m_renderMod &= ~RenderMod::RenderMod_Triangle;
        }
    }

    static bool showFilledTrig = true;

    if (ImGui::Checkbox("Dolu ucgen", &showFilledTrig))
    {
        if (showFilledTrig)
        {
            m_renderMod |= RenderMod::RenderMod_Triangle_Filled;
        }
        else
        {
            m_renderMod &= ~RenderMod::RenderMod_Triangle_Filled;
        }
    }

    static bool showVertex = false;

    if (ImGui::Checkbox("Noktalar", &showVertex))
    {
        if (showVertex)
        {
            m_renderMod |= RenderMod::RenderMod_Vertex;
        }
        else
        {
            m_renderMod &= ~RenderMod::RenderMod_Vertex;
        }
    }

    static bool showTexture = false;

    if (ImGui::Checkbox("Kaplama", &showTexture))
    {
        if (showTexture)
        {
            m_renderMod |= RenderMod::RenderMod_Textured;
        }
        else
        {
            m_renderMod &= ~RenderMod::RenderMod_Textured;
        }
    }

    ImGui::Checkbox("f_arkaPlan", &f_arkaPlan);

    ImGui::End();
#pragma endregion

    //===================================================//
    //===================================================//
    //===================================================//


    ImGui::Begin("Gazete Kagiti");

    if (ImGui::Combo("Kaplamalar", &currentTexture, textureid, IM_ARRAYSIZE(textureid)))
    {
        if (currentTexture != lastTexture)
        {
            m_currentTextureId = textureid[currentTexture];

            gp.useTexture(m_currentTextureId);

            lastTexture = currentTexture;
        }
    }
    
    ImGui::End();

    //===================================================//
    //===================================================//
    //===================================================//

#pragma region KontrolPaneli

    ImGui::Begin("Kontrol Paneli");

    if (ImGui::Combo("Modeller", &currentModel, models, IM_ARRAYSIZE(models)))
    {
        if (currentModel != lastModel)
        {
            loadObjModel(models[currentModel]);
            lastModel = currentModel;
        }
    }
    
    

    ImGui::NewLine();

    if (ImGui::Button("R##0"))
    {
        pozisyon.x = 0;
        pozisyon.y = 0;
        pozisyon.z = 0;
    }
    ImGui::SliderFloat("pozisyon.x", &pozisyon.x, -10, 10);
    ImGui::SliderFloat("pozisyon.y", &pozisyon.y, -10, 10);
    ImGui::SliderFloat("pozisyon.z", &pozisyon.z, -10, 10);

    if (ImGui::Button("R##1"))
    {
        olcek.x = 1;
        olcek.y = 1;
        olcek.z = 1;
    }
    ImGui::SliderFloat("olcek.x", &olcek.x, -10, 10);
    ImGui::SliderFloat("olcek.y", &olcek.y, -10, 10);
    ImGui::SliderFloat("olcek.z", &olcek.z, -10, 10);

    if (ImGui::Button("R##2"))
    {
        alfa.x = 0;
        alfa.y = 0;
        alfa.z = 0;
    }
    ImGui::SliderFloat("alfa.x", &alfa.x, 0, degToRad(360));
    ImGui::SliderFloat("alfa.y", &alfa.y, 0, degToRad(360));
    ImGui::SliderFloat("alfa.z", &alfa.z, 0, degToRad(360));
    

    ImGui::RadioButton("AC: Arka yuz eleme", (int*)&m_cullmode, (int)(CullMod::ACTIVE));
    ImGui::RadioButton("KAPAT: Arka yuz eleme", (int*)&m_cullmode, (int)(CullMod::NONE));
    
    ImGui::SliderFloat("m_cullValue", &m_cullValue, -10.0f, 10.0f);
    
    ImGui::End();

#pragma endregion

    //===================================================//
    //===================================================//
    //===================================================//
    ImGui::Render();
    ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), m_context.renderer);

    SDL_SetRenderLogicalPresentation(
        m_context.renderer,
        m_context.mantiksalGenislik,
        m_context.mantiksalYukseklik,
        SDL_LOGICAL_PRESENTATION_STRETCH
    );
}

//=============================================================================//
//=============================================================================//
//=============================================================================//

void Application::applyWindowSize()
{    
    SDL_GetWindowSize(m_context.window, &m_context.fizikselGenislik, &m_context.fizikselYukseklik);

    //p_context->mantiksalGenislik = p_context->fizikselGenislik;
    //p_context->mantiksalYukseklik = p_context->fizikselYukseklik;

    SDL_SetRenderLogicalPresentation(
        m_context.renderer,
        m_context.mantiksalGenislik,
        m_context.mantiksalYukseklik,
        SDL_LOGICAL_PRESENTATION_STRETCH
    );

    createTexture();
}

void Application::createTexture()
{
    SDL_DestroyTexture(m_context.canvas);
    delete[] m_context.colorBuffer;

    m_context.colorBuffer = new Color_t[m_context.fizikselGenislik * m_context.fizikselYukseklik];

    //RAM'de tuttuðumuz renk deðerlerini(colorBuffer) ekrana çizmek için kaplama(texture) oluþturuyoruz
    m_context.canvas = SDL_CreateTexture(m_context.renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, m_context.fizikselGenislik, m_context.fizikselYukseklik);

    if (m_context.canvas == nullptr)
    {
        std::cout << "Error:: Texture initializing failed\n";
        f_running = false;
    }

    //Kaplamanin piksel gorunume sahip olmasi icin
    SDL_SetTextureScaleMode(m_context.canvas, SDL_SCALEMODE_NEAREST);
}

void Application::loadObjModel(std::string model)
{
    std::cout << "============================\n"
              << "Model yukleniyor\n";

    model = cmake_PROJECT_MODELLER + model;
        
    std::vector<tinyobj::material_t> materials;

    std::string err;

    bool ret = tinyobj::LoadObj(&m_vertexAttributes, &m_shapes, &materials, &err, model.c_str(), cmake_PROJECT_MODELLER);

    if (!err.empty())
    {
        std::cout << err << "\n";
    }

    std::cout << "m_shapes.size():" << m_shapes.size() << "\n"
              << "m_vertexAttributes.vertices.size()" << m_vertexAttributes.vertices.size() << "\n";  
}