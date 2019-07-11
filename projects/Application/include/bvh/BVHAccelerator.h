#pragma once
#include "BVHNode.h"
#include "surfaces\Primitive.h"
#include "RayTracer.h"

#include <vector>
#include <memory>

class BVHAccelerator
{
public:
	void build(std::vector<std::unique_ptr<Primitive>> & primitives);

	bool intersect(const Ray &ray, RayTracer::HitData &hit) const;
	const std::vector<std::unique_ptr<BVHNode>> &nodes() const;
private:

	void buildRecursive(size_t leftIndex, size_t rightIndex, 
		BVHNode * parentNode, const unsigned primitiveThreshold, unsigned currentDepth);
	
	void sort(size_t left, size_t right, unsigned axis);
	AABB combine(const AABB &left, const AABB &right);

	std::vector<std::unique_ptr<BVHNode>> m_nodes;
	std::vector<Primitive*> m_primitives;
};