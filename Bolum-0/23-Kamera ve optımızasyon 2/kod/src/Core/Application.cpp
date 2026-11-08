#include "Application.h"

Application::Application()
    :gp(m_context),
    m_light(m_lightDirection),
    m_camera(m_fov, m_znear, m_zfar, (float)m_context.mantiksalGenislik / m_context.mantiksalYukseklik)
{   
          
    m_lightDirection = glm::vec3(0, 0, -5.0f);

    loadModel("sponza.obj");
    //loadModel("ucgenZpozitif.obj");
            
    m_renderMod = RenderMod::RenderMod_Triangle_Filled;

    
    /*gp.loadTexture("dama", "dama.png");
    gp.loadTexture("cizgi", "cizgi.png");
    gp.loadTexture("tombili","tombili.jpg");    
    gp.loadTexture("colorstone","colorstone.png");
    gp.loadTexture("container","container.jpg");
    gp.loadTexture("graystone","graystone.png");
    gp.loadTexture("redbrick","redbrick.png");
    gp.loadTexture("wall","wall.jpg");
    gp.loadTexture("wood", "wood.png");*/
 
    m_currentTextureId = "dama";
    currentTexture = 0;

    //gp.useTexture(m_currentTextureId);

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

        inputs(dt);

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

inline void Application::update(float dt)
{
    ZoneScopedN(__FUNCTION__);
   
    glm::mat4 projeksiyonMatrisi;
    {
        ZoneScopedN("T * R * S nokta => dunya islemi");

        izdusumUcgenleri.clear();
        transformedVertices.clear();

        //kamera
        //=============================================//
        glm::vec3 target(0.0f, 0.0f, 1.0f);
        glm::vec3 upDirection(0.0f, 1.0f, 0.0f);
        glm::mat4x4 cameraRot_yaw = glm::rotate(glm::mat4x4(1.0f), m_camera.m_yav, glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4x4 cameraRot_pitch = glm::rotate(cameraRot_yaw, m_camera.m_pitch, glm::vec3(1.0f, 0.0f, 0.0f));
        
        m_camera.m_direction = glm::vec3(cameraRot_pitch * glm::vec4(target, 0.0f));

        target = m_camera.m_position + m_camera.m_direction;
        glm::mat4 viewMatrix = glm::lookAt(m_camera.m_position, target, upDirection);
        //=============================================//

        //projeksiyon
        //=============================================//
        projeksiyonMatrisi = glm::perspective(m_fov, m_aspect, m_znear, m_zfar);
        //=============================================//

        for (MeshMatrix& meshMatrix : m_meshMatrixList)
        {
            //#pragma omp simd
            Mesh& mesh = m_meshList[meshMatrix.m_index];
            glm::mat4x4 modelView = viewMatrix * meshMatrix.getWorldMatrix();

            //yuzleri tara
            for (size_t i = 0; i + 2 < mesh.vertices.size(); i += 3)
            {
                //ucgen noktalarini kenara koy
                glm::vec4 verts[3];
                verts[0] = modelView * mesh.vertices[i + 0].pos;
                verts[1] = modelView * mesh.vertices[i + 1].pos;
                verts[2] = modelView * mesh.vertices[i + 2].pos;

                ////arka yuz eleme
                ////vec4 => vec3
                //glm::vec3 vectorA = glm::vec3(verts[0]);
                //glm::vec3 vectorB = glm::vec3(verts[1]);
                //glm::vec3 vectorC = glm::vec3(verts[2]);

                //////---Arka yuz eleme------//
                //glm::vec3 vectorAB = vectorB - vectorA;
                //glm::vec3 vectorAC = vectorC - vectorA;

                //glm::vec3 normal = glm::cross(vectorAB, vectorAC);
                //normal = glm::normalize(normal);

                //ucgen olustur
                Triangle projectedTrig;
                projectedTrig.texcoords[0] = mesh.vertices[i + 0].uv;
                projectedTrig.texcoords[1] = mesh.vertices[i + 1].uv;
                projectedTrig.texcoords[2] = mesh.vertices[i + 2].uv;


                bool f_clipContinue = false;

                for (size_t j = 0; j < 3; j++)
                {
                    //ekrana yansit
                    glm::vec4 projectedPoint = projeksiyonMatrisi * verts[j];

                    //kirp
                    float a = projectedPoint.w - m_znear;

                    if (a >= 0 && 
                        projectedPoint.x >= -projectedPoint.w && projectedPoint.x <= projectedPoint.w &&
                        projectedPoint.y >= -projectedPoint.w && projectedPoint.y <= projectedPoint.w &&
                        projectedPoint.z >= -projectedPoint.w && projectedPoint.z <= projectedPoint.w)
                    {                     
                        //icerde
                    }
                    else
                    {
                        //ucgen disarda
                        f_clipContinue = true;
                        break;
                    }


                    if (projectedPoint.w != 0.0f)
                    {
                        projectedPoint.x /= projectedPoint.w;
                        projectedPoint.y /= projectedPoint.w;
                        projectedPoint.z /= projectedPoint.w;
                    }

                    projectedPoint.y = -projectedPoint.y;

                    projectedPoint.x *= m_context.fizikselGenislik / 2;
                    projectedPoint.y *= m_context.fizikselYukseklik / 2;

                    projectedPoint.x += m_context.fizikselGenislik / 2;
                    projectedPoint.y += m_context.fizikselYukseklik / 2;

                    projectedTrig.points[j] = projectedPoint;
                }

                if (f_clipContinue)
                {
                    continue;
                }

                m_light.m_direction = m_lightDirection;

                m_light.m_direction = glm::normalize(m_light.m_direction);

                glm::vec3 ortDikVektor =
                    (mesh.vertices[i + 0].normal
                    + mesh.vertices[i + 1].normal
                    + mesh.vertices[i + 2].normal );

                ortDikVektor /= 3;

                ortDikVektor = glm::normalize(ortDikVektor);

                float intensityFactor = glm::dot(ortDikVektor, m_light.m_direction);

                if (m_lightMod == LightMod::FLAT)
                {
                    projectedTrig.color = m_light.applyLighting(m_color, intensityFactor);
                }
                else if (m_lightMod == LightMod::NONE)
                {
                    projectedTrig.color = 0xff00'ff00;
                }

                izdusumUcgenleri.emplace_back(projectedTrig);
            }
        }

    }
}

inline void Application::inputs(float dt)
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

        if (event.type == SDL_EVENT_KEY_DOWN)
        {
            float cameraSpeed = 40.0f;

            switch (event.key.key)
            {
            case SDLK_E:
                m_objId++;

                if (m_objId >= m_meshMatrixList.size())
                {
                    m_objId = 0;
                }
                break;

            case SDLK_Q:
                m_objId--;

                if (m_objId <= 0)
                {
                    m_objId = m_meshMatrixList.size() - 1;
                }
                break;

            case SDLK_UP:
                m_camera.m_position.y += dt * cameraSpeed;
                break;
            case SDLK_DOWN:
                m_camera.m_position.y -= dt * cameraSpeed;
                break;

            case SDLK_C:
                m_camera.m_pitch -= dt * 0.5f;
                break;

            case SDLK_V:
                m_camera.m_pitch += dt * 0.5f;
                break;

            case SDLK_A:
                m_camera.m_yav -= dt * 0.5f;                
                break;

            case SDLK_D:
                m_camera.m_yav += dt * 0.5f;
                break;

            case SDLK_W:
                m_camera.m_forwardVel = m_camera.m_direction * 20.0f * dt;
                m_camera.m_position = m_camera.m_position + m_camera.m_forwardVel;
                break;

            case SDLK_S:
                m_camera.m_forwardVel = m_camera.m_direction * 20.0f * dt;
                m_camera.m_position = m_camera.m_position - m_camera.m_forwardVel;
                break;

            }
        }

        switch (event.type)
        {
        case SDL_EVENT_WINDOW_RESIZED:
            //TODO z-tamponu icin kapatildi
            //applyWindowSize();
            break;

        case SDL_EVENT_QUIT:
            f_running = false;
            break;

        }
    }
}

