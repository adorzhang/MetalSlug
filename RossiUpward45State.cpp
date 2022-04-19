#include "RossiUpward45State.h"

void CRossiUpward45State::Shooting()
{
}

void CRossiUpward45State::UpdateColliders()
{
	int nx = owner->GetDirection();
	auto colliders = owner->GetColliders();

	colliders.at(0)->SetOffset(Vector2(-0.5f * nx, 10.5f));
	colliders.at(0)->SetBoxSize(BOX_SOPHIA_UPWARD_45);
}

void CRossiUpward45State::Update(DWORD dt)
{
	int nx = owner->GetDirection();
	owner->barrel->SetPosition(Vector2(-6.5f*nx, 0.0f));
	owner->legs->SetPosition(Vector2(6.5f*nx, 0.0f));
}

void CRossiUpward45State::Render()
{
}