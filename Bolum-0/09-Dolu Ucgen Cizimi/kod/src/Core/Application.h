#pragma once

#include <iostream>
#include <chrono>
#include <cstdint>
#include <vector>

#include "SDL3/SDL.h"

#include "imgui.h"

#include "backends/imgui_impl_sdl3.h"
#include "backends/imgui_impl_sdlrenderer3.h"



#include "Graphics.h"
#include "Defs.h"

#include "../math/Angle.h"
#include "../math/Vector2.h"
#include "../math/Vector3.h"
#include "../math/Vector4.h"

#include "../math/Matrix4x4.h"
#include "../math/Matrix3x3.h"
#include "../math/Matrix2x2.h"

#include "Camera.h"
#include "Face.h"
#include "Triangle.h"



//#define DEBUG_FULLSCREEN

enum class CullMod
{
	NONE,
	ACTIVE
};

enum RenderMod : uint8_t
{
	RenderMod_Vertex = 1 << 0,
	RenderMod_Triangle = 1 << 1,
	RenderMod_Triangle_Filled = 1 << 2
};

class Application
{
public:
	Application();
	~Application();

	void run();

private:
	void initSDL();
	void initImgui();
	
	void update(float dt);
	void inputs();
	void draw();
	void drawImgui();

	void applyWindowSize();
	void createTexture();

	void loadCube();

	//--------------------------------------------//
	//--------------------------------------------//

	std::vector<Vector3> modelNoktalari;
	std::vector<Triangle> izdusumUcgenleri;
	std::vector<Face> yuzeyListesi;
	
	Vector3 alfa;
	Vector3 pozisyon;
	Vector3 olcek;

	//ekranin ortasi
	//float cx = m_context.fizikselGenislik / 2.0f;
	//float cy = m_context.fizikselYukseklik / 2.0f;
	Vector2 ekranOrtasi;
	
	Camera m_camera;

	uint8_t m_renderMod = RenderMod::RenderMod_Triangle;
	CullMod m_cullmode = CullMod::ACTIVE;

	float m_cullValue = 1.0f;
	bool f_arkaPlan = true;
	//--------------------------------------------//
	//--------------------------------------------//
	RenderContext m_context;
	Graphics gp;

	bool f_running = true;

	float FPS = 0;
	float dt = 0;

	Color_t m_color = Color::WHITE;

};

