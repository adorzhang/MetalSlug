#include "MarcoRossi.h"
#include "AnimationManager.h"
#include "RossiIdleState.h"
#include "RossiLeftState.h"
#include "RossiRightState.h"
#include "RossiHorizontalState.h"
#include "RossiUpward45State.h"
#include "RossiUpwardState.h"
#include "InputHandler.h"
#include "Utils.h"
#include "Brick.h"
#include "SlugMariner.h"
#include "Camera.h"
#include "SceneManager.h"
#include "Enemy.h"
#include "PlayScene.h"
#include "Portal.h"
#include "RossiExplosion.h"
#include "Sound.h"

void CMarcoRossi::InitColliders()
{
	auto collider = new CCollider2D;
	collider->SetGameObject(this);
	collider->SetOffset(OFFSET_ROSSI_IDLE);
	collider->SetBoxSize(BOX_ROSSI_IDLE);
	collider->SetDynamic(true);
	colliders.push_back(collider);
}

void CMarcoRossi::SwitchingCharacter()
{
	lastTimeSwitch = GetTickCount();
	// Sophia is not controllable, collider is trigger, animation is idle
	controllable = false;
	velocity = VectorZero();
	for (auto co : colliders)
	{
		co->SetTrigger(true);
		co->SetDynamic(false);
	}

	//cabin->Switching();

	// Enable Jason, set jason's position, state is jumping
	auto game = CGame::GetInstance();
	((CPlayScene*)game->GetSystem<CSceneManager>()->GetCurrentScene())->SetPlayer(mariner);

	mariner->SetEnable(true);
	mariner->SetPosition(transform.position);
	mariner->SetControllable(true);
	mariner->SetDirection(nx);
	mariner->SetState(MarinerState::JUMPING);

	CGame::GetInstance()->GetSystem<CSound>()->PlayWaveFile("SwitchCharacter");
}

CMarcoRossi::CMarcoRossi()
{
	InitColliders();

	barrel = new CBarrel(this);
	legs = new CLegs(this);


	stateAction = new CRossiIdleState;
	stateDirection = new CRossiHorizontalState;
	stateAction->SetOwner(this);
	stateDirection->SetOwner(this);

	// Player's settings
	tag = CTag::Rossi;
	controllable = true;
	untouchable = false;
	rossi = this;
}

