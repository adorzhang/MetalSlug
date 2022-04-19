#include "RossiHorizontalState.h"
#include "PistolBullet.h"
#include "Sound.h"

CRossiHorizontalState::CRossiHorizontalState()
{
	isTurning = false;
	isMoving = false;
	lastTimeBouncing = GetTickCount();
}

void CRossiHorizontalState::Turning()
{
	isTurning = true;
	lastTimeTurning = GetTickCount();
}

void CRossiHorizontalState::Shooting()
{
	auto newBullet = Instantiate<CPistolBullet>(owner->GetPosition() + owner->barrel->GetPosition());
	int nx = owner->GetDirection();
	newBullet->SetDirection(nx);
	newBullet->SetVelocity(Vector2(nx * newBullet->GetSpeed(), 0.0f));

	CGame::GetInstance()->GetSystem<CSound>()->PlayWaveFile("SophiaBullet");
}

void CRossiHorizontalState::UpdateColliders()
{
	int nx = owner->GetDirection();
	auto colliders = owner->GetColliders();

	colliders.at(0)->SetOffset(OFFSET_ROSSI_IDLE);
	colliders.at(0)->SetBoxSize(BOX_ROSSI_IDLE);
}

void CRossiHorizontalState::Update(DWORD dt)
{
	int parentDirection = owner->GetDirection();

	if (isTurning == true)
	{
		DWORD now = GetTickCount();
		if (now - lastTimeTurning > turningDelay)
		{
			/*int cabinDirection = owner->cabin->GetDirection();
			if (cabinDirection == parentDirection)
			{
				isTurning = false;
			}
			else
			{
				owner->cabin->SetDirection(parentDirection);
			}*/

			lastTimeTurning = now;
		}
	}

	if (isMoving == true)
	{
		DWORD now = GetTickCount();
		if (now - lastTimeBouncing > bouncingDelay)
		{
			if (bounce < 0) bounce = BOUNCE;
			else bounce = -BOUNCE;

			lastTimeBouncing = now;
		}
	}
	else
	{
		bounce = 0;
	}

	owner->barrel->SetPosition(Vector2(-8.0f, 0.0f));
	owner->legs->SetPosition(Vector2(8.0f, 0.0f));

	/*if (isTurning == false)
	{
		owner->cabin->SetPosition(Vector2(-3.5f * parentDirection, 9.0f + bounce));
	}
	else
	{
		owner->cabin->SetPosition(Vector2(-1.0f * parentDirection, 9.0f + bounce));
	}*/
}

void CRossiHorizontalState::Render()
{
}