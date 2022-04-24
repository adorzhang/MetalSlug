#pragma once
#include "EnemyObject.h"
#define JUMPER_MOVE_SPEED 0.03f;
#define JUMPER_GRAVITY 0.002f;

#define JUMPER_BBOX_WIDTH 17
#define JUMPER_BBOX_HEIGHT 25

#define JUMPER_STATE_MOVE 100
#define JUMPER_STATE_MOVE_RIGHT 101
#define JUMPER_STATE_MOVE_LEFT 102
#define JUMPER_STATE_JUMP 103

#define JUMPER_STATE_DIE 200

#define JUMPER_STATE_IDLE 300


#define JUMPER_ANI_MOVE_LEFT 0
#define JUMPER_ANI_MOVE_RIGHT 1

#define TIME_CHANGE_DIRECTION 500
class CJumper :public CEnemyObject
{
	DWORD time_moving;
	bool isMovingLeft;
	bool isMovingRight;
	bool isOnGround;
	bool isJumping;
public:
	CJumper(int _item);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void SetState(int state);
	void SetIsJumping(bool _flag) { isJumping = _flag; }
};
