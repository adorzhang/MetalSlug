#pragma once
#include "IRossiState.h"

class CRossiLeftState : public IRossiState
{
public:
	void Update(DWORD dt);
	void Render();
};