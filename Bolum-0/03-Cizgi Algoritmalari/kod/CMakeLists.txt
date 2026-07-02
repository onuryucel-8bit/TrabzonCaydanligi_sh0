cmake_minimum_required(VERSION 3.15)

set(CMAKE_TOOLCHAIN_FILE "C:/vcpkg/scripts/buildsystems/vcpkg.cmake")

# projenin ismi
project(TrabzonCaydanligi)

# cmake degiskenleri tanimlaniyor

# c++ versiyonu ayarlaniyor
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Dinamik baglantiyi kes
set(SDL_SHARED OFF CACHE BOOL "Dinamik bagla:: Dinamik zimbirti" FORCE)
# Statik baglantiyi aktif hale getir
set(SDL_STATIC ON CACHE BOOL "Statik bagla:: Statik Dolma Pilavi" FORCE)


#====================LIBS=============================================#
#=====================================================================#

# kutuphanelerin dosya yolu(/proje/bolum/baslik/kod)
set(LIBS_LOCAL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../../libs)

# kutuphaneler
find_package(SDL3 CONFIG REQUIRED)

add_subdirectory(${LIBS_LOCAL_DIR}/imgui-docking imgui_build)

#=====================================================================#
#=====================================================================#

# supurge teknigi ile proje dosyalari MY_SOURCES icerisine atiliyor
file(GLOB_RECURSE MY_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

add_executable(${PROJECT_NAME} ${MY_SOURCES})

#====================LIBS=============================================#
#=====================================================================#

target_link_libraries(${PROJECT_NAME}
    PRIVATE
    SDL3::SDL3
    imgui
)

#=====================================================================#
#=====================================================================#