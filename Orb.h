#pragma once
#include "EnemyObject.h"
#define ORB_MOVE_SPEED 0.03f;
#define ORB_GRAVITY 0.002f;

#define ORB_BBOX_WIDTH 18
#define ORB_BBOX_HEIGHT 18

#define ORB_STATE_MOVE 100
#define ORB_STATE_MOVE_RIGHT 101
#define ORB_STATE_MOVE_LEFT 102
//#define ORB_STATE_JUMP 103
#define ORB_STATE_CHANGE_DIRECTION 104
#define ORB_STATE_ATTACK 105

#define ORB_STATE_DIE 200

#define ORB_STATE_IDLE 300


#define ORB_ANI_MOVE_LEFT 0
#define ORB_ANI_MOVE_RIGHT 1


#define ORB_TYPE_NORMAL 1
#define ORB_TYPE_SPECIAL 2

#define TIME_CHANGE_DIRECTION 500
class COrb :public CEnemyObject
{
	DWORD time_moving;
	int type;
public:
	COrb(int _item, int _type);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void SetState(int state);
	int GetType() { return type; }
};
