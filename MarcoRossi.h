#pragma once
#include "Playable.h"
#include "IRossiState.h"
#include "Barrel.h"
#include "Legs.h"

class IRossiState;
class CBarrel;
class CLegs;


class CMarcoRossi : public CPlayable
{
private:
	const float MOVE_SPEED = 0.15f;
	const float MOVE_ACCELERATION = 0.0002f;
	const float JUMP_SPEED = 0.7f;
	const float GRAVITY = -0.0026f;

	IRossiState* stateAction;
	IRossiState* stateDirection;

	DWORD lastTimeToLiftGun;
	DWORD lastTimeToLowerGun;

	// Power, Hover in class Player
	virtual void InitColliders();
	void SwitchingCharacter();
public:
	CMarcoRossi();

	CBarrel* barrel;
	CLegs* legs;

	IRossiState* GetActionState() { return this->stateAction; }
	IRossiState* GetDirectionState() { return this->stateDirection; }

	virtual void Update(DWORD dt);
	virtual void Render();
	void OnDead();

	void OnOverlapped(CCollider2D* selfCollider, CGameObject* object);
	void OnCollisionEnter(CCollider2D* selfCollider, CCollisionEvent* collision);
	void OnTriggerEnter(CCollider2D* selfCollider, CCollisionEvent* collision);
};