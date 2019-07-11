#include "surfaces\Primitive.h"

Primitive::Primitive(Surface *const surface, const Material & material):
	m_surface(surface),
	m_material(material)
{
}

const Surface * Primitive::surface() const
{
	return m_surface.get();
}

const Material & Primitive::material() const
{
	return m_material;
}
