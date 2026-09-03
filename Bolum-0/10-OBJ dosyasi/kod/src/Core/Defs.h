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
	
	int fizikselGenislik = 800;
	int fizikselYukseklik = 600;

	int mantiksalGenislik = 800;
	int mantiksalYukseklik = 600;
};
