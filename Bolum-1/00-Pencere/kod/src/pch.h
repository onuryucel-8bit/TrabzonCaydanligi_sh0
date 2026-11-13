#pragma once

//std lib
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include <cstdint>

//glad - glfw - logger
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>

//imgui
#include <imgui.h>
#include <backends/imgui_impl_opengl3.h>

#ifdef cmake_IMGUI_USE_SDL3

	#include <SDL3/SDL.h>
	#include <backends/imgui_impl_sdl3.h>

#elif defined(cmake_IMGUI_USE_GLFW)

	#include <GLFW/glfw3.h>
	#include <backends/imgui_impl_glfw.h>

#endif

//GLM
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>