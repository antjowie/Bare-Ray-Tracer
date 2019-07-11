#include "Scene.h"

#include "surfaces\Surface.h"
#include "lights\Light.h"

#include <string>
#include <fstream>

using json = nlohmann::json;

Scene::Scene(
	std::vector<Primitive*> & primitives, std::vector<Light*> & lights,
	float ambientIntensity, const vec3 &backgroundColor):

	m_ambientIntensity(ambientIntensity),
	m_backgroundColor(backgroundColor)
{
	std::printf("Converting primitives...\n");
	for (std::vector<Primitive*>::const_iterator iter = primitives.begin(); iter != primitives.end(); iter++)
	{
		m_primitives.push_back(std::unique_ptr<Primitive>(*iter));
		if (!(*iter)->surface()->m_hasBoundingVolume)
			m_unbounded.push_back(*iter);
	}
	for (std::vector<Light*>::const_iterator iter = lights.begin(); iter != lights.end(); iter++)
		m_lights.push_back(std::unique_ptr<Light>(*iter));
	
	std::printf("Creating BVH...\n");
	m_bvhAccelerator.build(m_primitives);
	std::printf("Finished BVH!\n");
}

Scene* SceneLoader::load(const int sceneNumber) const
{
	char path[32];
	sprintf_s(path, (m_sceneDirectory + "\\scene%i.json").c_str(), sceneNumber);
	std::ifstream file(path);
	if (!file.is_open())
	{
		std::printf("Error opening file %s. File probably doesn't exist.\n",path);
		return nullptr;
	}

	json j;
	file >> j;

	const json surfaces = j["surfaces"];
	const json lights =   j["lights"];
	const float ambientIntensity = j["scene"]["ambientIntensity"].get<float>();

	vec3 backgroundColor;
	if (j["scene"].count("backgroundColor") == 1)
		backgroundColor = getColor(j["scene"]["backgroundColor"].get<std::string>());
	else
		backgroundColor = vec3(64, 92, 137);

	std::vector<Primitive*>primitivesVec;
	std::vector<Light*>lightsVec;

	// The iterator also contains the key of the current value
	for (json::const_iterator surfaceType = surfaces.begin(); surfaceType != surfaces.end(); surfaceType++)
	{
		for (const json &primitive: *surfaceType)
		{
			primitivesVec.push_back(m_primitiveConstructors.at(surfaceType.key())(primitive));
		}
	}
	for (json::const_iterator lightType = lights.begin(); lightType != lights.end(); lightType++)
	{
		for (const json &light: *lightType)
		{
			lightsVec.push_back(m_lightConstructors.at(lightType.key())(light));
		}
	}
	return new Scene(primitivesVec, lightsVec,ambientIntensity,backgroundColor);
}

void SceneLoader::pushConstructFunction(const std::string &surfaceType, createPrimitive function)
{
	m_primitiveConstructors[surfaceType] = function;
}

void SceneLoader::pushConstructFunction(const std::string &surfaceType, createLight function)
{
	m_lightConstructors[surfaceType] = function;
}

const std::vector<std::unique_ptr<Primitive>> &Scene::primitives() const
{
	return m_primitives;
}

const std::vector<Primitive*>& Scene::unbounded() const
{
	return m_unbounded;
}

const std::vector<std::unique_ptr<Light>> &Scene::lights() const
{
	return m_lights;
}

const BVHAccelerator & Scene::accelerator() const
{
	return m_bvhAccelerator;
}

SceneLoader::SceneLoader(const std::string sceneDirectory):
	m_sceneDirectory(sceneDirectory)
{
}
