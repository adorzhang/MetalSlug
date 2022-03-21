#pragma once
#include "GameObject.h"
#include "MainCharacter.h"
#define GUNHUB_ANI_TEXT 0
#define GUNHUB_ANI_LEVEL 2
class CGunHub :public CGameObject
{
	int power;
	CMainCharacter* player;
public:
	CGunHub();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);

	void SetPlayerObject(CMainCharacter* _player) { player = _player; }
};