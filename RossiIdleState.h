#pragma once
#include "IRossiState.h"

class CRossiIdleState : public IRossiState
{
public:
	void Update(DWORD dt);
	void Render();
};