inline void Application::draw()
{
    ZoneScopedN(__FUNCTION__);

    clearColorBuffer(Color::BLACK);
    clearZbuffer();
    //------------------------------//    
    
    for (size_t i = 0; i < izdusumUcgenleri.size(); i++)
    {
        Triangle trig = izdusumUcgenleri[i];

       /* if ((m_renderMod & RenderMod::RenderMod_Vertex) == RenderMod::RenderMod_Vertex)
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

        if ((m_renderMod & RenderMod::RenderMod_PerspectiveCorrect) == RenderMod::RenderMod_PerspectiveCorrect)
        {
            gp.drawTexturedTriangle_Barycentric_PerspectiveCorrect(
                trig.points[0].x, trig.points[0].y, trig.points[0].z, trig.points[0].w,
                trig.texcoords[0].x, trig.texcoords[0].y,

                trig.points[1].x, trig.points[1].y, trig.points[1].z, trig.points[1].w,
                trig.texcoords[1].x, trig.texcoords[1].y,

                trig.points[2].x, trig.points[2].y, trig.points[2].z, trig.points[2].w,
                trig.texcoords[2].x, trig.texcoords[2].y
            );
        }*/

        if ((m_renderMod & RenderMod::RenderMod_Triangle_Filled) == RenderMod::RenderMod_Triangle_Filled)
        {                       
            drawTriangle_zbuffer(
                trig.points[0],
                trig.points[1],
                trig.points[2],                
                trig.color
            );
        }
        /*
        if ((m_renderMod & RenderMod::RenderMod_Triangle) == RenderMod::RenderMod_Triangle)
        {
            gp.drawTriangle(
                trig.points[0].x, trig.points[0].y,
                trig.points[1].x, trig.points[1].y,
                trig.points[2].x, trig.points[2].y,
                trig.color
            );
        }*/
        
    }

    if (f_arkaPlan)
    {
        bresenhamLineAlgo(0, m_context.fizikselYukseklik / 2, m_context.fizikselGenislik, m_context.fizikselYukseklik / 2, Color::GREEN);

        bresenhamLineAlgo(m_context.fizikselGenislik / 2, 0, m_context.fizikselGenislik / 2, m_context.fizikselYukseklik, Color::GREEN);

        //drawDots(Color::GREEN);
    }    

    //------------------------------//
    drawColorBuffer();
}

