#pragma once
#include "./GameObject.h"
#include "./MainCharacter.h"

#define LEG_ANI_IDLE		0
#define LEG_ANI_RUN		2
#define LEG_ANI_JUMP			3
#define LEG_ANI_WALK			1
#define WHEEL_EFFECT_TIME 1000
#define WHEEL_BBOX_WIDTH 8
#define LEG_BBOX_WIDTH 22
#define WHEEL_BBOX_HEIGHT 8
#define LEG_BBOX_HEIGHT 22
#define WHEEL_PUSH_EFFECT_TIME 300
#define WHEEL_UP_DOWN_EFFECT_TIME 100
class CLegs : public CGameObject
{
	int level;
	DWORD up_effect_start;
	DWORD push_effect_time;
	DWORD up_down_effect_time;
	bool is_being_up_effect_wheel;
	float start_x;			// initial position of legs at scene
	float start_y;

	//bool is_Right_Wheel;
	//bool is_Middle_Wheel;

	bool is_being_up;

	float x_delta;
	float y_delta;
	bool is_start_push_effect;
	bool is_end_push_effect;
public:
	CLegs(float x = 0.0f, float y = 0.0f);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();

	void SetState(int state);
	void SetLevel(int l) { level = l; }
	void StartUpEffect() { y -= 1; up_effect_start = GetTickCount(); }

	void Reset();

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	//void SetIsRightWheel();
	//void SetIsMiddleWheel();

	//bool GetIsRightWheel() { return is_Right_Wheel; }
};