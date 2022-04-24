#pragma once
#include "EnemyObject.h"
#define LOCUST_MOVE_SPEED 0.03f;
#define LOCUST_GRAVITY 0.002f;

#define LOCUST_BBOX_WIDTH 71
#define LOCUST_BBOX_HEIGHT 58

#define LOCUST_STATE_MOVE 100
#define LOCUST_STATE_MOVE_RIGHT 101
#define LOCUST_STATE_MOVE_LEFT 102
//#define LOCUST_STATE_JUMP 103
#define LOCUST_STATE_CHANGE_DIRECTION 104
#define LOCUST_STATE_ATTACK 105

#define LOCUST_STATE_DIE 200

#define LOCUST_STATE_IDLE 300


#define LOCUST_ANI_MOVE_LEFT 0
#define LOCUST_ANI_MOVE_RIGHT 1


#define LOCUST_TYPE_NORMAL 1
#define LOCUST_TYPE_SPECIAL 2

#define TIME_CHANGE_DIRECTION 500
class CLocust :public CEnemyObject
{
	DWORD time_moving;
	int type;
public:
	CLocust(int _item, int _type);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void SetState(int state);
	int GetType() { return type; }
};
