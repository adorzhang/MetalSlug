#pragma once
#include "GameObject.h"
#include "Playable.h"

class CHealthBar : public CGameObject
{
private:
	Vector2 offset;
	CCamera* mainCam;
	CPlayable* owner;
	void InitSprites();
public:
	CHealthBar();
	void SetOwner(CPlayable* object) { this->owner = object; }
	void Update(DWORD dt);
	void Render();
};

