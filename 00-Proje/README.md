- Gerekli araclar


- cmake https://cmake.org/download/

- msvc cmake araclari
![cmake](../resimler/cmake_0.png)

## Proje Yapisi

![projeYapisi](../resimler/projeY.png)


**main.cpp**
```cpp
#include <iostream>
int main()
{
    std::cout << "hmmm... Calisiyor ilginc\n";
}
```

## Cmake
**CMakeLists.txt**
```Cmake
#3.15 - 4.0 versiyon araliginda cmake varsa calistir
cmake_minimum_required(VERSION 3.15...4.0)

#projenin ismi
project(TrabzonCaydanligi)

#cmake degiskenleri tanimlaniyor

#c++ versiyonu ayarlaniyor
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


#supurge teknigi ile proje dosyalari MY_SOURCES icerisine atiliyor
file(GLOB_RECURSE MY_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")


add_executable(${PROJECT_NAME} ${MY_SOURCES})

```

![projeAcilimi](../resimler/Adsız.png)

![x64_release](../resimler/Adsız1.png)

![x64_release](../resimler/2026-04-08%2010-09-57.png)