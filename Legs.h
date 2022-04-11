#pragma once
#include "MarcoRossi.h"

#define SPR_CABIN_00 "Cabin-00"
#define SPR_CABIN_UPWARD "Cabin-Upward"
#define SPR_CABIN_TURN "Cabin-Turn"
#define SPR_CABIN_OPEN "Cabin-Open"

class CLegs : public CGameObject
{
protected:
	const DWORD switchingDuration = 300;
	DWORD lastTimeSwitching;
	CMarcoRossi* parent;
	bool isSwitching;
	void InitSprites();
public:
	CLegs(CMarcoRossi* rossi);
	~CLegs();

	void Switching();

	virtual void Update(DWORD dt);
	virtual void Render();
};

