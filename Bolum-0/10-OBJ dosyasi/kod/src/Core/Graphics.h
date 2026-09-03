#pragma once

#include <cstdint>
#include <vector>

#include "makroVeTanimlar/TracyLib.h"

#include "Defs.h"

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

	void setLineAlgo(LineAlgoType lineAlgoType);
	

    inline static LineAlgoType m_lineAlgoType;

private:
	void swap(int& a, int& b);
	void fillFlatBottomTriangle(int x0, int y0, int x1, int y1, int x2, int y2, Color_t color);
	void fillFlatTopTriangle(int x0, int y0, int x1, int y1, int x2, int y2, Color_t color);

	void ddaLineAlgo(int x0, int y0, int x1, int y1, Color_t color);
	void bresenhamLineAlgo(int x0, int y0, int x1, int y1, Color_t color);

	RenderContext& m_context;

};

