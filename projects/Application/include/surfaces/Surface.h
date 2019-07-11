#pragma once
#include "MathLibrary.h"
#include "AABB.h"
#include "Ray.h"

class Ray;

class Surface
{
public:
	const bool m_hasBoundingVolume;

	// This function returns the multiplier of the ray of intersection
	virtual float hit(const Ray &ray) const = 0;
	virtual vec3 normal(const vec3 &point) const = 0;
	// Infinite planes don't have bounding volumes. It then returns a null ptr
	virtual AABB boundingVolume() const = 0;

protected:
	Surface(bool hasBoundingVolume);
};