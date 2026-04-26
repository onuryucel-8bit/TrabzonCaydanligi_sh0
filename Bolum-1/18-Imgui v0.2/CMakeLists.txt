cmake_minimum_required(VERSION 3.15...4.0)
project(imgui)

option(IMGUI_USE_SDL3 "SDL3" OFF)
option(IMGUI_USE_GLFW "GLFW" OFF)

add_library(imgui)
target_sources(imgui PRIVATE 
"${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_demo.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_draw.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_tables.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_widgets.cpp"
)

if(IMGUI_USE_SDL3)

    target_sources(imgui PRIVATE 
    "${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends/imgui_impl_sdl3.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends/imgui_impl_sdlrenderer3.cpp"
    )

    target_link_libraries(imgui PUBLIC SDL3::SDL3)
endif()


if(IMGUI_USE_GLFW)

    target_sources(imgui PRIVATE 
    "${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends/imgui_impl_glfw.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends/imgui_impl_opengl3.cpp"
    )
    target_link_libraries(imgui PUBLIC glfw)
endif()

target_include_directories(imgui PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/imgui")

