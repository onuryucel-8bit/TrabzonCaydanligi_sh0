#pragma once

#include <cstdint>
#include "SDL3/SDL.h"

using Color_t = uint32_t;

struct RenderContext
{
	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;
	SDL_Texture* canvas = nullptr;

	Color_t* colorBuffer = nullptr;
	
	const int WindowWidth = 800;
	const int WindowHeight = 600;
};
