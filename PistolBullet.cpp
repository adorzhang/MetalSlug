#include "PistolBullet.h"
#include "Enemy.h"
#include "Brick.h"
#include "RandomExplosion.h"
#include "BreakableBrick.h"
#include "Sound.h"

void CPistolBullet::InitSprites()
{
	auto sprites = CGame::GetInstance()->GetSystem<CSprites>();
	AddSprite(BULLET_X, sprites->Get("spr-pistol-xbullet"));
	AddSprite(BULLET_Y, sprites->Get("spr-pistol-ybullet"));
}

CPistolBullet::CPistolBullet()
{
	InitSprites();

	// Init collider
	auto collider = new CCollider2D;
	collider->SetGameObject(this);
	collider->SetOffset(VectorZero());
	collider->SetDynamic(true);
	collider->SetTrigger(true);
	colliders.push_back(collider);

	tag =CTag::PlayerBullet;
}

CPistolBullet::~CPistolBullet()
{
}

void CPistolBullet::SetLevel(int jasonHP)
{
}

void CPistolBullet::Initialize(Vector2 direction)
{
	velocity = direction * speed;

	if (velocity.x != 0)
	{
		colliders.at(0)->SetBoxSize(BOX_X);
		axisY = transform.position.y;
	}
	else if (velocity.y != 0)
	{
		colliders.at(0)->SetBoxSize(BOX_Y);
		axisX = transform.position.x;
	}

	angular = CMath::Random(0, 360);
}

void CPistolBullet::Update(DWORD dt)
{
	angular += angularFrequency * dt;

	if (velocity.x != 0)
	{
		transform.position.y = axisY + angularAmplitude * sin(angular * CMath::PI / 180);
	}
	else if (velocity.y != 0)
	{
		transform.position.x = axisX + angularAmplitude * sin(angular * CMath::PI / 180);
	}
}

void CPistolBullet::Render()
{
	if (velocity.x != 0) sprites.at(BULLET_X)->Draw(transform.position, -nx, layer_index);
	else if (velocity.y != 0) sprites.at(BULLET_Y)->Draw(transform.position, 1, layer_index);
}

void CPistolBullet::OnTriggerEnter(CCollider2D* selfCollider, CCollisionEvent* collision)
{
	auto other = collision->obj;
	if (dynamic_cast<CEnemy*>(other))
	{
		((CEnemy*)other)->TakeDamage(damage);

		isEnabled = false;
		isDestroyed = true;
	}
	else if (dynamic_cast<CBrick*>(other))
	{
		isEnabled = false;
		isDestroyed = true;

		CGame::GetInstance()->GetSystem<CSound>()->PlayWaveFile("SwitchCharacter");

		Instantiate<CRandomExplosion>(transform.position);

		if (dynamic_cast<CBreakableBrick*>(other))
		{
			other->SetDestroyed();
			other->SetEnable(false);
		}
	}
}
