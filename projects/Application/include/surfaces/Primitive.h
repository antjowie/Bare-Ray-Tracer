#pragma once
#include "Material.h"
#include "Surface.h"

#include <memory>

class Primitive
{
public:
	Primitive(Surface *const surface, const Material &material);

	const Surface *surface() const;
	const Material &material() const;

private:
	const std::unique_ptr<Surface> m_surface;
	const Material m_material;
};