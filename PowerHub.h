#pragma once
#include "GameObject.h"
#include "MainCharacter.h"
#define POWERHUB_ANI_TEXT 0
#define POWERHUB_ANI_LEVEL 1
class CPowerHub :public CGameObject
{
	int power;
	CMainCharacter* player;
public:
	CPowerHub();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);

	void SetPlayerObject(CMainCharacter* _player) { player = _player; }
};