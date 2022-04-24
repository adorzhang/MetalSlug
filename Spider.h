#pragma once
#include "EnemyObject.h"
#define SPIDER_MOVE_SPEED 0.03f;
#define SPIDER_GRAVITY 0.002f;

#define SPIDER_BBOX_WIDTH 18
#define SPIDER_BBOX_HEIGHT 15

#define SPIDER_STATE_MOVE 100

#define SPIDER_STATE_DIE 200

#define SPIDER_STATE_IDLE 300


#define SPIDER_ANI_MOVE_LEFT 0
#define SPIDER_ANI_MOVE_RIGHT 1
class CSpider :public CEnemyObject
{
	
public:
	CSpider();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void SetState(int state);
};
