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
find_package(SDL3_image CONFIG REQUIRED)

add_subdirectory(${LIBS_LOCAL_DIR}/imgui-docking imgui_build)

set(TRACY_ON_DEMAND ON CACHE BOOL "Tracy baglanmadiysa kayitlari toplama, bellegin anasini belleme ulan" FORCE)
set(TRACY_ENABLE ON CACHE BOOL "Enable Tracy Profiling" FORCE)
add_subdirectory(${LIBS_LOCAL_DIR}/tracy tracy_build)

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
        /arch:native
        $<$<CONFIG:Release>:/O3 /Ot>        
    )
endif()

#====================LIBS=============================================#
#=====================================================================#

target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src/")

target_link_libraries(${PROJECT_NAME}
    PRIVATE
    SDL3::SDL3
    imgui
    baku
    Tracy::TracyClient
    SDL3_image::SDL3_image
)

#===================MAKROLAR==========================================#
#=====================================================================#

# kod => baslik => bolum /../../../
target_compile_definitions(${PROJECT_NAME} PUBLIC cmake_PROJECT_MODELLER="${CMAKE_CURRENT_SOURCE_DIR}/../../../modeller/")
target_compile_definitions(${PROJECT_NAME} PUBLIC cmake_PROJECT_KAPLAMALAR="${CMAKE_CURRENT_SOURCE_DIR}/../../../kaplamalar/")

