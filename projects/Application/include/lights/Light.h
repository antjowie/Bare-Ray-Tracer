#pragma once
#include "MathLibrary.h"

class Light
{
public:
	Light(const vec3 &center, const float lightIntensity, const vec3 &color);

	const vec3 m_center;
	const vec3 m_color;
	const float m_intensity;
};