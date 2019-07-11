#pragma once
#include "Surface.h"

class Plane : public Surface
{
public:
	Plane(const vec3 &center, const vec3 &surfaceNormal, const vec3 &size, bool isFinite);

	virtual float hit(const Ray &ray) const override final;
	virtual vec3 normal(const vec3 &point) const override final;
	virtual AABB boundingVolume() const override final;

private:
	const vec3 m_center;
	const vec3 m_normal;
	const vec3 m_size;
	const bool m_isFinite;
};