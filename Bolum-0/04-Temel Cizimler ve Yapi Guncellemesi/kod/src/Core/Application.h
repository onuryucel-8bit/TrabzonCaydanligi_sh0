#pragma once

#include <iostream>
#include <chrono>
#include <cstdint>
#include <vector>

#include "SDL3/SDL.h"

#include "imgui.h"

#include "backends/imgui_impl_sdl3.h"
#include "backends/imgui_impl_sdlrenderer3.h"

#include "../Graphics.h"
#include "Defs.h"

class Application
{
public:
	Application();
	~Application();

	void run();

private:
	void initSDL();
	void initImgui();
	void setup();

	void update(float dt);
	void inputs();
	void draw();
	void drawImgui();


	//--------------------------------------------//
	//--------------------------------------------//

	RenderContext rcontext;
	Graphics gp;

	bool f_running = true;

	float FPS;
	float dt;
};

