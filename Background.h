#pragma once
#include "GameObject.h"
#define BACKGROUND_ANI_NORMAL	0
class CBackground : public CGameObject
{
	int width;
	int height;
	int type;	//Default: 1	Destructible: 2,3
	bool isDisplay;
	bool isEnable;
public:
	CBackground(float x, float y);
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void SetState(int state);
	int GetType() { return type; }
};