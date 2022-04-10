#include "Quadtree.h"
#include "Utils.h"

CQuadtree::CQuadtree(const int level, const Rect& rect)
	: m_level(level), m_rect(rect), m_subNodes{ nullptr, nullptr, nullptr, nullptr }
{
	m_inNodes.reserve(NODE_CAPACITY);
}

CQuadtree::~CQuadtree()
{
}

void CQuadtree::Update(std::vector<CGameObject*> gameObjects)
{
	for (const auto& object : gameObjects)
	{
		if (object->IsEnabled() == false) continue;
		if (object->GetQuadtree() == nullptr)
		{
			object->SetEnable(false);
			object->SetDestroyed();
			continue;
		}

		for (auto co : object->GetColliders())
		{
			if (co->IsDynamic() == true)
			{
				if (object->GetQuadtree()->m_rect.Contain(object->GetPosition()) == false)
				{
					// Step 1: Find the leaf which this game object belongs to
					// Step 2: Remove game object from this leaf
					RemoveGameObjectFromLeaf(object);

					// Step 3: Find the new leaf
					Insert(object);
				}
			}
		}
	}
}

void CQuadtree::Split()
{
	Vector2 min = Vector2(m_rect.left, m_rect.bottom);
	Vector2 max = Vector2(m_rect.right, m_rect.top);

	float x = min.x;
	float y = min.y;
	float width = max.x - min.x;
	float height = max.y - min.y;

	float w = width * 0.5;
	float h = height * 0.5;

	Rect left_bottom = Rect(x, y + h, x + w, y);
	Rect right_bottom = Rect(x + w, y + h, x + width, y);
	Rect left_top = Rect(x, y + height, x + w, y + h);
	Rect right_top = Rect(x + w, y + height, x + width, y + h);

	m_subNodes[0] = std::make_unique<CQuadtree>(m_level + 1, left_bottom);
	m_subNodes[1] = std::make_unique<CQuadtree>(m_level + 1, right_bottom);
	m_subNodes[2] = std::make_unique<CQuadtree>(m_level + 1, left_top);
	m_subNodes[3] = std::make_unique<CQuadtree>(m_level + 1, right_top);
}

void CQuadtree::RemoveGameObjectFromLeaf(CGameObject* gameObject)
{
	if (gameObject->GetQuadtree() == nullptr) return;

	Rect rect = gameObject->GetQuadtree()->m_rect;
	if (m_subNodes[0] != nullptr)
	{
		if (m_subNodes[0]->m_rect.Contain(rect)) m_subNodes[0]->RemoveGameObjectFromLeaf(gameObject);
		else if (m_subNodes[1]->m_rect.Contain(rect)) m_subNodes[1]->RemoveGameObjectFromLeaf(gameObject);
		else if (m_subNodes[2]->m_rect.Contain(rect)) m_subNodes[2]->RemoveGameObjectFromLeaf(gameObject);
		else if (m_subNodes[3]->m_rect.Contain(rect)) m_subNodes[3]->RemoveGameObjectFromLeaf(gameObject);

		return;
	}

	if (m_rect.Equal(rect))
	{
		// Normal vector swap
		m_inNodes[gameObject->GetInNodesIndex()] = m_inNodes.back();
		m_inNodes.pop_back();
		// Update vector index
		int index = gameObject->GetInNodesIndex();
		if (index < m_inNodes.size())
		{
			m_inNodes[index]->SetInNodesIndex(index);
		}
		// Set the index of game object to -1
		gameObject->SetInNodesIndex(-1);
		gameObject->SetQuadtree(nullptr);
	}
}

void CQuadtree::Insert(CGameObject* gameObject)
{
	// If this node has split
	if (m_subNodes[0] != nullptr) {
		// Find the subnodes that contain it and insert it there
		if (m_subNodes[0]->Contain(gameObject)) m_subNodes[0]->Insert(gameObject);
		else if (m_subNodes[1]->Contain(gameObject)) m_subNodes[1]->Insert(gameObject);
		else if (m_subNodes[2]->Contain(gameObject)) m_subNodes[2]->Insert(gameObject);
		else if (m_subNodes[3]->Contain(gameObject)) m_subNodes[3]->Insert(gameObject);

		return;
	}

	// Add object to this node
	m_inNodes.emplace_back(gameObject);
	gameObject->SetQuadtree(this);
	gameObject->SetInNodesIndex(m_inNodes.size() - 1);

	// If it has NOT split and NODE_CAPACITY is reached and we are not at MAX LEVEL
	if (m_inNodes.size() > NODE_CAPACITY && m_level < NODE_MAX_DEPTH) {
		// Split into subnodes
		Split();

		// Go through all this nodes objects
		for (const auto& obj : m_inNodes)
		{
			// Go through all newly created subnodes
			if (m_subNodes[0]->Contain(obj)) m_subNodes[0]->Insert(obj);
			else if (m_subNodes[1]->Contain(obj)) m_subNodes[1]->Insert(obj);
			else if (m_subNodes[2]->Contain(obj)) m_subNodes[2]->Insert(obj);
			else if (m_subNodes[3]->Contain(obj)) m_subNodes[3]->Insert(obj);
		}

		// Remove all indexes from THIS node
		m_inNodes.clear();
		m_inNodes.shrink_to_fit();
	}
}

void CQuadtree::Retrieve(std::vector<CGameObject*>& container, const Rect& rect)
{
	if (m_subNodes[0] != nullptr)
	{
		if (m_subNodes[0]->m_rect.Overlap(rect)) m_subNodes[0]->Retrieve(container, rect);
		if (m_subNodes[1]->m_rect.Overlap(rect)) m_subNodes[1]->Retrieve(container, rect);
		if (m_subNodes[2]->m_rect.Overlap(rect)) m_subNodes[2]->Retrieve(container, rect);
		if (m_subNodes[3]->m_rect.Overlap(rect)) m_subNodes[3]->Retrieve(container, rect);

		return;
	}

	// Add all game objects to container
	for (const auto& obj : m_inNodes)
	{
		container.emplace_back(obj);
	}
}

void CQuadtree::Reset(float screen_width, float screen_height)
{
	m_rect = Rect(0, screen_height, screen_width, 0);
	m_inNodes.clear();
	m_inNodes.shrink_to_fit();

	m_subNodes[0] = nullptr;
	m_subNodes[1] = nullptr;
	m_subNodes[2] = nullptr;
	m_subNodes[3] = nullptr;
}

bool CQuadtree::Contain(CGameObject* gameObject)
{
	Vector2 posObj = gameObject->GetPosition();
	return m_rect.Contain(posObj);
}