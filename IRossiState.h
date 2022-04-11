#pragma once
#include "MarcoRossi.h"

class CMarcoRossi;

class IRossiState
{
protected:
	static CMarcoRossi* owner;
public:
	void SetOwner(CMarcoRossi* rossi) { owner = rossi; }
	virtual void Shooting();
	virtual void UpdateColliders();
	virtual void Update(DWORD dt);
	virtual void Render();
};
