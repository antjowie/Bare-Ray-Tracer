#include "RayTracer.h"
#include "Scene.h"
#include "Ray.h"

#include "surfaces\Primitive.h"
#include "surfaces\Surface.h"
#include "lights\Light.h"
#include "bvh\BVHNode.h"

#include <SFML\System\Clock.hpp>
#include <SFML\Graphics\Texture.hpp>
#include <SFML\Graphics\Sprite.hpp>

#include <ctime>
#include <iostream>

vec3 RayTracer::calculatePixelColor(const Ray & ray, const Scene & scene, const int currentReflectionDepth) const
{
	if (m_renderBVH)
	{
		unsigned depth = 0;
		if (calculateIntersection(ray, scene.accelerator().nodes(),depth))
		{
			float r = 0;
			float g = 0;
			float b = 0;
			switch (depth % 6)
			{
			case 0:
				r = 122.5f;
				break;
			case 1:
				g = 122.5f;
				break;
			case 2:
				b = 122.5f;
				break;
			case 3:
				r = 255.f;
				break;
			case 4:
				g = 255.f;
				break;
			case 5:
				b = 255.f;
				break;
			}

			return vec3(r, g, b);				
		}
		//else
		//	return scene.m_backgroundColor;
	}
	//else
	{
		vec3 pixelColor(0, 0, 0);
	
		HitData hit;
		if (currentReflectionDepth < m_maxReflectionDepth && calculateIntersection(ray, scene, hit))
		//if (currentReflectionDepth < m_maxReflectionDepth && scene.accelerator().intersect(ray, hit))
		{
			IntersectionData data(ray,hit);

			const Material &material = *data.material;
	
			// If surface is diffuse
			if (material.surfaceColorRatio == 1.f && material.refractionIndex == 1.f)
			{
				pixelColor = calculateSurfaceColor(scene, data);
			}
			// If surface is reflective
			else if (material.refractionIndex == 1.f)
			{
				pixelColor = 
					calculateSurfaceColor(scene, data) * material.surfaceColorRatio + 
					calculatePixelColor(createReflectionRay(data), scene, currentReflectionDepth + 1) * (1.f - material.surfaceColorRatio);
			}
			// If surface is reflective and refractive
			else
			{
				const float reflectionFresnel = calculateReflectionFresnel(data);
				if (reflectionFresnel != 1.f)
				{
					pixelColor = calculatePixelColor(createRefractionRay(data), scene, currentReflectionDepth + 1) * (1.f-reflectionFresnel);
				}
				pixelColor += calculatePixelColor(createReflectionRay(data), scene, currentReflectionDepth + 1) * reflectionFresnel;
			}
		}
		else
		{
			pixelColor = scene.m_backgroundColor;
		}
	
		for (int i = 0; i < 3; i++)
			if(pixelColor[i] > 255.f)
				pixelColor[i] = 255.f;
		return pixelColor;
	}
}

bool RayTracer::calculateIntersection(const Ray &ray, const Scene &scene, HitData &data) const
{
	// Calculate intersection
	data.hitMultiplier = -1.f;
	data.hitPrimitive = nullptr;

	scene.accelerator().intersect(ray, data);

	const std::vector<Primitive*> &unbounded = scene.unbounded();
	for (std::vector<Primitive*>::const_iterator primitive = unbounded.begin(); primitive != unbounded.end(); primitive++)
	{
		float hit = (*primitive)->surface()->hit(ray);

		if (hit > 0.f && (hit < data.hitMultiplier || data.hitPrimitive == nullptr))
		{
			data.hitMultiplier = hit;
			data.hitPrimitive = *primitive;
		}
	}
	// If an intersection happened
	if (data.hitPrimitive)
		return true;
	return false;
}

bool RayTracer::calculateIntersection(const Ray &ray, const std::vector<std::unique_ptr<BVHNode>> &nodes, unsigned &depth) const
{
	// Calculate intersection
	for (std::vector<std::unique_ptr<BVHNode>>::const_iterator node = nodes.begin(); node != nodes.end(); node++)
	{
		if (node->get()->depth == m_BVHDepth)
		{
			const AABB aabb = node->get()->aabb();
			const float hit = aabb.hit(ray);

			if (hit > 0.f)
			{
				const vec3 pos = ray.getPosition(hit) - aabb.m_center;
				unsigned edgeCount = 0;
				for (int i = 0; i < 3; i++)
				{
					if (std::abs(pos[i]) > (aabb.m_size[i] - 0.25f))
						edgeCount++;
				}

				if (edgeCount >= 2)
				{
					depth = node->get()->depth;
					return true;
				}
			}
		}
	}

	return false;
}

Ray RayTracer::createReflectionRay(const IntersectionData & data) const
{
	// Calculate reflection ray
	const vec3 incidenceDirection = data.toRayOrigin * -1.f;
	vec3 surfaceNormal = data.hitNormal;
	float iDotN = incidenceDirection.dot(data.hitNormal);
	const vec3 refDirection = incidenceDirection -
		surfaceNormal * 2.f * iDotN;
	const vec3 refOrigin = data.hitPoint + refDirection * m_floatOffset;

	return Ray(refOrigin, refOrigin + refDirection);
}

