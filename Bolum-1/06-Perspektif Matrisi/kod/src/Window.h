#pragma once

#include "pch.h"

class Window
{
public:
	bool init();

	SDL_Window* m_window = nullptr;
	SDL_GLContext m_glContext = nullptr;
	int width = 800;
	int height = 600;
};