#pragma once

#include <cstdint>

#include "Camera.h"
#include "math/Matrix4x4.h"

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

enum class CullMod
{
    NONE,
    ACTIVE
};

enum class LightMod
{
    NONE,
    FLAT
};

enum class DepthTest
{
    NONE,
    PAINTER_AVERAGE,
    PAINTER_DISTANCE
};

struct AppState
{
    Camera m_camera;

    //----------------------------------------------------//
    ProjectMod m_projectMod = ProjectMod::Perspective;
    uint8_t m_renderMod = RenderMod::RenderMode_Triangle;
    CullMod m_cullMod = CullMod::ACTIVE;
    LightMod m_lightMod = LightMod::NONE;
    DepthTest m_depthTest = DepthTest::NONE;
    //----------------------------------------------------//

    Matrix4x4 m_viewMatrix;

    float m_cullValue = 0;
    
    float m_FPS = 0;

    //window
    float m_mouseX = 0;
    float m_mouseY = 0;
    int m_screenWidth = 0;
    int m_screenHeight = 0;

    Vector3 m_position{ 0,0,0 };
    Vector3 m_scale{ 1,1,1 };
    Vector3 m_rotate{ 0,0,0 };
    Vector3 m_lightPos{ 0,0,-2 };
};