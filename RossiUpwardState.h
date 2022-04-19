#pragma once
#include "IRossiState.h"

#define BOX_SOPHIA_UPWARD Vector2(24.0f, 31.0f)

class CRossiUpwardState : public IRossiState
{
public:
	virtual void Shooting();
	virtual void UpdateColliders();
	virtual void Update(DWORD dt);
	virtual void Render();
};