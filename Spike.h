#pragma once
#include "GameObject.h"

class CSpike : public CGameObject
{
	int width;
	int height;
public:
	CSpike(float l, float t, float r, float b);
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void Render();
	
};