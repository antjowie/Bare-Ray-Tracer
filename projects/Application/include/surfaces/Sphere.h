#pragma once
#include "Surface.h"

class Sphere : public Surface
{
public:
	Sphere(const vec3 center, const float radius);

	virtual float hit(const Ray &ray) const override final;

	// This value is not normalized
	virtual vec3 normal(const vec3 &point) const override final;
	virtual AABB boundingVolume() const override final;

private:
	const vec3 m_center;
	const float m_radius;

};