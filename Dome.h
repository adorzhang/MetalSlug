#pragma once
#include "EnemyObject.h"
#define DOME_MOVE_SPEED 0.03f;
#define DOME_GRAVITY 0.002f;

#define DOME_BBOX_WIDTH 18
#define DOME_BBOX_HEIGHT 15

#define DOME_STATE_MOVE 100
#define DOME_STATE_CLIMB 101
#define DOME_STATE_LAY_UP 102

#define DOME_STATE_DIE 200

#define DOME_STATE_START 300


#define DOME_ANI_MOVE_LEFT 0
#define DOME_ANI_MOVE_RIGHT 1
#define DOME_ANI_CLIMB 2
#define DOME_ANI_LAY_UP 3
class CDome :public CEnemyObject
{
	bool isMoving;
	bool isClimbing;
	bool isLayingUp;
public:
	CDome(int _item);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void SetState(int state);
};
