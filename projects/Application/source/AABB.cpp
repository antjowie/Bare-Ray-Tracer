#include "AABB.h"
#include "Ray.h"

AABB::AABB()
{
}

AABB::AABB(const vec3 & center, const vec3 & size) :
	m_center(center),
	m_size(size * 0.5f)
{
}

float AABB::hit(const Ray & ray) const
{
	const float p1 = (m_center[0] - m_size[0] - ray.getOrigin()[0]) / ray.getDirection()[0];
	const float p2 = (m_center[0] + m_size[0] - ray.getOrigin()[0]) / ray.getDirection()[0];

	float tMin = std::fminf(p1, p2);
	float tMax = std::fmaxf(p1, p2);

	for (int i = 1; i < 3; i++)
	{
		if (ray.getDirection()[i] != 0.f)
		{
			const float pos1 = (m_center[i] - m_size[i] -ray.getOrigin()[i]) / ray.getDirection()[i];
			const float pos2 = (m_center[i] + m_size[i] - ray.getOrigin()[i]) / ray.getDirection()[i];

			// If we look at the box from the back
			// pos2 can be the min and pos1 the max
			tMin = std::fmaxf(tMin, std::fminf(pos1, pos2));
			tMax = std::fminf(tMax, std::fmaxf(pos1, pos2));
		}
	}

	if (tMin >= tMax)
		return -1.f;

	if (tMin < 0.f && tMax > 0.f)
		return tMax;
	return tMin;
}