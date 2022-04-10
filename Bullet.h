#pragma once
#include "GameObject.h"

class CBullet : public CGameObject
{
protected:
	float speed = 0.2f;
	int damage = 1;
public:
	CBullet();
	~CBullet();

	float GetSpeed() { return this->speed; }
	int GetDamage() { return this->damage; }
	virtual void Update(DWORD dt);
	virtual void Render();
};

