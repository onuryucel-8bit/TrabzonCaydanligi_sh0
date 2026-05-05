#pragma once

#include <functional>
#include <vector>
#include <string>

#include "SDL3/SDL.h"

#include "imgui.h"
#include "backends/imgui_impl_sdl3.h"
#include "backends/imgui_impl_sdlrenderer3.h"

#include "RenderSettings.h"


class Gui
{
public:
	Gui(SDL_Window* window, SDL_Renderer* renderer, AppState* appState, std::function<void(std::string)> objLoader);

    void draw();

	~Gui();

private:

    std::function<void(std::string)> m_loadObjModel;

	SDL_Renderer& m_renderer;
	AppState& m_appState;
	ImGuiIO* io;

    //std::function<void(Application&, std::string)> loadObj;

    int currentModel = 0;
    int lastModel = 0;
    const char*models[11] = {
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
    };
};

