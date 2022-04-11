#pragma once
#include "IRossiState.h"

class CRossiRightState : public IRossiState
{
public:
	void Update(DWORD dt);
	void Render();
};