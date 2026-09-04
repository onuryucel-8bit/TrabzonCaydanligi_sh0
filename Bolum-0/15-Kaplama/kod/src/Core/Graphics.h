#pragma once

#include <cstdint>
#include <vector>
#include <string>
#include <unordered_map>
#include <iostream>

#include "SDL3_image/SDL_image.h"

#include "makroVeTanimlar/TracyLib.h"

#include "TemelTanimlar/Defs.h"

enum Color : uint32_t
{
	BLACK = 0x0000'0000,
	WHITE = 0xffff'ffff,
	RED = 0xffff'0000,
	GREEN = 0xff00'ff00,
	BLUE = 0xff00'00ff
};

enum class LineAlgoType
{
	Brensham,
	DDA
};

struct TextureData
{
	int width;
	int height;

	Color_t* data;
};

class Graphics
{
public:
	Graphics(RenderContext& context);
	~Graphics();

	void loadTexture(std::string id, std::string path);
	void useTexture(std::string textureName);
	void unbindTexture();

	void clearColorBuffer(Color_t color);
	void drawColorBuffer();

	/*
	* @brief Renk tamponundaki tum pikseller belirtilen renk ile temizlenir
	* @param color renk degeri(uint32_t)
	*/
	void drawPixel(int x, int y, Color_t color = Color::GREEN);

	void drawLine(int x0, int y0, int x1, int y1, Color_t color = Color::GREEN);
	
	void drawTriangle(int x0, int y0, int x1, int y1, int x2, int y2, Color_t color = Color::GREEN);
	
	/*
	*	Merkez([x0][y0]) ucgenNoktalari( [x1][y1] [x2][y2] .... [xn][yn] )
	*/
	void drawTriangleFan(std::vector<int>& pointList, Color_t color = Color::GREEN);

	void drawRectangle(int x, int y, int width, int height, Color_t color = Color::GREEN);
	void drawFilledRectangle(int x, int y, int width, int height, Color_t color = Color::GREEN);
	
	void drawGrid(Color_t color = Color::GREEN);
	void drawDots(Color_t color = Color::GREEN);
	
	void drawFilledTriangle(int x0, int y0, int x1, int y1, int x2, int y2, Color_t color);

	void drawTexturedTriangle(int x0, int y0, int x1, int y1, int x2, int y2);

	void setLineAlgo(LineAlgoType lineAlgoType);
	

    inline static LineAlgoType m_lineAlgoType;

private:
	void swap(int& a, int& b);
	void fillFlatBottomTriangle(int x0, int y0, int x1, int y1, int x2, int y2, Color_t color);
	void fillFlatTopTriangle(int x0, int y0, int x1, int y1, int x2, int y2, Color_t color);

	void ddaLineAlgo(int x0, int y0, int x1, int y1, Color_t color);
	void bresenhamLineAlgo(int x0, int y0, int x1, int y1, Color_t color);

	RenderContext& m_context;
	std::unordered_map<std::string, TextureData> m_textureMap;

	std::string m_currentTextureId;
	bool f_textureActive;
};

