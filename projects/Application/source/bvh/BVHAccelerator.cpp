#include "bvh\BVHAccelerator.h"
#include "RayTracer.h"

#include <algorithm>
#include <stack>

#include <cassert>

void BVHAccelerator::build(std::vector<std::unique_ptr<Primitive>> & primitives)
{
	AABB world;
	bool madeWorld = false;
	m_nodes.push_back(std::make_unique<BVHNode>());
	BVHNode *root = m_nodes.back().get();

	for (std::vector<std::unique_ptr<Primitive>>::const_iterator pri = primitives.begin(); pri != primitives.end(); pri++)
	{
		if (pri->get()->surface()->m_hasBoundingVolume)
		{
			if (!madeWorld)
			{
				world = pri->get()->surface()->boundingVolume();
				madeWorld = true;
			}
			m_primitives.push_back(pri->get());
			world = combine(world, pri->get()->surface()->boundingVolume());
		}
	}
	root->setAABB(world);
	buildRecursive(0, m_primitives.size(), root, 2, 0);
}

bool BVHAccelerator::intersect(const Ray &ray, RayTracer::HitData &hit) const
{
	BVHNode * currentNode = m_nodes[0].get();
	hit.hitMultiplier = -1.f;
	if (currentNode->aabb().hit(ray) == -1.f)
		return false;

	std::stack<BVHNode*> nodeStack;
	while (true)
	{
		// Traverse the tree
		if (!currentNode->isLeaf())
		{
			BVHNode *lNode = m_nodes[currentNode->childIndex()].get();
			BVHNode *rNode = m_nodes[currentNode->childIndex() + 1].get();

			const float lHit = lNode->aabb().hit(ray);
			const float rHit = rNode->aabb().hit(ray);

			if (lHit > 0.f && rHit > 0.f)
			{
				if (lHit < rHit)
				{
					currentNode = lNode;
					nodeStack.push(rNode);
					continue;
				}
				else
				{
					currentNode = rNode;
					nodeStack.push(lNode);
					continue;
				}
			}
			else if (lHit > 0.f)
			{
				currentNode = lNode;
				continue;
			}
			else if (rHit > 0.f)
			{
				currentNode = rNode;
				continue;
			}
		}
		else
		{
			for (size_t i = currentNode->childIndex(); i < currentNode->childIndex() + currentNode->primitiveCount(); i++)
			{
				const float hitMultiplier = m_primitives[i]->surface()->hit(ray);
				if (hitMultiplier > 0.f && (hitMultiplier < hit.hitMultiplier || hit.hitMultiplier == -1.f))
				{
					hit.hitMultiplier = hitMultiplier;
					hit.hitPrimitive = m_primitives[i];
				}
			}
			// Excecution will go to nodestack because we traversed a whole tree
		}

		if (nodeStack.empty())
		{
			return hit.hitMultiplier != -1.f;
		}

		while (!nodeStack.empty())
		{
			currentNode = nodeStack.top();
			nodeStack.pop();
			if (currentNode->aabb().hit(ray) < hit.hitMultiplier || hit.hitMultiplier == -1.f)
				break;
		}
	}
}

const std::vector<std::unique_ptr<BVHNode>> &BVHAccelerator::nodes() const
{
	return m_nodes;
}

bool predX(const Primitive *left, const Primitive *right)
{
	return left->surface()->boundingVolume().m_center[0] < right->surface()->boundingVolume().m_center[0];
}

bool predY(const Primitive *left, const Primitive *right)
{
	return left->surface()->boundingVolume().m_center[1] < right->surface()->boundingVolume().m_center[1];
}

bool predZ(const Primitive *left, const Primitive *right)
{
	return left->surface()->boundingVolume().m_center[2] < right->surface()->boundingVolume().m_center[2];
}

void BVHAccelerator::buildRecursive(size_t leftIndex, size_t rightIndex, BVHNode * parentNode, const unsigned primitiveThreshold, unsigned currentDepth)
{
	parentNode->depth = currentDepth;
	if (rightIndex - leftIndex <= primitiveThreshold)
		parentNode->makeLeaf(leftIndex, static_cast<unsigned>(rightIndex - leftIndex));
	else
	{
		// Get largest side
		int largestAxis = 0;
		for (int i = 1; i < 3; i++)
		{
			if (parentNode->aabb().m_size[i] > parentNode->aabb().m_size[largestAxis])
				largestAxis = i;
		}

		sort(leftIndex, rightIndex, largestAxis);
		
		size_t splitIndex = leftIndex;
		float splitPos = parentNode->aabb().m_center[largestAxis];
		while(splitIndex < rightIndex)
		{
			const float pos = m_primitives[splitIndex]->surface()->boundingVolume().m_center[largestAxis];
			if (pos > splitPos)
				break;
			splitIndex++;
		}
		if (splitIndex == leftIndex || splitIndex == rightIndex)
		{
			parentNode->makeLeaf(leftIndex, static_cast<unsigned>(rightIndex - leftIndex));
		}
		else
		{
			AABB left = m_primitives[leftIndex]->surface()->boundingVolume();
			AABB right = m_primitives[splitIndex]->surface()->boundingVolume();
		
			for (size_t i = leftIndex + 1; i < splitIndex; i++)
				left = combine(left, m_primitives[i]->surface()->boundingVolume());
			for (size_t i = splitIndex + 1; i < rightIndex; i++)
				right = combine(right, m_primitives[i]->surface()->boundingVolume());

			
			m_nodes.push_back(std::make_unique<BVHNode>());
			BVHNode * lNode = m_nodes.back().get();
			lNode->setAABB(left);
			m_nodes.push_back(std::make_unique<BVHNode>());
			BVHNode * rNode = m_nodes.back().get();
			rNode->setAABB(right);

			parentNode->makeNode(m_nodes.size() - 2, static_cast<unsigned>(rightIndex - leftIndex));
			
			
			buildRecursive(leftIndex, splitIndex, lNode, primitiveThreshold, currentDepth + 1);
			buildRecursive(splitIndex, rightIndex, rNode, primitiveThreshold, currentDepth + 1);
		}
	}
}

void BVHAccelerator::sort(size_t left, size_t right, unsigned axis)
{
	bool(*pred)(const Primitive*, const Primitive*);
	switch (axis)
	{
	case 0:
		pred = predX;
		break;
	case 1:
		pred = predY;
		break;
	case 2:
		pred = predZ;
		break;
	default:
		assert(false && "Invalid axis chosen");
		pred = predX; // To work around compiler warning
	}
	std::sort(m_primitives.begin() + left, m_primitives.begin() + right, pred);
}

AABB BVHAccelerator::combine(const AABB &left, const AABB &right)
{
	vec3 min;
	vec3 max;
	for (int i = 0; i < 3; i++)
	{
		max[i] = std::fmaxf(left.m_center[i] + left.m_size[i], right.m_center[i] + right.m_size[i]);
		min[i] = std::fminf(left.m_center[i] - left.m_size[i], right.m_center[i] - right.m_size[i]);
	}
	return AABB(min + ((max - min) * 0.5f), max - min);
}
