#pragma once
#include "EnemyObject.h"
#define LOCUST_FLYING_SPEED 0.03f;
#define LOCUST_GRAVITY 0.002f;

#define LOCUST_BBOX_WIDTH					71
#define LOCUST_BBOX_HEIGHT					58

#define LOCUST_STATE_FLYING					100
#define LOCUST_STATE_TURNING				01
#define LOCUST_STATE_PREPARING_ATTACK		102
#define LOCUST_STATE_ATTACK					103
#define LOCUST_STATE_DIE					104
#define LOCUST_STATE_CATCHING				105
#define LOCUST_STATE_EATING					106
#define LOCUST_STATE_IDLE					107
#define LOCUST_STATE_CHANGE_DIRECTION		108
#define LOCUST_TYPE_NORMAL					109
#define LOCUST_TYPE_SPECIAL					110
#define LOCUST_ANI_FLYING_LEFT				0
#define LOCUST_ANI_FLYING_RIGHT				1

#define TIME_CHANGE_DIRECTION				500

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
