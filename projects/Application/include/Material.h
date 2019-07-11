#pragma once
#include "MathLibrary.h"

struct Material
{
public:
	using texturingFunction = vec3(*)(const vec3 &color, const vec2 &uv);
	
	// -Surface(Diffuse) color [0,255] is the color of the surface
	// -Specular exponent (1,inf) is the shinniness of highlight
	//  The higher this value, the less shiny the surface is
	//  Typical values are :
	//  10 — “eggshell”;
	//  100 — mildly shiny;
	//  1000 — really glossy;
	//  10000 — nearly mirror like.
	Material(
		const vec3 &surfaceColor,
		const float specularExponent,
		const float refractionIndex,
		const float surfaceColorRatio);
	
	const vec3		surfaceColor;
	const float		specularExponent;
	const float		refractionIndex;
	const float		surfaceColorRatio;

};