#pragma once
#include "AABB.h"

class BVHNode
{
public:
	void makeLeaf(size_t primitiveIndex, unsigned primitiveCount);
	void makeNode(size_t leftIndex, unsigned primitiveCount);

	// For debug purposes
	unsigned depth;

	bool isLeaf() const;
	size_t childIndex() const;
	unsigned primitiveCount() const;

	void setAABB(const AABB &aabb);
	const AABB &aabb() const; 

private:

	bool m_isLeaf;
	size_t m_childIndex;
	unsigned m_primitiveCount;
	
	AABB m_aabb;
};