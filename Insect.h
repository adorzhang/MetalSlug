#pragma once
#include "EnemyObject.h"
#define INSECT_MOVE_SPEED 0.03f;
#define INSECT_GRAVITY 0.002f;

#define INSECT_BBOX_WIDTH 17
#define INSECT_BBOX_HEIGHT 20

#define INSECT_STATE_MOVE 100
#define INSECT_STATE_MOVE_UP_RIGHT 101
#define INSECT_STATE_MOVE_UP_LEFT 102
#define INSECT_STATE_MOVE_DIRECTION_Y_RIGHT 103
#define INSECT_STATE_MOVE_DIRECTION_Y_LEFT 104


#define INSECT_STATE_DIE 200

#define INSECT_STATE_IDLE 300


#define INSECT_ANI_MOVE_LEFT 0
#define INSECT_ANI_MOVE_RIGHT 1

#define TIME_CHANGE_DIRECTION	6000
#define TIME_CHANGE_DIRECTION_Y 1000
class CInsect :public CEnemyObject
{
	DWORD time_moving;
	DWORD time_change;
	bool isMovingLeft;
	bool isMovingRight;
	bool isOnGround;
	bool isBeingDown;
	bool isBeingUp;
	bool isRepeat;
public:
	CInsect(int _item);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void SetState(int state);
};
