#pragma once
#include "GameObject.h"


#define ARROW_BBOX_WIDTH  1280
#define ARROW_BBOX_HEIGHT 16

#define ARROW_STATE_NORMAL	100
#define ARROW_STATE_DESTROYED	101
#define ARROW_STATE_NONE	102

#define ARROW_ANI_NORMAL	0

#define ARROW_ANI_DESTROYED	1
#define ARROW_TYPE_NORMAL	1
#define ARROW_TYPE_DESTRUCTIBLE_1	2
#define ARROW_TYPE_DESTRUCTIBLE_2	3
class CArrow : public CGameObject
{
	int width;
	int height;
	bool isDisplay;
	bool isEnable;
public:
	CArrow(float l, float t, float r, float b);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void SetState(int state);
};