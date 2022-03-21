#pragma once
#include "GameObject.h"
class CStair :public CGameObject
{
	int width;
	int height;
public:
	CStair(float l, float t, float r, float b);
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void Render();
};