#pragma once
#include "IRossiState.h"

class CRossiUpState : public IRossiState
{
public:
	void Update(DWORD dt);
	void Render();
};