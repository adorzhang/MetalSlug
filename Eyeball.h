#pragma once
#include "EnemyObject.h"
#define EYEBALL_MOVE_SPEED 0.04f;
#define EYEBALL_GRAVITY 0.002f;

#define EYEBALL_BBOX_WIDTH 16
#define EYEBALL_BBOX_HEIGHT 16

#define EYEBALL_STATE_MOVE 100

#define EYEBALL_STATE_DIE 200

#define EYEBALL_STATE_IDLE 300
#define EYEBALL_STATE_CLOSE_EYE 400
#define EYEBALL_STATE_ATTACK 500
#define EYEBALL_ANI_LEFT 0
#define EYEBALL_ANI_CLOSE_EYES 1

#define TIME_START_MOVING			2000
#define TIME_MOVING			1000
class CEyeball :public CEnemyObject
{
	DWORD timeWaitingToMove;
	DWORD timeMoving;
	bool isStartMoving;
	bool isMoving;
	bool isAttack;

	vector<CGameObject*> list_weapon;
public:
	CEyeball(int _item);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void SetState(int state);
	

	
};
