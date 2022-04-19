#pragma once
#include "IRossiState.h"

#define BOX_SOPHIA_UPWARD_45 Vector2(23.0f, 29.0f)
#define OFFSET_SOPHIA_UPWARD_45 Vector2(-0.5f, 10.5f)

class CRossiUpward45State : public IRossiState
{
public:
	virtual void Shooting();
	virtual void UpdateColliders();
	virtual void Update(DWORD dt);
	virtual void Render();
};