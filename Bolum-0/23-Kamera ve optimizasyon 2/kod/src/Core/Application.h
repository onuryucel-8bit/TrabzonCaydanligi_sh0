#pragma once

//===STD===========================//
#include <iostream>
#include <chrono>
#include <cstdint>
#include <vector>
#include <algorithm>
#include <numbers>
//===========================//
#include <SDL3/SDL.h>
#include <spdlog/spdlog.h>

#include "imgui.h"

#include "backends/imgui_impl_sdl3.h"
#include "backends/imgui_impl_sdlrenderer3.h"

#include "makroVeTanimlar/TracyLib.h"

#include "tiny_obj_loader.h"

//https://deepwiki.com/g-truc/glm/6.1-simd-optimizations
//https://glm.g-truc.net/0.9.1/api/a00002.html
//#define GLM_FORCE_PURE
//#define GLM_FORCE_INLINE 

//#define GLM_FORCE_CLIP_CONTROL    //Enables custom clip control
#define GLM_FORCE_LEFT_HANDED       //Sol el koordinat duzlemini kullan
#define GLM_FORCE_DEPTH_ZERO_TO_ONE //Derinlik [0,1] arasinda
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>


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
#include "Mesh.h"
#include "Vertex.h"
#include "MeshMatrix.h"

//#define DEBUG_PolygonVerticesSize
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
};

enum class LightMod
{
	NONE,
	FLAT
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
	
	inline void update(float dt);
	inline void inputs(float dt);
	inline void draw();
	inline void drawImgui();

	void applyWindowSize();
	void createTexture();

	std::vector<Mesh> loadObj(std::string model);
	void loadModel(const std::string model);

	//--------------------------------------------//
	//--------------------------------------------//	

	inline void clearColorBuffer(Color_t color);
	inline void clearZbuffer();	
	inline void drawColorBuffer();

	inline glm::vec3 barycentricWeights(glm::vec2 a, glm::vec2 b, glm::vec2 c, glm::vec2 p);

	inline void bresenhamLineAlgo(int x0, int y0, int x1, int y1, Color_t color);
	
	inline void drawPixel(int x, int y, Color_t color);
	inline void drawPixel_zbuffer(int x, int y, Vector4 a, Vector4 b, Vector4 c, Color_t color);


	inline void drawTriangle_zbuffer(glm::vec4 v0, glm::vec4 v1, glm::vec4 v2, Color_t color);

	inline void drawTriangle_zbuffer_v0(
		int x0, int y0, int z0, int w0,
		int x1, int y1, int z1, int w1,
		int x2, int y2, int z2, int w2,
		Color_t color);

	//--------------------------------------------//
	//--------------------------------------------//
	tinyobj::attrib_t m_vertexAttributes;
	std::vector<tinyobj::shape_t> m_shapes;

	std::vector<Vertex> transformedVertices;
	std::vector<Vertex> m_vertices;


	std::vector<Vector3> modelNoktalari;
	std::vector<Triangle> izdusumUcgenleri;

	Vector2 ekranOrtasi;
	
		

	uint8_t m_renderMod;
	CullMod m_cullmode = CullMod::ACTIVE;

	float m_cullValue = 0.0f;
	bool f_arkaPlan = true;

	int currentModel = 0;
	int lastModel = 0;
	const char* models[18] = {
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
		"Kaplama_garipSekil.obj",
		"sponza.obj",
		"sahneV0.1.obj"
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

	
	glm::vec3 m_lightDirection = glm::vec3(0,0,-5.0f);
	LightMod m_lightMod = LightMod::FLAT;
	
	//--------------------------------------------//
	//--------------------------------------------//		
	Color_t m_color = Color::WHITE;
	//=============================//	
	Light m_light;
	RenderContext m_context;
	Graphics gp;
	
	int m_objId = 0;

	std::vector<Mesh> m_meshList;
	std::vector<MeshMatrix> m_meshMatrixList;

	bool f_running = true;

	float FPS = 0;
	float dt = 0;

	//60 derece
	float m_fov = glm::radians(60.0f);
	float m_aspect = (float)m_context.mantiksalGenislik / (float)m_context.mantiksalYukseklik;;
	float m_znear = 0.5f;
	float m_zfar = 1000.0f;

	Camera m_camera;
};