inline void Application::drawImgui()
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

    static bool persCorrect_showTexture = false;

    if (ImGui::Checkbox("Perpektif Kaplama", &persCorrect_showTexture))
    {
        if (persCorrect_showTexture)
        {
            m_renderMod |= RenderMod::RenderMod_PerspectiveCorrect;
        }
        else
        {
            m_renderMod &= ~RenderMod::RenderMod_PerspectiveCorrect;
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

            //gp.useTexture(m_currentTextureId);

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
            loadModel(models[currentModel]);
            lastModel = currentModel;
        }
    }
    
    

    ImGui::NewLine();

    if (ImGui::Button("R##0"))
    {
        m_meshMatrixList[m_objId].m_pos = glm::vec3(0);
    }

    ImGui::SliderFloat("pozisyon.x", &m_meshMatrixList[m_objId].m_pos.x, -10, 10);
    ImGui::SliderFloat("pozisyon.y", &m_meshMatrixList[m_objId].m_pos.y, -10, 10);
    ImGui::SliderFloat("pozisyon.z", &m_meshMatrixList[m_objId].m_pos.z, -10, 10);

    if (ImGui::Button("R##1"))
    {
        m_meshMatrixList[m_objId].m_scale = glm::vec3(1);
    }

    ImGui::SliderFloat("olcek.x", &m_meshMatrixList[m_objId].m_scale.x, -10, 10);
    ImGui::SliderFloat("olcek.y", &m_meshMatrixList[m_objId].m_scale.y, -10, 10);
    ImGui::SliderFloat("olcek.z", &m_meshMatrixList[m_objId].m_scale.z, -10, 10);

    if (ImGui::Button("R##2"))
    {
        m_meshMatrixList[m_objId].m_rot = glm::vec3(0);
    }

    ImGui::SliderFloat("alfa.x", &m_meshMatrixList[m_objId].m_rot.x, -360.0f, 360.0f);
    ImGui::SliderFloat("alfa.y", &m_meshMatrixList[m_objId].m_rot.y, -360.0f, 360.0f);
    ImGui::SliderFloat("alfa.z", &m_meshMatrixList[m_objId].m_rot.z, -360.0f, 360.0f);
    

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
    delete[] m_context.zbuffer; 

    m_context.colorBuffer = new Color_t[m_context.fizikselGenislik * m_context.fizikselYukseklik];
    m_context.zbuffer = new float[m_context.fizikselGenislik * m_context.fizikselYukseklik];


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

