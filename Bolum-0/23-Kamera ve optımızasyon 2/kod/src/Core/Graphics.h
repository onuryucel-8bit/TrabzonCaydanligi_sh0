#pragma once

#include <cstdint>
#include <vector>
#include <string>
#include <unordered_map>
#include <iostream>
#include <algorithm>
#include <cmath>

#include "SDL3_image/SDL_image.h"
#include "spdlog/spdlog.h"
#include <glm/glm/glm.hpp>

#include "makroVeTanimlar/TracyLib.h"

#include "TemelTanimlar/Defs.h"

#include "math/Vector2.h"
#include "math/Vector3.h"


enum Color : uint32_t
{
	BLACK = 0x0000'0000,
	WHITE = 0xffff'ffff,
	RED = 0xffff'0000,
	GREEN = 0xff00'ff00,
	BLUE = 0xff00'00ff
};


struct TextureData
{
	int width = 0;
	int height = 0;

	//Color_t* data;

	std::unique_ptr<Color_t[]> data;
};

class Graphics
{
public:
	Graphics(RenderContext& context);
	~Graphics();

	void loadTexture(std::string id, std::string path);
	void useTexture(std::string textureName);
	

	void clearColorBuffer(Color_t color);
	void clearZbuffer();
	void drawColorBuffer();

	/*
	* @brief Renk tamponundaki tum pikseller belirtilen renk ile temizlenir
	* @param color renk degeri(uint32_t)
	*/
	inline void drawPixel(int x, int y, Color_t color = Color::GREEN);
	inline void drawPixel_zbuffer(int x, int y, Vector4 a, Vector4 b, Vector4 c, Color_t color);

	void drawTexel(
		int x, int y,
		Vector2 a, Vector2 b, Vector2 c,
		float u0, float v0,
		float u1, float v1,
		float u2, float v2);

	void drawTexel_persCorrect(
		int x, int y,
		Vector4 a, Vector4 b, Vector4 c,
		float u0, float v0,
		float u1, float v1,
		float u2, float v2
	);	
	
	void drawTriangle(int x0, int y0, int x1, int y1, int x2, int y2, Color_t color = Color::GREEN);
	
	void drawTriangle_zbuffer(
		glm::ivec4 v0,
		glm::ivec4 v1,
		glm::ivec4 v2,		
		Color_t color = Color::GREEN);
	/*
	*	Merkez([x0][y0]) ucgenNoktalari( [x1][y1] [x2][y2] .... [xn][yn] )
	*/
	void drawTriangleFan(std::vector<int>& pointList, Color_t color = Color::GREEN);

	void drawRectangle(int x, int y, int width, int height, Color_t color = Color::GREEN);
	void drawFilledRectangle(int x, int y, int width, int height, Color_t color = Color::GREEN);
	
	void drawGrid(Color_t color = Color::GREEN);
	void drawDots(Color_t color = Color::GREEN);

	void drawTexturedTriangle_Barycentric(
		int x0, int y0, float u0, float v0,
		int x1, int y1, float u1, float v1,
		int x2, int y2, float u2, float v2);

	void drawTexturedTriangle_Barycentric_PerspectiveCorrect(
		int x0, int y0, int z0, float w0, float u0, float v0,
		int x1, int y1, int z1, float w1, float u1, float v1,
		int x2, int y2, int z2, float w2, float u2, float v2);			    	

	inline void bresenhamLineAlgo(int x0, int y0, int x1, int y1, Color_t color);	

private:
	inline glm::vec3 barycentricWeights(glm::vec2 a, glm::vec2 b, glm::vec2 c, glm::vec2 p);


	
	

	RenderContext& m_context;
	std::unordered_map<std::string, TextureData> m_textureMap;

	std::string m_currentTextureId;	
};

