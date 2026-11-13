# 3.15 - 4.0 versiyon araliginda cmake varsa calistir
cmake_minimum_required(VERSION 3.15...4.0)

# projenin ismi
project(TrabzonCaydanligi)

# cmake degiskenleri tanimlaniyor

# c++ versiyonu ayarlaniyor
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# kutuphanelerin dosya yolu
set(LIBS_LOCAL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libsLocal)


add_library(baku INTERFACE)
target_include_directories(baku INTERFACE ${LIBS_LOCAL_DIR}/baku)

#====================LIBS=============================================#
#=====================================================================#


add_subdirectory(${LIBS_LOCAL_DIR}/SDL3 SDL_build)
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
    baku
)

#=====================================================================#
#=====================================================================#

target_compile_definitions(${PROJECT_NAME} PUBLIC cmake_PROJECT_RES="${CMAKE_CURRENT_SOURCE_DIR}/res/")