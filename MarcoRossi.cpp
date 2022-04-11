#include "MarcoRossi.h"
#include "AnimationManager.h"
#include "InputHandler.h"
#include "Utils.h"
#include "SlugMariner.h"
#include "SlugNoid.h"
#include "Camera.h"
#include "PlayScene.h"
#include "SceneManager.h"
#include "PistolBullet.h"
#include "ShotgunBullet.h"
#include "AKBullet.h"
#include "Enemy.h"
#include "Sound.h"
#include "Brick.h"

void CMarcoRossi::InitAnimations()
{
	auto animation_manager = CGame::GetInstance()->GetSystem<CAnimationManager>();
	AddAnimation("Idle", animation_manager->Get("ani-rossi-idle"));
	AddAnimation("Walk", animation_manager->Get("ani-rossi-walk"));
	AddAnimation("Jump", animation_manager->Get("ani-rossi-jump"));
	AddAnimation("Dead", animation_manager->Get("ani-rossi-die"));
	animations.at("Dead")->SetIsLooped(false);
}

void CMarcoRossi::InitColliders()
{
	auto collider = new CCollider2D;
	collider->SetGameObject(this);
	collider->SetOffset(VectorZero());
	collider->SetBoxSize(IDLE_SIZE);
	collider->SetDynamic(true);
	colliders.push_back(collider);
}

void CMarcoRossi::SwitchingCharacter()
{
	lastTimeSwitch = GetTickCount();
	controllable = false;

	// Recovery full power
	power = maxPower;

	transform.position = mariner->GetPosition();
	velocity.x = 0;
	acceleration.x = 0;
	nx = mariner->GetDirection();
	SetState(RossiState::JUMPING);

	CGame::GetInstance()->GetSystem<CSound>()->PlayWaveFile("SwitchCharacter");

	
}

CMarcoRossi::CMarcoRossi()
{
	InitAnimations();
	InitColliders();

	// Player's settings
	tag = CTag::Rossi;
	isEnabled = false;
	onGround = false;
	controllable = false;
	rossi = this;
	SetState(RossiState::IDLE);
}

void CMarcoRossi::SetState(RossiState _state)
{
	state = _state;
	switch (state)
	{
	case RossiState::IDLE:
		velocity.x = 0.0f;
		acceleration.x = 0.0f;
		if (onGround == true) animation = animations.at("Idle");
		break;
	case RossiState::MOVING_LEFT:
		acceleration.x = -WALK_ACCELERATION;
		nx = -1;
		if (onGround == true && velocity.x != 0) animation = animations.at("Walk");
		break;
	case RossiState::MOVING_RIGHT:
		acceleration.x = WALK_ACCELERATION;
		nx = 1;
		if (onGround == true && velocity.x != 0) animation = animations.at("Walk");
		break;
	case RossiState::JUMPING:
		onGround = false;
		velocity.y = JUMP_SPEED;
		animation = animations.at("Jump");
		break;
	case RossiState::DEAD:
		nx = -1;
		velocity = VectorZero();
		acceleration = VectorZero();
		controllable = false;
		animation = animations.at("Dead");
		break;
	default:
		break;
	}
}

void CMarcoRossi::Update(DWORD dt)
{
	DebugOut(L"jason pos %f %f\n", transform.position.x, transform.position.y);

	velocity.y += GRAVITY * dt;
	velocity.x += acceleration.x * dt;

	if (velocity.x > WALK_SPEED) velocity.x = WALK_SPEED;
	else if (velocity.x < -WALK_SPEED) velocity.x = -WALK_SPEED;

	if (controllable == false) return;

	auto inputHandler = CGame::GetInstance()->GetSystem<CInputHandler>();

	if (inputHandler->IsKeyDown(ControlKey::MOVE_RIGHT_KEY))
	{
		SetState(RossiState::MOVING_RIGHT);
	}
	else if (inputHandler->IsKeyDown(ControlKey::MOVE_LEFT_KEY))
	{
		SetState(RossiState::MOVING_LEFT);
	}
	else
	{
		SetState(RossiState::IDLE);
	}

	if (inputHandler->OnKeyDown(ControlKey::JUMPING_KEY) && onGround == true)
	{
		SetState(RossiState::JUMPING);

		CGame::GetInstance()->GetSystem<CSound>()->PlayWaveFile("JasonJump");
	}

	if (inputHandler->OnKeyDown(ControlKey::SWITCH_CHARACTER_KEY) &&
		GetTickCount() - lastTimeSwitch > switchDelay &&
		mariner->GetColliders().at(0)->GetBoundingBox().Contain(colliders.at(0)->GetBoundingBox()))
	{
		SwitchingCharacter();
	}

	if (inputHandler->OnKeyDown(ControlKey::SHOOTING_KEY))
	{
		auto bullet = Instantiate<CPistolBullet>(transform.position);
		bullet->SetVelocity(Vector2(nx * bullet->GetSpeed(), 0.0f));

		CGame::GetInstance()->GetSystem<CSound>()->PlayWaveFile("JasonBullet");
	}

	UntouchableUpdate();
}

void CMarcoRossi::Render()
{
	if (controllable == false)
	{
		if (animation->IsFinished() == true)
		{
			isEnabled = false;
			isDestroyed = true;
		}
	}

	animation->Render(transform.position, -nx, layer_index + 1, 0, damagedColor[colorIndex]);
}

void CMarcoRossi::OnDead()
{
	DebugOut(L"[JASON] On Dead\n");
	SetState(RossiState::DEAD);

	CGame::GetInstance()->GetSystem<CSound>()->PlayWaveFile("JasonDie");
}

void CMarcoRossi::OnOverlapped(CCollider2D* selfCollider, CGameObject* object)
{
	if (dynamic_cast<CEnemy*>(object))
	{
		// TODO: is pushed in the direction of the enemy's movement
		AffectPowerAttribute(((CEnemy*)object)->GetDamageOnCollision());

		CGame::GetInstance()->GetSystem<CSound>()->PlayWaveFile("JasonOnDamaged");
	}
}

void CMarcoRossi::OnCollisionEnter(CCollider2D* selfCollider, CCollisionEvent* collision)
{
	auto other = collision->obj;

	if (dynamic_cast<CBrick*>(other))
	{
		if (onGround == false && collision->ny == 1) onGround = true;
		// TODO: Collise with wall, then hold idle state
	}
	else if (dynamic_cast<CSlugMariner*>(other))
	{
		if (controllable == false && velocity.y < 0)
		{
			isEnabled = false;

			auto game = CGame::GetInstance();
			((CPlayScene*)game->GetSystem<CSceneManager>()->GetCurrentScene())->SetPlayer(mariner);

			mariner->SetControllable(true);
			for (auto co : mariner->GetColliders())
			{
				co->SetTrigger(false);
				co->SetDynamic(true);
			}
		}
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
