#pragma once
#include "MathLibrary.h"

class Ray
{
public:
	Ray(const vec3 &origin, const vec3 &endPoint);

	const vec3 getPosition(const float t) const;

	const vec3 &getOrigin() const;
	// This is normalized
	const vec3 &getDirection() const;

private:
	const vec3 m_origin;
	const vec3 m_direction;
};