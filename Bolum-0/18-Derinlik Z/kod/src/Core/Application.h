#pragma once

//===STD===========================//
#include <iostream>
#include <chrono>
#include <cstdint>
#include <vector>
#include <algorithm>
#include <numbers>
//===========================//
#include "SDL3/SDL.h"

#include "imgui.h"

#include "backends/imgui_impl_sdl3.h"
#include "backends/imgui_impl_sdlrenderer3.h"

#include "makroVeTanimlar/TracyLib.h"

#include "tiny_obj_loader.h"


//===========================//

#include "math/Angle.h"
#include "math/Vector2.h"
#include "math/Vector3.h"
#include "math/Vector4.h"

#include "math/Matrix4x4.h"
#include "math/Matrix3x3.h"
#include "math/Matrix2x2.h"

#include "TemelTanimlar/Defs.h"
#include "TemelTanimlar/Triangle.h"

#include "Graphics.h"
#include "Camera.h"
#include "Light/Light.h"




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
	RenderMod_Triangle_Filled = 1 << 2,
	RenderMod_Textured = 1 << 3,
	RenderMod_PerspectiveCorrect = 1 << 4,
	RenderMod_Zbuffer = 1 << 5
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
	PAINTER_DISTANCE,
	Z_BUFFER
};

struct Vertex
{
	Vector4 pos;
	Vector2 texCoords;
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

	void loadObjModel(std::string model);

	//--------------------------------------------//
	//--------------------------------------------//

	inline void derinlik(Triangle& projectedTriangle, Vector3 veca, Vector3 vecb, Vector3 vecc);
	inline void isik(Triangle& protectedTriangle, Vector3 normal);
	inline void izdusum(Triangle& protectedTriangle, Matrix4x4 projeksiyonMatrisi, Vector4* verts);

	tinyobj::attrib_t m_vertexAttributes;
	std::vector<tinyobj::shape_t> m_shapes;

	std::vector<Vertex> transformedVertices;
	std::vector<Vertex> m_vertices;


	std::vector<Vector3> modelNoktalari;
	std::vector<Triangle> izdusumUcgenleri;
	
	Vector3 alfa;
	Vector3 pozisyon;
	Vector3 olcek;
	
	Vector2 ekranOrtasi;
	
	Camera m_camera;	

	uint8_t m_renderMod;
	CullMod m_cullmode = CullMod::ACTIVE;

	float m_cullValue = 0.0f;
	bool f_arkaPlan = true;

	int currentModel = 0;
	int lastModel = 0;
	const char* models[16] = {
		"model.obj",
		"ucgenZpozitif.obj",
		"ucgen.obj",
		"kupKaplama_standart1x1.obj",
		"kasa.obj",
		"kup.obj",
		"tuzluk.obj",
		"kale.obj",
		"cardboard_box_02.obj",
		"canned_food_01.obj",
		"candle_holder_01.obj",
		"bowl_01.obj",
		"bottle_02.obj",
		"apple.obj",
		"amphora_01.obj",
		"Kaplama_garipSekil.obj"
	};

	std::string m_currentTextureId = "dama";

	int currentTexture = 0;
	int lastTexture = 0;
	const char* textureid[9] =
	{
		"dama",
		"tombili",
		"cizgi",
		"colorstone",
		"container",
		"graystone",
		"redbrick",
		"wall",
		"wood"
	};

	Light light;
	Vector3 m_lightDirection;
	LightMod m_lightMod = LightMod::FLAT;

	DepthTest m_depthTest = DepthTest::Z_BUFFER;
	
	//--------------------------------------------//
	//--------------------------------------------//
	RenderContext m_context;
	Graphics gp;

	bool f_running = true;

	float FPS = 0;
	float dt = 0;

	//60 derece
	float m_fov = std::numbers::pi / 3.0f;
	float m_aspect = (float)600 / (float)800;
	float m_znear = 0.1f;
	float m_zfar = 100.0f;

	Color_t m_color = Color::WHITE;

};

