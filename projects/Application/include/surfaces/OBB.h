#pragma once
#include "Surface.h"

class OBB : public Surface
{
public:
	OBB(
		const vec3 &center,
		const vec3 &size,
		const vec3 &rotation);

	virtual float hit(const Ray &ray) const override final;
	virtual vec3 normal(const vec3 &point) const override final;
	virtual AABB boundingVolume() const override final;

private:
	AABB m_aabb;
	vec3 m_center;

	const mat4 m_objToWorld;
	const mat4 m_worldToObj;
};