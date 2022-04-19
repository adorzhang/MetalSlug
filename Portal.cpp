#include "Portal.h"
#include "Playable.h"
#include "MarcoRossi.h"

CPortal::CPortal(float w, float h, int scene_id)
{
	this->scene_id = scene_id;
	width = w;
	height = h;

	tag = CTag::Portal;

	transform.position = Vector2(0.0f, -height);

	auto collider = new CCollider2D;
	collider->SetGameObject(this);
	collider->SetOffset(VectorZero());
	collider->SetBoxSize(Vector2(w, h));
	collider->SetTrigger(true);
	collider->SetDynamic(true);
	colliders.push_back(collider);
}

void CPortal::Update(DWORD dt)
{
}

void CPortal::Render()
{
	colliders.at(0)->RenderBoundingBox();
}

void CPortal::OnTriggerEnter(CCollider2D* selfCollider, CCollisionEvent* collision)
{
	if (dynamic_cast<CPlayable*>(collision->obj))
	{
		CGame::GetInstance()->GetSystem<CSceneManager>()->SwitchSection(scene_id, translation);
	}
}