void Application::loadModel(std::string model)
{
    model = cmake_PROJECT_MODELLER + model;

    std::vector<Mesh> obj = loadObj(model);

    for (Mesh& mesh : obj)
    {
        m_meshList.push_back(mesh);

        MeshMatrix meshMatrix;
        meshMatrix.m_index = m_meshList.size() - 1;

        m_meshMatrixList.push_back(meshMatrix);        
    }
}

//=============================================================================//
//=============================================================================//
//=============================================================================//

inline void Application::drawColorBuffer()
{
    //load colorbuffer
    SDL_UpdateTexture(m_context.canvas, NULL, m_context.colorBuffer, (int)(m_context.fizikselGenislik * sizeof(Color_t)));

    //render canvas
    SDL_RenderTexture(m_context.renderer, m_context.canvas, NULL, NULL);
}

inline void Application::clearZbuffer()
{
    const size_t totalPixels = static_cast<size_t>(m_context.fizikselGenislik) * m_context.fizikselYukseklik;
    std::fill(m_context.zbuffer, m_context.zbuffer + totalPixels, 1.0f);
}

inline void Application::clearColorBuffer(Color_t color)
{
    const size_t totalPixels = static_cast<size_t>(m_context.fizikselGenislik) * m_context.fizikselYukseklik;
    std::fill(m_context.colorBuffer, m_context.colorBuffer + totalPixels, color);
}

inline void Application::bresenhamLineAlgo(int x0, int y0, int x1, int y1, Color_t color)
{
    ZoneScopedN(__FUNCTION__);

    int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
    int dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
    int err = dx + dy, e2; /* error value e_xy */

    for (;;) {  /* loop */
        drawPixel(x0, y0, color);
        if (x0 == x1 && y0 == y1) break;
        e2 = 2 * err;
        if (e2 >= dy) { err += dy; x0 += sx; } /* e_xy+e_x > 0 */
        if (e2 <= dx) { err += dx; y0 += sy; } /* e_xy+e_y < 0 */
    }
}

inline void Application::drawPixel(int x, int y, Color_t color)
{
    ZoneScopedN(__FUNCTION__);

    if (x >= 0 && x < m_context.fizikselGenislik && y >= 0 && y < m_context.fizikselYukseklik)
    {
        m_context.colorBuffer[y * m_context.fizikselGenislik + x] = color;
    }
}

