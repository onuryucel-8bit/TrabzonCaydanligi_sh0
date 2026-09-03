#pragma once

#include <cstdint>

#include "Camera.h"

enum class ProjectMod
{
    Ortho,
    Perspective
};

enum RenderMod : uint8_t
{
    RenderMode_Vertex = 1 << 0,
    RenderMode_Triangle = 1 << 1,
    RenderMode_Triangle_Filled = 1 << 2
};

enum class CullMode
{
    NONE,
    ACTIVE
};

struct AppState
{
    Camera camera;

    //----------------------------------------------------//
    ProjectMod m_projectMod = ProjectMod::Perspective;
    uint8_t m_renderMod = RenderMod::RenderMode_Triangle;
    CullMode m_cullMod = CullMode::ACTIVE;
    //----------------------------------------------------//

    float m_cullValue = 0;
    
    //window
    float m_mouseX = 0;
    float m_mouseY = 0;
    int m_screenWidth = 0;
    int m_screenHeight = 0;

    Vector3 m_position{ 0,0,0 };
    Vector3 m_scale{ 1,1,1 };
    Vector3 m_rotate{ 0,0,0 };
};