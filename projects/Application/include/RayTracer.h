#pragma once
#include "MathLibrary.h"
#include "surfaces\Primitive.h"
#include "lights\Light.h"

#include <vector>
#include <memory>

class Ray;
class Scene;
class BVHNode;

class RayTracer
{
public:
	struct HitData
	{
		const Primitive *hitPrimitive;
		float hitMultiplier;
	};

	RayTracer();

	vec3 calculatePixelColor(
		const Ray &ray,
		const Scene &scene,
		const int currentReflectionDepth = 0) const;

	bool m_renderBVH;
	unsigned m_BVHDepth;
	
private:
	struct IntersectionData
	{
	public:
		IntersectionData(const Ray &ray, const HitData &hit);

		const Material * material;
		vec3 hitPoint;
		//This vector is of unit length
		vec3 hitNormal;
		//This vector is of unit length
		vec3 toRayOrigin;
	};

	const float m_floatOffset;
	const int m_maxReflectionDepth;

	vec3 calculateSurfaceColor(
		const Scene &scene,
		const IntersectionData &data) const;

	bool calculateIntersection(
		const Ray &ray,
		const Scene &scene,
		HitData &data) const;

	bool calculateIntersection(
		const Ray &ray,
		const std::vector<std::unique_ptr<BVHNode>> &nodes,
		unsigned &depth) const;

	Ray createReflectionRay(const IntersectionData &data) const;
	// This function modifies the previous ior to match the new ior
	Ray createRefractionRay(const IntersectionData &data) const;

	float calculateReflectionFresnel(const IntersectionData &data) const;

};