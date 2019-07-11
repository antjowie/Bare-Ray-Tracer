#include "Material.h"

Material::Material(
	const vec3 &surfaceColor,
	const float	specularExponent,
	const float refractionIndex,
	const float surfaceColorRatio):

	surfaceColor(surfaceColor),
	// SpecularExponent can't be below or equal to 1
	specularExponent(clamp(specularExponent,1.1f,specularExponent)),
	// If ambientColor is 0, an undesired result, it will copy the surfaceColor
	refractionIndex(clamp(refractionIndex,1.f,refractionIndex)),
	surfaceColorRatio(clamp(surfaceColorRatio,0.f,1.f))
{}