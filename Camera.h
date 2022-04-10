#pragma once
#include "Component.h"
#include "Transform.h"
#include "GameObject.h"

class CGameObject;

enum class State
{
	FreePlaying_Scrolling,
	FreePlaying_TopDown,

	Switching_BlockTop,
	Switching_BlockBottom,
	Switching_NoneBlock,

	Switching_StopTop,
	Switching_StopBottom,
};

class CCamera : public CComponent
{
protected:
	const DWORD switchingDuration = 2500;

	Vector2 position;
	Rect boundary;
	Rect freezeBoundary = Rect(104, 64, 152, 160);
	Vector2 bbSize;
	CGameObject* target;

	State state;
	bool isLeftToRight;
	Vector2 velocitySwitching;
	float boundary_blocking;
	float boundary_stopping;
public:
	CCamera();
	~CCamera();
	Vector2 WorldToScreenPoint(Vector2 pos);

	Vector2 GetPosition() { return this->position; }
	void SetPosition(Vector2 pos) { this->position = pos; }
	Rect GetBoundingBox();
	Vector2 GetBoundingBoxSize() { return this->bbSize; }
	void SetBoundingBoxSize(Vector2 boxSize) { this->bbSize = boxSize; }
	void GetBoundary(Rect boundary) { this->boundary = boundary; }
	CGameObject* GetTarget() { return this->target; }
	void SetTarget(CGameObject* target) { this->target = target; }
	void SetState(State state) { this->state = state; }

	void PreUpdateSwitchingScrollingSection(Vector2 destination, Vector2 translation);
	void PreUpdateSwitchingTopdownSection(Vector2 destination, Vector2 translation);
	void Update();
	void UpdateFreePlaying();
	void UpdateSwitchingScrollingSection();
	void UpdateSwitchingTopdownSection();
};