#pragma once
#include "IRossiState.h"

#define BOX_ROSSI_IDLE Vector2(24.0f, 17.0f)
#define OFFSET_ROSSI_IDLE Vector2(0.0f, 4.5f)

class CRossiHorizontalState : public IRossiState
{
protected:
	const DWORD turningDelay = 100;
	const DWORD bouncingDelay = 100;
	const float BOUNCE = 0.5;
	DWORD lastTimeTurning;
	DWORD lastTimeBouncing;
	bool isTurning;
	bool isMoving;
	float bounce;
public:
	CRossiHorizontalState();
	void Turning();
	void SetIsMoving(bool value) { this->isMoving = value; }
	bool IsTurning() { return this->isTurning; }

	virtual void Shooting();
	virtual void UpdateColliders();
	virtual void Update(DWORD dt);
	virtual void Render();
};