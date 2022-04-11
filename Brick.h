#pragma once
#include "GameObject.h"

#define BRICK_WIDTH 16.0f

class CBrick : public CGameObject
{
public:
	CBrick();
	void Update(DWORD dt);
	void Render();
};