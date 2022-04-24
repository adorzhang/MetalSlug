#pragma once
#include "GameObject.h"
#include "Game.h"
#define STATE_ITEM 10000
#define ITEM_P_BBOX_WIDTH	16
#define ITEM_P_BBOX_HEIGHT	16
//#include "Weapon.h"
class CEnemyObject : public CGameObject
{
protected:
	int blood;
	int item;
	bool isEnable;
	bool isDisplay;
	bool hasItem;
	CGameObject* player;
public:
	CEnemyObject();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom) = 0;
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render() = 0;
	virtual void SetState(int state) {};
	void LostBlood(int _blood) { this->blood -= _blood; }

	int GetBlood() { return this->blood; }
	void SetPlayerObject(CGameObject* _player) { this->player = _player; }
	LPGAMEOBJECT GetPlayerObject() { return this->player; }
};