Ray RayTracer::createRefractionRay(const IntersectionData & data) const
{
	// Assumes that we enter the surface from the air
	float cIor = 1.f;
	float iIor = data.material->refractionIndex;
	const vec3 incidence = data.toRayOrigin * -1.f;

	vec3 surfaceNormal = data.hitNormal;

	float c1 = incidence.dot(data.hitNormal);

	// This means that the ray enters the surface
	if (c1 < 0.f)
	{
		// The normal is facing the right direction,
		// the dot product (c1) should be positive
		c1 = -c1;
	}
	else
	{
		// The normal is facing in the wrong direction
		// The iorRatio also has to be flipped
		surfaceNormal *= -1.f;
		std::swap(cIor, iIor);
	}
	
	const float refractionRatio = cIor / iIor;
	const float c2 = 1.f - ((refractionRatio * refractionRatio) * (1.f - (c1*c1)));

	const vec3 refractedDirection = (incidence * refractionRatio) + (surfaceNormal * (refractionRatio * c1 - std::sqrtf(c2)));
	const vec3 refractedOrigin = data.hitPoint + refractedDirection * m_floatOffset;
	return Ray(refractedOrigin, refractedOrigin + refractedDirection);
}

float RayTracer::calculateReflectionFresnel(const IntersectionData & data) const
{
	const vec3 incidence = data.toRayOrigin * -1.f;
	vec3 surfaceNormal = data.hitNormal;
	float cos1 = incidence.dot(surfaceNormal);

	// n = ior
	float n1 = 1.f;
	float n2 = data.material->refractionIndex;

	if (cos1 > 0.f)
	{
		std::swap(n1, n2);
	}

	const float sin2 = n1 * (1.f - (cos1 * cos1)) / n2;
	// Total internal reflection
	if (sin2 > 1.f)
		return 1.f;

	cos1 = std::fabsf(cos1);
	const float cos2 = std::sqrtf(1.f - (sin2 * sin2));
	const float f1 = 
		(n2 * cos1 - n1 * cos2) / 
		(n2 * cos1 + n1 * cos2);
	const float f2 = 
		(n1 * cos2 - n2 * cos1) / 
		(n1 * cos2 + n2 * cos1);

	return 0.5f*(f1 * f1 + f2 * f2);
}

vec3 RayTracer::calculateSurfaceColor(const Scene & scene, const IntersectionData & data) const
{
	vec3 finalColor;
	for (std::vector<std::unique_ptr<Light>>::const_iterator light = scene.lights().begin(); light != scene.lights().end(); light++)
	{
		// Calculate shadow ray
		const vec3 shadowRayOrigin = data.hitPoint + data.hitNormal * m_floatOffset;
		Ray shadowRay(shadowRayOrigin, light->get()->m_center);

		HitData hit;
		bool inShadow = calculateIntersection(shadowRay, scene, hit);
		//bool inShadow = scene.accelerator().intersect(shadowRay, hit);
		
		// If rays are opposite direction (dot product is lower than 0 so we don't have to normalize)
		// the intersection was past the light. Then no intersection has happened
		if (inShadow && vec3(light->get()->m_center - shadowRay.getPosition(hit.hitMultiplier)).dot(shadowRay.getDirection()) < 0.f)
		{
			inShadow = false;
		}

		const vec3 toLight = vec3(light->get()->m_center - data.hitPoint).normalize();
		if (!inShadow)
		{
			// I use a shading model that uses Lambertian shading for the brightness of a surface
			// Blinn-Phong shading is used for the higlights and I also
			// use a constant component called ambient shading that is used
			// to negate the black places on an object which is a result of Lambertian shading

			// Calculate lambertian shading product
			const float lambertianFactor = std::max(0.f, data.hitNormal.dot(toLight));

			const Material &material = *data.material;
			const vec3 lamb(material.surfaceColor * material.surfaceColorRatio * lambertianFactor * light->get()->m_intensity);

			// Calculate Blinn-Phong shading product
			const vec3 halfBlinnPhong = (toLight + data.toRayOrigin) / (toLight.length() + data.toRayOrigin.length());
	
			const float blinnPhongFactor = std::powf(std::max(0.f, data.hitNormal.dot(halfBlinnPhong)), 
				material.specularExponent);
	
			const vec3 blinn(light->get()->m_color * blinnPhongFactor * light->get()->m_intensity);
			
			finalColor += lamb + blinn;
		}
	}

	// Add ambient shading product
	finalColor += data.material->surfaceColor * scene.m_ambientIntensity;

	// Check if color is inside bounds
	for (int i = 0; i < 3; i++)
	{
		if (finalColor[i] > 255.f)
			finalColor[i] = 255.f;
	}
	return finalColor;
}

RayTracer::RayTracer() : 
	m_floatOffset(1e-2f),
	m_maxReflectionDepth(4),
	m_renderBVH(false),
	m_BVHDepth(0)
{
}

RayTracer::IntersectionData::IntersectionData(const Ray &ray, const HitData &hit)
{
	material = &hit.hitPrimitive->material();
	hitPoint = ray.getPosition(hit.hitMultiplier);

	hitNormal = hit.hitPrimitive->surface()->normal(hitPoint);
	hitNormal.normalize();

	toRayOrigin = ray.getOrigin() - hitPoint;
	toRayOrigin.normalize();
}
