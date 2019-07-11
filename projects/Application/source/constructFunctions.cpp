#include "constructFunctions.h"

#include "surfaces\Primitive.h"
#include "surfaces\Sphere.h"
#include "surfaces\Plane.h"
#include "surfaces\OBB.h"

#include "lights\Light.h"

#include "Material.h"

template<class T>
T checkKey(const nlohmann::json &json, const char* key, const T &defaultValue)
{
	if (json.count(key) == 0)
		return defaultValue;
	else
		return json[key].get<T>();
}

Material getMaterial(const nlohmann::json & json)
{
	const nlohmann::json &material{ json["material"] };

	return	Material(
		getColor(checkKey(material, "surfaceColor", std::string("FFFFFF"))),
		checkKey(material, "specularExponent", 1000.f),
		checkKey(material, "refractionIndex", 1.f),
		checkKey(material, "surfaceColorRatio", 1.f));
}

Primitive * createSphere(const nlohmann::json & json)
{
	return new Primitive(
		new Sphere(
		vec3(json["x"].get<float>(),
			json["y"].get<float>(),
			json["z"].get<float>()),
		json["radius"].get<float>()),
		getMaterial(json));
}

Primitive * createPlane(const nlohmann::json & json)
{
	const vec3 size(
		json["width"].get<float>(),
		json["height"].get<float>(),
		json["depth"].get<float>());
	const bool isFinite = size == vec3() ? false : true;
	
	return new Primitive(
		new Plane(
		vec3(json["x"].get<float>(),
			json["y"].get<float>(),
			json["z"].get<float>()),
		vec3(
			json["normal"]["x"].get<float>(),
			json["normal"]["y"].get<float>(),
			json["normal"]["z"].get<float>()),
		size, isFinite),
		getMaterial(json));
}

Primitive * createOBB(const nlohmann::json & json)
{
	// This is needed because checkKey can't check if 
	// rotation object doesn't exist
	vec3 rotation;
	if (json.count("rotation"))
		rotation = vec3(
			checkKey(json["rotation"], "x", 0.f),
			checkKey(json["rotation"], "y", 0.f),
			checkKey(json["rotation"], "z", 0.f));
	
	return new Primitive(
		new OBB(

		vec3(
			json["x"].get<float>(),
			json["y"].get<float>(),
			json["z"].get<float>()),
		vec3(
			json["size"]["x"].get<float>(),
			json["size"]["y"].get<float>(),
			json["size"]["z"].get<float>()),
		rotation),
		getMaterial(json));
}

Light * createLight(const nlohmann::json & json)
{
	return new Light(
		vec3(json["x"].get<float>(),
			json["y"].get<float>(),
			json["z"].get<float>()),
		json["intensity"].get<float>(),
		getColor(json["color"]));
}