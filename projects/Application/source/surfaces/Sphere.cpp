#include "surfaces\Sphere.h"

#include "Ray.h"

Sphere::Sphere(const vec3 center, const float radius):
	Surface(true),
	m_center(center),
	m_radius(radius)
{
}

float Sphere::hit(const Ray & ray) const
{
	// These are the values used by my formula
	const vec3 e = ray.getOrigin();
	const vec3 d = ray.getDirection();
	const vec3 c = m_center;

	// These are the values used by the quadratic expression
	const float A = d.dot(d);
	const float B = 2.f * (e - c).dot(d);
	const float C = e.dot(e) + c.dot(c) - (2.f * e.dot(c)) - (m_radius*m_radius);

	float discriminant = B*B - 4.f * A*C; 

	if (discriminant < 0.f)
		return -1.f;

	discriminant = std::sqrtf(discriminant);
	float t1 = (-B + discriminant) / (2.f*A);
	float t2 = (-B - discriminant) / (2.f*A);

	if (t1 < 0.f)
		t1 = t2;
	if (t2 < 0.f)
		t2 = t1;

	return t1 < t2 ? t1 : t2;
}

vec3 Sphere::normal(const vec3 & point) const
{
	return vec3(point - m_center);
}

AABB Sphere::boundingVolume() const
{
	return AABB(m_center,vec3(m_radius*2.f, m_radius*2.f, m_radius*2.f));
}
