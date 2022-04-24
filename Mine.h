#pragma once
#include "EnemyObject.h"
#define MINE_BBOX_WIDTH 16
#define MINE_BBOX_HEIGHT 16

#define MINE_STATE_EXPLOSION 102
#define MINE_STATE_ATTACK 101

#define MINE_STATE_DIE 200

#define MINE_STATE_IDLE 300


#define MINE_ANI_IDLE 0
#define MINE_ANI_EXPLOSION 1
class CMine :public CEnemyObject
{
	bool startMoving;
	vector<CGameObject*> list_weapon;
public:
	CMine(int _item);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void SetState(int state);
};