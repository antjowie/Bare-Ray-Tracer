#pragma once
#include "surfaces\Primitive.h"
#include "MathLibrary.h"
#include "lights\Light.h"

#include "bvh\BVHAccelerator.h"

#include "json.hpp"

#include <memory>
#include <vector>
#include <unordered_map>

class SceneLoader;
class Scene
{
public:
	Scene(
		std::vector<Primitive*> & primitives,
		std::vector<Light*> & lights,
		float ambientIntensity,
		const vec3 &backgroundColor); 

	const std::vector<std::unique_ptr<Primitive>> &primitives() const;
	const std::vector<Primitive*> &unbounded() const;
	const std::vector<std::unique_ptr<Light>> &lights() const;
	const BVHAccelerator &accelerator() const;

	const float m_ambientIntensity;
	const vec3	m_backgroundColor;

private:
	std::vector<std::unique_ptr<Primitive>> m_primitives;
	std::vector<Primitive*> m_unbounded;
	std::vector<std::unique_ptr<Light>>		m_lights;

	BVHAccelerator m_bvhAccelerator;
};

class SceneLoader
{
public:
	using createPrimitive = Primitive * (*)(const nlohmann::json&);
	using createLight = Light * (*)(const nlohmann::json&);

	SceneLoader(const std::string sceneDirectory);
	Scene* load(const int sceneNumber) const;

	void pushConstructFunction(const std::string &surfaceType, createPrimitive function);
	void pushConstructFunction(const std::string &surfaceType, createLight function);

private:
	// Unordered map are faster too index than regular maps. 
	// It's faster to modify the state of a regular map than a unordered map.
	// I don't need the state of the map to be modified, it serves as a look up table.
	std::unordered_map<std::string, createPrimitive> m_primitiveConstructors;
	std::unordered_map<std::string, createLight>   m_lightConstructors;

	std::string m_sceneDirectory;
};