cmake_minimum_required(VERSION 3.15)

set(CMAKE_TOOLCHAIN_FILE "C:/vcpkg/scripts/buildsystems/vcpkg.cmake")

# projenin ismi
project(TrabzonCaydanligi)

# cmake degiskenleri tanimlaniyor

# c++ versiyonu ayarlaniyor
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#=====================================================================#
#===========Kutuphane ve diger zamazingo ayarlari=====================#
#=====================================================================#

# glfw ayarlari
#set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
#set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
#set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
#set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

# Dinamik baglantiyi ac
set(SDL_SHARED ON CACHE BOOL "Dinamik bagla:: Dinamik zimbirti" FORCE)
# Statik baglantiyi kes
set(SDL_STATIC OFF CACHE BOOL "Statik bagla:: Statik Dolma Pilavi" FORCE)


#====================LIBS=============================================#
#=====================================================================#

# kutuphanelerin dosya yolu(/proje/bolum/baslik/kod)
set(LIBS_LOCAL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../../libs)

# kutuphaneler
find_package(OpenGL REQUIRED)

#vcpkg install glfw3
#find_package(glfw3 REQUIRED)

# imgui ayarlari
# sdl ac
set(IMGUI_USE_SDL3 ON CACHE BOOL "" FORCE)
# glfw ile bagla
set(IMGUI_USE_GLFW OFF CACHE BOOL "" FORCE)



find_package(SDL3 CONFIG REQUIRED)

add_subdirectory(${LIBS_LOCAL_DIR}/imgui-docking imgui_build)

# tracy ayarlari
set(TRACY_ON_DEMAND ON CACHE BOOL "Tracy baglanmadiysa kayitlari toplama, bellegin anasini belleme ulan" FORCE)
set(TRACY_ENABLE ON CACHE BOOL "Enable Tracy Profiling" FORCE)
add_subdirectory(${LIBS_LOCAL_DIR}/tracy tracy_build)

#glad
add_library(glad STATIC "${LIBS_LOCAL_DIR}/baku/glad/src/glad.c")

target_include_directories(glad PUBLIC "${LIBS_LOCAL_DIR}/baku/glad/include")

# header only
add_library(baku INTERFACE)
target_include_directories(baku INTERFACE ${LIBS_LOCAL_DIR}/baku)

#=====================================================================#
#=====================================================================#

# supurge teknigi ile proje dosyalari MY_SOURCES icerisine atiliyor
file(GLOB_RECURSE MY_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

add_executable(${PROJECT_NAME} ${MY_SOURCES})

#spdlog => fmt icin
#static_assert(!FMT_UNICODE || use_utf8,
#              "Unicode support requires compiling with /utf-8");
if (MSVC)
#/O2 sets a combination of optimizations that optimizes code for maximum speed.
#/Ot (a default setting) tells the compiler to favor optimizations for speed over optimizations for size.
    target_compile_options(${PROJECT_NAME} PRIVATE
        /utf-8        
        $<$<CONFIG:Release>:/O3 /Ot>        
    )
endif()

#====================LIBS=============================================#
#=====================================================================#

target_precompile_headers(${PROJECT_NAME} PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/src/pch.h"
)

target_include_directories(${PROJECT_NAME} PRIVATE 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/"
    "${LIBS_LOCAL_DIR}/baku"
)


target_link_libraries(${PROJECT_NAME}
    PRIVATE
    glad
	OpenGL::GL
    imgui
	SDL3::SDL3
	baku
    Tracy::TracyClient    
)

#===================MAKROLAR==========================================#
#=====================================================================#

#hangi imgui baglantisi kullaniliyorsa onun makrosunu ekle
if(IMGUI_USE_SDL3)
    target_compile_definitions(${PROJECT_NAME} PRIVATE cmake_IMGUI_USE_SDL3)
elseif(IMGUI_USE_GLFW)
    target_compile_definitions(${PROJECT_NAME} PRIVATE cmake_IMGUI_USE_GLFW)
endif()


# kod => baslik => bolum /../../../
target_compile_definitions(${PROJECT_NAME} PUBLIC cmake_PROJECT_MODELLER="${CMAKE_CURRENT_SOURCE_DIR}/../../../modeller/")
target_compile_definitions(${PROJECT_NAME} PUBLIC cmake_PROJECT_KAPLAMALAR="${CMAKE_CURRENT_SOURCE_DIR}/../../../kaplamalar/")

