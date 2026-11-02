#include "Light.h"

Light::Light(glm::vec3 direction)
{
	m_direction = direction;
}

Light::~Light()
{
}

Color_t Light::applyLighting(Color_t color, float intensity)
{

    intensity = std::clamp(intensity, 0.0f, 1.0f);

    uint8_t a = (color >> 24) & 0xFF;
    uint8_t r = (color >> 16) & 0xFF;
    uint8_t g = (color >> 8) & 0xFF;
    uint8_t b = (color >> 0) & 0xFF;

    r = static_cast<uint8_t>(r * intensity);
    g = static_cast<uint8_t>(g * intensity);
    b = static_cast<uint8_t>(b * intensity);

    return (a << 24) | (r << 16) | (g << 8) | b;
}
