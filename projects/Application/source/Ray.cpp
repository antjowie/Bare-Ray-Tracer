#include "Ray.h"
#include "MathLibrary.h"

Ray::Ray(const vec3 &origin, const vec3 &endPoint):
	m_origin(origin),
	m_direction(vec3(endPoint - origin).normalize())
{
}

const vec3 Ray::getPosition(const float t) const
{
	return m_origin + m_direction*t;
}

const vec3 & Ray::getOrigin() const
{
	return m_origin;
}

const vec3 & Ray::getDirection() const
{
	return m_direction;
}
