#pragma once
#include "MathLibrary.h"

class Ray;

class AABB
{
public:
	AABB();
	AABB(
		const vec3 &center,
		const vec3 &size);

	float hit(const Ray &ray) const;

	// Unsigned half size of a side
	vec3 m_size;
	vec3 m_center;
};