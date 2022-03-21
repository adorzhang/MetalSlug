#pragma once
#include "EnemyObject.h"
#define ENEMY1_WALKING_SPEED 0.05f;
#define ENEMY1_GRAVITY 0.002f;

#define ENEMY1_BBOX_WIDTH 13
#define ENEMY1_BBOX_HEIGHT 25

#define ENEMY1_STATE_WALKING 100

#define ENEMY1_STATE_DIE 200

#define ENEMY1_STATE_IDLE 300


#define ENEMY1_ANI_WALKING_LEFT 0
#define ENEMY1_ANI_WALKING_RIGHT 1
class CEnemyObject1:public CEnemyObject
{
	
public:
	CEnemyObject1();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void SetState(int state);
};
