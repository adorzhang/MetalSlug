#pragma once
#include "EnemyObject.h"

#define EGG_BBOX_WIDTH  16
#define EGG_BBOX_HEIGHT 16

#define EGG_STATE_NORMAL	100
#define EGG_STATE_DESTROYED	200
#define EGG_STATE_NONE	102

#define EGG_ANI_NORMAL	0

class CEgg : public CEnemyObject
{
	int width;
	int height;
	bool isDisplay;
	bool isEnable;
public:
	CEgg( int item);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void SetState(int state);
};