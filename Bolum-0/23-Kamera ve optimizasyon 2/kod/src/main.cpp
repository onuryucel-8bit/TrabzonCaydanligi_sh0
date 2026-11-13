#include "Core/Application.h"

int main()
{

#ifdef __AVX2__
    spdlog::info("AVX2 supported!");
#else
    spdlog::info("AVX2 not supported");
#endif

    Application app;
    app.run();
}

//https://polyhaven.com/textures
