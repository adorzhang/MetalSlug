#pragma once
#include "EnemyObject.h"
#define WORM_MOVE_SPEED 0.03f;
#define WORM_GRAVITY 0.002f;

#define WORM_BBOX_WIDTH 18
#define WORM_BBOX_HEIGHT 9

#define WORM_STATE_MOVE 100
#define WORM_STATE_JUMP 101

#define WORM_STATE_DIE 200


#define WORM_STATE_IDLE 300


#define WORM_ANI_MOVE_LEFT 0
#define WORM_ANI_MOVE_RIGHT 1


class CWorm :public CEnemyObject
{
	bool startMoving;
public:
	CWorm(int _item);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void SetState(int state);
};