inline void Application::drawPixel_zbuffer(int x, int y, Vector4 a, Vector4 b, Vector4 c, Color_t color)
{
    ZoneScopedN(__FUNCTION__);

    if (x < 0 || x >= m_context.fizikselGenislik ||
        y < 0 || y >= m_context.fizikselYukseklik)
    {
        return;
    }

    glm::vec2 p(x, y);

    glm::vec3 weight = barycentricWeights(
        glm::vec2(a.x, a.y),
        glm::vec2(b.x, b.y),
        glm::vec2(c.x, c.y),
        p);

    float alfa = weight.x;
    float beta = weight.y;
    float gamma = weight.z;

    float interpolated_w = (1 / a.w) * alfa + (1 / b.w) * beta + (1 / c.w) * gamma;

    interpolated_w = 1.0f - interpolated_w;

    if (interpolated_w < m_context.zbuffer[y * m_context.fizikselGenislik + x])
    {
        drawPixel(x, y, color);

        m_context.zbuffer[y * m_context.fizikselGenislik + x] = interpolated_w;
    }
}

inline glm::vec3 Application::barycentricWeights(glm::vec2 a, glm::vec2 b, glm::vec2 c, glm::vec2 p)
{
    glm::vec2 ac = c - a;
    glm::vec2 ab = b - a;
    glm::vec2 pc = c - p;
    glm::vec2 pb = b - p;
    glm::vec2 ap = p - a;

    //z ekrana dik 
    float areaABC = ac.x * ab.y - ac.y * ab.x;  // ||AC x AB ||

    /*
              || PC x PB ||
      alfa = -------------
              || AC x AB ||
    */
    float alfa = (pc.x * pb.y - pc.y * pb.x) / areaABC;

    /*
              || AC x AP ||
       beta = -------------
              || AC x AB ||
    */
    float beta = (ac.x * ap.y - ac.y * ap.x) / areaABC;

    /*
        alfa + beta + gamma = 1

        gamma = 1 - alfa - beta
    */

    float gamma = 1.0f - alfa - beta;

    glm::vec3 weight{ alfa, beta, gamma };

    return weight;

}

