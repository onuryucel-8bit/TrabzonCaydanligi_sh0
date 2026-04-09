<h1> Temel atma</h1>

![cmake](../resimler/cmake_0.png)

***sekil 0.0***

**Gerekli araclar**

  - Visual Studio kurulum aracindan Sekil 0.0 da gorulmekte olan zimbirtiyi isaretleyip indiriyoruz

- cmake https://cmake.org/download/
     linke tiklayip cmake i kuruyoruz (Windows icin PATH eklenmesi gerekmektedir)

![cmakePATH](../resimler/2026-04-09%2010-28-42.png)

- makefile/ninja

## Proje Yapisi

![projeYapisi](../resimler/projeY.png)

***Sekil 0.1***

- src/ .cpp .h dosyalarin tutuldugu yer
- res/ .obj, kaplamalar ve diger kaynaklarin tutuldugu yer
- libsLocal/ SDL3 ve diger kutuphanelerin tutuldugu yer

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

Projenin bulundugu klasore sag tiklayip Visual studio ile aciyoruz

![x64_release](../resimler/Adsız1.png)



![x64_release](../resimler/2026-04-08%2010-09-57.png)