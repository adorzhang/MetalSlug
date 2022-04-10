#include "GameObject.h"

CGameObject::CGameObject()
{
	nx = 1;
	transform.position = VectorZero();
	velocity = VectorZero();
	acceleration = VectorZero();
	colliders.clear();
}

CGameObject::~CGameObject()
{
	for (auto co : colliders)
	{
		delete co;
		co = nullptr;
	}
	colliders.clear();

	delete ownerQuadtree;
}

void CGameObject::AddSprite(std::string stateName, LPSPRITE sprite)
{
	sprites.insert(std::make_pair(stateName, sprite));
}

void CGameObject::AddAnimation(std::string stateName, LPANIMATION animation)
{
	animations.insert(std::make_pair(stateName, animation));
}

void CGameObject::PhysicsUpdate(std::vector<CGameObject*>* coObjects)
{
	for (auto co : colliders)
		if (co != nullptr) co->PhysicsUpdate(coObjects);
}

void CGameObject::OnOverlapped(CCollider2D* selfCollider, CGameObject* object)
{
}

void CGameObject::OnCollisionEnter(CCollider2D* selfCollider, CCollisionEvent* collision)
{
}

void CGameObject::OnTriggerEnter(CCollider2D* selfCollider, CCollisionEvent* collision)
{
}

void CGameObject::OnDestroy()
{
}
