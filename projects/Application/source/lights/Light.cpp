#include "lights\Light.h"

Light::Light(const vec3 &center, const float lightIntensity, const vec3 &color) :
	m_center(center),
	m_intensity(lightIntensity),
	m_color(color)
{}