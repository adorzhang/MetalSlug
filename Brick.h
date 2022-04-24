#pragma once
#include "GameObject.h"

#define BRICK_BBOX_WIDTH  1280
#define BRICK_BBOX_HEIGHT 16

#define BRICK_STATE_NORMAL	100
#define BRICK_STATE_DESTROYED	101
#define BRICK_STATE_NONE	102

#define BRICK_ANI_NORMAL	0

#define BRICK_ANI_DESTROYED	1
#define BRICK_TYPE_NORMAL	1
#define BRICK_TYPE_DESTRUCTIBLE_1	2
#define BRICK_TYPE_DESTRUCTIBLE_2	3
class CBrick : public CGameObject
{
	int width;
	int height;
	int type;	//Default: 1	Destructible: 2,3
	bool isDisplay;
	bool isEnable;
public:
	CBrick(float l, float t, float r, float b, int type = BRICK_TYPE_NORMAL);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
	virtual void SetState(int state);
	int GetType() { return type; }
};