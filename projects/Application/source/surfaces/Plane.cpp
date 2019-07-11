#include "surfaces\Plane.h"

#include "Ray.h"

Plane::Plane(const vec3 &center, const vec3 &surfaceNormal, const vec3 &size, bool isFinite) : 
	Surface(isFinite),
	m_center(center),
	m_normal(vec3(surfaceNormal).normalize()),
	m_size(size),
	m_isFinite(isFinite)
{
}

float Plane::hit(const Ray &ray) const
{
	const float denom = m_normal.dot(ray.getDirection());
	if (std::abs(denom) > 1e-6f) // Almost parallel
	{
		const float nomin = vec3(m_center - ray.getOrigin()).dot(m_normal);
		const float multiplier = nomin / denom;
		const vec3 distance = m_center - ray.getPosition(multiplier);

		if (!m_isFinite || 
			std::abs(distance[0]) < m_size[0] * 0.5f &&
			std::abs(distance[1]) < m_size[1] * 0.5f &&
			std::abs(distance[2]) < m_size[2] * 0.5f)
			return multiplier;
	}
	return -1.f;
}

vec3 Plane::normal(const vec3 & point) const
{
	point;
	return m_normal;
}

AABB Plane::boundingVolume() const
{
	return AABB(m_center,m_size);
}