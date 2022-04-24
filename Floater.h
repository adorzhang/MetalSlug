#pragma once
#include "EnemyObject.h"
#define FLOATER_MOVE_SPEED 0.03f;
#define FLOATER_GRAVITY 0.002f;

#define FLOATER_BBOX_WIDTH 18
#define FLOATER_BBOX_HEIGHT 15

#define FLOATER_STATE_MOVE 100
#define FLOATER_STATE_MOVE_CHANGE_DIRECTION_X 101
#define FLOATER_STATE_MOVE_CHANGE_DIRECTION_Y 102
#define FLOATER_STATE_ATTACK 103

#define FLOATER_STATE_DIE 200

#define FLOATER_STATE_IDLE 300


#define FLOATER_ANI_MOVE_LEFT 0
#define FLOATER_ANI_MOVE_RIGHT 1

#define TIME_CHANGE_DIRECTION 1000
class CFloater :public CEnemyObject
{
	DWORD time_moving;
	bool isBeingDown;
	bool isBeingUp;
	bool isRepeat;
	vector<CGameObject*> list_weapon;
public:
	CFloater(int _item);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void SetState(int state);
};
