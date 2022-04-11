#pragma once
#include "Playable.h"

enum class MarinerState
{
	IDLE,
	MOVING_LEFT,
	MOVING_RIGHT,
	JUMPING,
	DEAD,
};

class CPlayable;

class CSlugMariner : public CPlayable
{
private:
	const float WALK_SPEED = 0.12f;
	const float WALK_ACCELERATION = 0.002f;
	const float JUMP_SPEED = 0.53f;
	const float GRAVITY = -0.0026f;
	const float CROUCH_SPEED = 0.02f;
	const Vector2 IDLE_SIZE = Vector2(8.0f, 16.0f);

	MarinerState state;
	LPANIMATION animation;
protected:
	void InitAnimations();
	void InitColliders();

	void SwitchingCharacter();
public:
	CSlugMariner();

	void SetState(MarinerState state);

	void Update(DWORD dt);
	void Render();
	void OnDead();

	void OnOverlapped(CCollider2D* selfCollider, CGameObject* object);
	void OnCollisionEnter(CCollider2D* selfCollider, CCollisionEvent* collision);
	void OnTriggerEnter(CCollider2D* selfCollider, CCollisionEvent* collision);
};