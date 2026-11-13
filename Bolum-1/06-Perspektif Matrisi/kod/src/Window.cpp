#include "Window.h"

bool Window::init()
{
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(
        SDL_GL_CONTEXT_PROFILE_MASK,
        SDL_GL_CONTEXT_PROFILE_CORE
    );

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    m_window = SDL_CreateWindow("Trabzon Caydanligi", 800, 600, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

    if (m_window == NULL)
    {
        spdlog::error("Pencere acilamadi");
        return false;
    }

    m_glContext = SDL_GL_CreateContext(m_window);

    if (m_glContext == NULL)
    {
        spdlog::error("Context zimbirtisi olusturulamadi");
        return false;
    }
    return true;
}