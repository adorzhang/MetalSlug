#pragma once
#include <memory>
#include "GameObject.h"

#define NODE_CAPACITY 10
#define NODE_MAX_DEPTH 6

class CQuadtree
{
private:
	int m_level;
	Rect m_rect;
	std::unique_ptr<CQuadtree> m_subNodes[4];
	std::vector<CGameObject*> m_inNodes;
public:
	CQuadtree(const int level, const Rect& rect);
	~CQuadtree();
	void Split();
	void RemoveGameObjectFromLeaf(CGameObject* gameObject);
	bool Contain(CGameObject* gameObject);
	void Insert(CGameObject* gameObject);
	void Update(std::vector<CGameObject*> gameObjects);
	void Retrieve(std::vector<CGameObject*>& container, const Rect& rect);
	void Reset(float screen_width, float screen_height);
};