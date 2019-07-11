#pragma once
#include "MathLibrary.h" 

class Ray;

class Camera
{
public:
	Camera();
	Camera(const int pCountX, const int pCountY, const float totalFovAngle);

	void translate(const vec3 &translation);
	void rotateX(const float degrees);
	void rotateY(const float degrees);

	void resetCamera();
	//void setCamToWorld(const mat4 &transformation);
	//void transformCamToWorld(const mat4 &transformation);
	const mat4 &getCamToWorld() const;

	// Call this function before the createRay function, it applies all the translations to the camToWorld matrix
	void updateCamToWorld();

	Ray createRay(const int pixelX, const int pixelY) const;

	void setFovAngle(const float totalFovAngleDegrees);
	void addFovAngle(const float totalFovDegrees);
	

private:

	mat4 m_cameraToWorld;
	mat4 m_rotationX;
	mat4 m_rotationY;
	float m_currentRotationX;
	//mat4 m_rotation;
	mat4 m_translation;

	float m_fovAngle;
	float m_tangentProduct;
	const int m_pCountX;
	const int m_pCountY;
	const float m_imageAspectRatio;
};