inline void Application::drawTriangle_zbuffer(glm::ivec4 v0, glm::ivec4 v1, glm::ivec4 v2, Color_t color)
{
    ZoneScopedN(__FUNCTION__);

    //y0 < y1 < y2
    if (v0.y > v1.y)
    {
        std::swap(v0, v1);
    }
    if (v1.y > v2.y)
    {
        std::swap(v1, v2);
    }
    if (v0.y > v1.y)
    {
        std::swap(v0, v1);
    }

    //====================================================//

    float invSlopeLeft = 0;
    float invSlopeRight = 0;

    if (v1.y - v0.y != 0)
    {
        invSlopeLeft = (float)(v1.x - v0.x) / abs(v1.y - v0.y);
    }
    if (v2.y - v0.y != 0)
    {
        invSlopeRight = (float)(v2.x - v0.x) / abs(v2.y - v0.y);
    }

    float ac_x = (float)(v2.x - v0.x);
    float ac_y = (float)(v2.y - v0.y);
    float ab_x = (float)(v1.x - v0.x);
    float ab_y = (float)(v1.y - v0.y);
    
    float invW0 = (1.0f / v0.w);
    float invW1 = (1.0f / v1.w);
    float invW2 = (1.0f / v2.w);

    //z ekrana dik ||AC x AB ||
    float areaABC = ac_x * ab_y - ac_y * ab_x;
    areaABC = 1.0f / areaABC;

    //bacaklar
    for (int y = v0.y; y < v1.y; y++)
    {
        ZoneScopedN("for (int y = v0.y; y < v1.y; y++)");
        //y = ax + b dogrusal denklem
        float startx = (y - v1.y) * invSlopeLeft + v1.x;
        float endx = (y - v0.y) * invSlopeRight + v0.x;

        if (endx < startx)
        {
            std::swap(startx, endx);
        }

        Color_t* colorRow = &m_context.colorBuffer[y * m_context.fizikselGenislik];
        float* zRow = &m_context.zbuffer[y * m_context.fizikselGenislik];

        //ara bolge
        for (int x = startx; x < endx; x++)
        {
            ZoneScopedN("for (int x = startx; x < endx; x++)");

            if (x < 0 || x >= m_context.fizikselGenislik ||
                y < 0 || y >= m_context.fizikselYukseklik)
            {
                continue;
            }

            //-----------------------------------------------------//                                     
            float pc_x = v2.x - x;
            float pc_y = v2.y - y;
            float pb_x = v1.x - x;
            float pb_y = v1.y - y;
            float ap_x = x - v0.x;
            float ap_y = y - v0.y;
            
            /*
                      || PC x PB ||
              alfa = -------------
                      || AC x AB ||
            */
            float alfa = (pc_x * pb_y - pc_y * pb_x) * areaABC;

            /*
                      || AC x AP ||
               beta = -------------
                      || AC x AB ||
            */
            float beta = (ac_x * ap_y - ac_y * ap_x) * areaABC;

            /*
                alfa + beta + gamma = 1

                gamma = 1 - alfa - beta
            */

            float interpolated_w = invW0 * alfa + invW1 * beta + invW2 * (1.0f - alfa - beta);

            interpolated_w = 1.0f - interpolated_w;            
            //-----------------------------------------------------//

            if (interpolated_w < zRow[x])
            {
                //yukarda sinir kontrolu yapilmakta
                colorRow[x] = color;
                zRow[x] = interpolated_w;
            }
        }
    }

    //-------------------------------Alt--------------------------------//
    invSlopeLeft = 0;
    invSlopeRight = 0;

    if (v2.y - v1.y != 0)
    {
        invSlopeLeft = (float)(v2.x - v1.x) / abs(v2.y - v1.y);
    }
    if (v2.y - v0.y != 0)
    {
        invSlopeRight = (float)(v2.x - v0.x) / abs(v2.y - v0.y);
    }

    for (int y = v1.y; y < v2.y; y++)
    {
        float startx = (y - v1.y) * invSlopeLeft + v1.x;
        float endx = (y - v0.y) * invSlopeRight + v0.x;

        if (endx < startx)
        {
            std::swap(startx, endx);
        }

        Color_t* colorRow = &m_context.colorBuffer[y * m_context.fizikselGenislik];
        float* zRow = &m_context.zbuffer[y * m_context.fizikselGenislik];

        for (int x = startx; x < endx; x++)
        {
            if (x <= 0 || x > m_context.fizikselGenislik ||
                y <= 0 || y > m_context.fizikselYukseklik)
            {
                continue;
            }

            //-----------------------------------------------------//                                     
            float pc_x = v2.x - x;
            float pc_y = v2.y - y;
            float pb_x = v1.x - x;
            float pb_y = v1.y - y;
            float ap_x = x - v0.x;
            float ap_y = y - v0.y;

            /*
                      || PC x PB ||
              alfa = -------------
                      || AC x AB ||
            */
            float alfa = (pc_x * pb_y - pc_y * pb_x) * areaABC;

            /*
                      || AC x AP ||
               beta = -------------
                      || AC x AB ||
            */
            float beta = (ac_x * ap_y - ac_y * ap_x) * areaABC;

            /*
                alfa + beta + gamma = 1

                gamma = 1 - alfa - beta
            */
            
            float interpolated_w = invW0 * alfa + invW1 * beta + invW2 * (1.0f - alfa - beta);

            interpolated_w = 1.0f - interpolated_w;            
            //-----------------------------------------------------//

            if (interpolated_w < zRow[x])
            {
                //yukarda sinir kontrolu yapilmakta
                colorRow[x] = color;
                zRow[x] = interpolated_w;
            }
        }
    }
}

//=============================================================================//
//=============================================================================//
//=============================================================================//


std::vector<Mesh> Application::loadObj(std::string model)
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


