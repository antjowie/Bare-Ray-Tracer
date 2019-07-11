#include "surfaces\OBB.h"

#include <cmath>

OBB::OBB(const vec3 & center, const vec3 & size, const vec3 &rotation) :
	Surface(true),
	m_aabb(vec3(),size),
	m_center(center),
	m_objToWorld(
		mat4::translate(center) *
		mat4::rotateZ(degreesToRad(rotation[2])) *
		mat4::rotateY(degreesToRad(rotation[1])) *
		mat4::rotateX(degreesToRad(rotation[0])) *
		mat4::identity()),
	m_worldToObj(mat4(m_objToWorld).inverse())
{
}

float OBB::hit(const Ray & ray) const
{
	// Transform world to object space
	const vec3 origin = m_worldToObj * ray.getOrigin();
	const vec3 direction = m_worldToObj * vec4(ray.getDirection(),0);

	const Ray tRay(origin, origin + direction);
	return m_aabb.hit(tRay);
}

vec3 OBB::normal(const vec3 & point) const
{
	// Transform hitpoint to obj space
	const vec3 tPoint = m_worldToObj * point;

	vec3 dir = tPoint;

	for (int i = 0; i < 3; i++)
	{
		dir[i] /= m_aabb.m_size[i];
	}

	int greatest = 0;
	for (int i = 1; i < 3; i++)
	{
		if (std::fabs(dir[i]) > std::fabs(dir[greatest]))
		{
			dir[greatest] = 0.f;
			greatest = i;
		}
		else
		{
			dir[i] = 0.f;
		}
	}

	// Transform normal back to world space without translation
	dir.normalize();
	dir = m_objToWorld * vec4(dir, 0);
	return dir;
}

AABB OBB::boundingVolume() const
{
	const vec3 &size = m_aabb.m_size;

	vec3 max;
	vec3 min;

	// Flip from negative to positive
	for (int i = 0; i < 8; i++)
	{
		float x = -1.f;
		float y = -1.f;
		float z = 1.f;

		if (i % 4 < 2)
			y = 1.f;
		if (i % 4 == 1 || i % 4 == 3)
			x = 1.f;
		if (i > 4)
			z = -1.f;
		
		const vec3 aabbVertex(size[0] * x, size[1] * y, size[2] * z);
		const vec3 obbVertex(m_objToWorld * vec4(aabbVertex, 0.f));

		for (int j = 0; j < 3; j++)
		{
			max[j] = std::fmaxf(max[j], std::fmaxf(aabbVertex[j], obbVertex[j]));
			min[j] = std::fminf(min[j], std::fminf(aabbVertex[j], obbVertex[j]));
		}
	}

	return AABB(m_center,max-min);
}
