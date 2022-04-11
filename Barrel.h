#pragma once
#include "MarcoRossi.h"

#define SPR_CABIN_00 "Cabin-00"
#define SPR_CABIN_UPWARD "Cabin-Upward"
#define SPR_CABIN_TURN "Cabin-Turn"
#define SPR_CABIN_OPEN "Cabin-Open"

class CBarrel : public CGameObject
{
protected:
	const DWORD switchingDuration = 300;
	DWORD lastTimeSwitching;
	CMarcoRossi* parent;
	bool isSwitching;
	void InitSprites();
public:
	CBarrel(CMarcoRossi* rossi);
	~CBarrel();

	void Switching();

	virtual void Update(DWORD dt);
	virtual void Render();
};

