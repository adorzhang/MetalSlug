#pragma once
#include "EnemyObject.h"
#define CHOWMEIN_MOVE_SPEED 0.03f;
#define CHOWMEIN_GRAVITY 0.002f;

#define CHOWMEIN_BBOX_WIDTH 49
#define CHOWMEIN_BBOX_HEIGHT 47

#define CHOWMEIN_STATE_IDLE 100
#define CHOWMEIN_STATE_WALKING 101
#define CHOWMEIN_STATE_JUMP 102
#define CHOWMEIN_STATE_TURNING 103
#define CHOWMEIN_STATE_ATTACK 104
#define CHOWMEIN_STATE_EATING 105
#define CHOWMEIN_STATE_DIE 106

#define CHOWMEIN_ANI_IDLE 0
#define CHOWMEIN_ANI_MOVE_LEFT 0
#define CHOWMEIN_ANI_MOVE_RIGHT 1
#define CHOWMEIN_ANI_JUMP 2
#define CHOWMEIN_ANI_TURNING 3
#define CHOWMEIN_ANI_ATTACK 4
#define CHOWMEIN_ANI_EATING 5
#define CHOWMEIN_ANI_DIE 6


class CChowmein :public CEnemyObject
{
	bool startMoving;
	int currentAni = -1;
public:
	CChowmein(int _item);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void SetState(int state);
	bool isPlaying;
};