void CMarcoRossi::Update(DWORD dt)
{
	velocity.x += acceleration.x * dt;
	if (colliders.at(0)->IsDynamic() == true)
		velocity.y += GRAVITY * dt;

	if (velocity.x > MOVE_SPEED) velocity.x = MOVE_SPEED;
	else if (velocity.x < -MOVE_SPEED) velocity.x = -MOVE_SPEED;

#pragma region State Transition
	if (controllable == false)
	{
		if (dynamic_cast<CRossiIdleState*>(stateAction) == nullptr)
			stateAction = new CRossiIdleState;
		if (dynamic_cast<CRossiHorizontalState*>(stateDirection) == nullptr)
			stateDirection = new CRossiHorizontalState;
	}
	else
	{
		auto inputHandler = CGame::GetInstance()->GetSystem<CInputHandler>();

		if (inputHandler->IsKeyDown(ControlKey::MOVE_RIGHT_KEY))
		{
			if (nx == -1)
			{
				if (dynamic_cast<CRossiHorizontalState*>(stateDirection))
				{
					((CRossiHorizontalState*)stateDirection)->Turning();
				}
			}

			acceleration.x = MOVE_ACCELERATION;
			nx = 1;
			stateAction = new CRossiRightState;
			if (dynamic_cast<CRossiHorizontalState*>(stateDirection))
			{
				((CRossiHorizontalState*)stateDirection)->SetIsMoving(true);
			}
		}
		else if (inputHandler->IsKeyDown(ControlKey::MOVE_LEFT_KEY))
		{
			if (nx == 1)
			{
				if (dynamic_cast<CRossiHorizontalState*>(stateDirection))
				{
					((CRossiHorizontalState*)stateDirection)->Turning();
				}
			}

			acceleration.x = -MOVE_ACCELERATION;
			nx = -1;
			stateAction = new CRossiLeftState;
			if (dynamic_cast<CRossiHorizontalState*>(stateDirection))
			{
				((CRossiHorizontalState*)stateDirection)->SetIsMoving(true);
			}
		}
		else
		{
			velocity.x = 0.0f;
			acceleration.x = 0.0f;
			stateAction = new CRossiIdleState;
			if (dynamic_cast<CRossiHorizontalState*>(stateDirection))
			{
				((CRossiHorizontalState*)stateDirection)->SetIsMoving(false);
			}
		}

		// Update gun's direction
		if (dynamic_cast<CRossiUpwardState*>(stateDirection)) lastTimeToLowerGun = GetTickCount();

		if (inputHandler->IsKeyDown(ControlKey::SOPHIA_UPWARD_KEY))
		{
			DWORD now = GetTickCount();
			if (dynamic_cast<CRossiHorizontalState*>(stateDirection))
			{
				stateDirection = new CRossiUpward45State;
			}
			else if (now - lastTimeToLiftGun > 200 && dynamic_cast<CRossiUpward45State*>(stateDirection))
			{
				stateDirection = new CRossiUpwardState;
			}
		}
		else
		{
			lastTimeToLiftGun = GetTickCount();
			DWORD now = lastTimeToLiftGun;
			if (dynamic_cast<CRossiUpwardState*>(stateDirection))
			{
				stateDirection = new CRossiUpward45State;
			}
			else if (now - lastTimeToLowerGun > 200 && dynamic_cast<CRossiUpward45State*>(stateDirection))
			{
				stateDirection = new CRossiHorizontalState;
			}
		}

		if (inputHandler->OnKeyDown(ControlKey::JUMPING_KEY) && onGround == true)
		{
			CGame::GetInstance()->GetSystem<CSound>()->PlayWaveFile("ScrollingMapJump");

			onGround = false;
			velocity.y = JUMP_SPEED;

			DWORD now = GetTickCount();

			if (inputHandler->IsKeyDown(ControlKey::JUMPING_KEY))
			{

			}
		}

		if (inputHandler->OnKeyDown(DIK_C))
		{
			stateDirection->Shooting();
		}

		if (inputHandler->OnKeyDown(ControlKey::SWITCH_CHARACTER_KEY) &&
			GetTickCount() - lastTimeSwitch > switchDelay)
		{
			SwitchingCharacter();
		}
	}
#pragma endregion

	// State Update
	stateDirection->Update(dt);
	stateDirection->UpdateColliders();

	// Part Update
	barrel->Update(dt);
	legs->Update(dt);


	UntouchableUpdate();
}

void CMarcoRossi::Render()
{
	stateAction->Render();
	barrel->Render();
	legs->Render();
}

void CMarcoRossi::OnDead()
{
	DebugOut(L"[SOPHIA] On Dead\n");
	isEnabled = false;
	isDestroyed = true;

	Instantiate<CRossiExplosion>(transform.position);

	CGame::GetInstance()->GetSystem<CSound>()->PlayWaveFile("SophiaDie");
}

void CMarcoRossi::OnOverlapped(CCollider2D* selfCollider, CGameObject* object)
{
	if (dynamic_cast<CEnemy*>(object))
	{
		// TODO: is pushed in the direction of the enemy's movement
		AffectPowerAttribute(((CEnemy*)object)->GetDamageOnCollision());
	}
}

void CMarcoRossi::OnCollisionEnter(CCollider2D* selfCollider, CCollisionEvent* collision)
{
	auto other = collision->obj;

	if (dynamic_cast<CBrick*>(other))
	{
		if (onGround == false && collision->ny == 1) onGround = true;
	}
	else if (dynamic_cast<CEnemy*>(other))
	{
		// TODO: is pushed in the direction of the enemy's movement
		AffectPowerAttribute(((CEnemy*)other)->GetDamageOnCollision());

		CGame::GetInstance()->GetSystem<CSound>()->PlayWaveFile("JasonOnDamaged");
	}
}

void CMarcoRossi::OnTriggerEnter(CCollider2D* selfCollider, CCollisionEvent* collision)
{
}
