#include "bvh\BVHNode.h"

void BVHNode::makeLeaf(size_t primitiveIndex, unsigned primitiveCount)
{
	m_isLeaf = true;
	m_childIndex = primitiveIndex;
	m_primitiveCount = primitiveCount;
}

void BVHNode::makeNode(size_t leftIndex, unsigned primitiveCount)
{
	m_isLeaf = false;
	m_childIndex = leftIndex;
	m_primitiveCount = primitiveCount;
}

bool BVHNode::isLeaf() const
{
	return m_isLeaf;
}

size_t BVHNode::childIndex() const
{
	return m_childIndex;
}

unsigned BVHNode::primitiveCount() const
{
	return m_primitiveCount;
}

void BVHNode::setAABB(const AABB & aabb)
{
	m_aabb = aabb;
}

const AABB &BVHNode::aabb() const
{
	return m_aabb;
}