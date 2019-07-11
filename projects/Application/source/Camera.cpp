#include "Camera.h"
#include "Ray.h"

#include <iostream>

Camera::Camera():
	Camera(0,0,0.f)
{
}

Camera::Camera(const int pCountX, const int pCountY, const float totalFovAngle):
	m_pCountX(pCountX),
	m_pCountY(pCountY),
	m_imageAspectRatio(static_cast<float>(pCountX)/static_cast<float>(pCountY)),
	m_cameraToWorld(mat4::identity()),
	m_rotationX(mat4::identity()),
	m_currentRotationX(0),
	m_rotationY(mat4::identity()),
	//m_rotation(mat4::identity()),
	m_translation(mat4::identity())

{
	setFovAngle(totalFovAngle);
}

void Camera::setFovAngle(const float totalFovAngleDegrees)
{
	m_fovAngle = totalFovAngleDegrees;
	m_tangentProduct = std::tan(degreesToRad(m_fovAngle) / 2.f);
}

void Camera::addFovAngle(const float degrees)
{
	m_fovAngle = clamp(m_fovAngle + degrees, -0.f, 180.f);
	setFovAngle(m_fovAngle);
}

const mat4 & Camera::getCamToWorld() const
{
	return m_cameraToWorld;
}

void Camera::translate(const vec3 &translation)
{
	// We multiply by the rotation so that we use the right basis
	m_translation = mat4::translate(m_rotationY * m_rotationX * translation) * m_translation;
}

void Camera::rotateX(const float degrees)
{
	m_currentRotationX = clamp(m_currentRotationX + degrees, -90.f, 90.f);
	m_rotationX = mat4::rotateX(degreesToRad(m_currentRotationX)) * mat4::identity();
}

void Camera::rotateY(const float degrees)
{
	m_rotationY = mat4::rotateY(degreesToRad(degrees)) * m_rotationY;
}
void Camera::resetCamera()
{
	m_translation = mat4::identity();
	m_rotationX = mat4::identity();
	m_rotationY = mat4::identity();
	m_cameraToWorld = mat4::identity();
}

void Camera::updateCamToWorld()
{
	m_cameraToWorld = m_translation* m_rotationY *m_rotationX;
	//m_cameraToWorld = m_translation * m_rotation;
}

Ray Camera::createRay(const int pixelX, const int pixelY) const
{
	const float cameraX = (      ((static_cast<float>(pixelX) + 0.5f) / static_cast<float>(m_pCountX)) * 2.f - 1.f) * m_imageAspectRatio * m_tangentProduct;
	const float cameraY = (1.f - ((static_cast<float>(pixelY) + 0.5f) / static_cast<float>(m_pCountY)) * 2.f      ) * m_tangentProduct;

	vec3 origin(0, 0, 0);
	vec3 direction(cameraX, cameraY, -1);

	origin = m_cameraToWorld * origin;
	direction = m_cameraToWorld * direction;

	Ray ray(origin, direction);

	return ray;
}