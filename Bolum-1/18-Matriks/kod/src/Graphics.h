#pragma once

#include <cstdint>
#include <vector>

#include "Defs.h"
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


class Graphics
{
public:
	Graphics(RenderContext& context);
	~Graphics();

	void clearColorBuffer(Color_t color);
	void drawColorBuffer();

	/*
	* @brief Renk tamponundaki tum pikseller belirtilen renk ile temizlenir
	* @param color renk degeri(uint32_t)
	*/
	void drawPixel(int x, int y, Color_t color);

	void drawLine(int x0, int y0, int x1, int y1, Color_t color);
	void ddaLineAlgo(int x0, int y0, int x1, int y1, Color_t color);

	void drawTriangle(int x0, int y0, int x1, int y1, int x2, int y2, Color_t color);
	
	void drawFilledTriangle(int x0, int y0, int x1, int y1, int x2, int y2, Color_t color);

	/*
	*	Merkez(v0) ucgenNoktalari( v1, v2, v3, .... [vn][vn] )
	*/
	void drawTriangleFan(std::vector<Vector2>& pointList, Color_t color);
	
	void drawRectangle(int x, int y, int width, int height, Color_t color);
	void drawFilledRectangle(int x, int y, int width, int height, Color_t color);

	void drawGrid(Color_t color);
	void drawDots(Color_t color);
	
private:

	void fillFlatBottomTriangle(int x0, int y0, int x1, int y1, int x2, int y2, Color_t color);

	void fillFlatTopTriangle(int x0, int y0, int x1, int y1, int x2, int y2, Color_t color);

	void swap(int& a, int& b);

	RenderContext* m_context;
};

