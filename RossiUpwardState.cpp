#include "RossiUpwardState.h"
#include "PistolBullet.h"
#include "Sound.h"

void CRossiUpwardState::Shooting()
{
	auto newBullet = Instantiate<CPistolBullet>(owner->GetPosition() + owner->barrel->GetPosition());
	newBullet->SetVelocity(Vector2(0.0f, newBullet->GetSpeed()));

	CGame::GetInstance()->GetSystem<CSound>()->PlayWaveFile("SophiaBullet");
}

void CRossiUpwardState::UpdateColliders()
{
	int nx = owner->GetDirection();
	auto colliders = owner->GetColliders();

	colliders.at(0)->SetOffset(Vector2(-3.0f * nx, 11.5f));
	colliders.at(0)->SetBoxSize(BOX_SOPHIA_UPWARD);

	owner->SetColliders(colliders);
}

void CRossiUpwardState::Update(DWORD dt)
{
	int nx = owner->GetDirection();
	owner->legs->SetPosition(Vector2(-5.0f, 0.0f));
	owner->barrel->SetPosition(Vector2(5.0f, 0.0f));
}

void CRossiUpwardState::Render()
{

}
