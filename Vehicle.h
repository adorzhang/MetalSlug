#pragma once
#include "./GameObject.h"
#include "./MainCharacter.h"
#define VEHICLE_ANI_MOVE	0
#define VEHICLE_ANI_XOAY	1
#define VEHICLE_ANI_MO_CABIN	2
#define VEHICLE_ANI_NONG_SUNG	3
#define VEHICLE_UP_DOWN_EFFECT_TIME 100
class CVehicle : public CGameObject
{
	int level;
	DWORD up_effect_start;
	DWORD up_down_effect_time;
	bool is_being_up_effect_cabin;

	float start_x;			// initial position of CABIN at scene
	float start_y;

	bool is_barrel_up;
	int vehicle_nx;

	bool isBarrelStraight;

	float y_delta;
	bool is_firing;
	bool isCabinOpened;
	bool isStateOpenCabin;
public:
	
	CVehicle(float x = 0.0f, float y = 0.0f);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();

	void SetState(int state);
	void SetLevel(int l) { level = l; }
	void StartUpEffect() { y -= 1; up_effect_start = GetTickCount(); }

	void Reset();

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);

	bool GetIsBarrelUp() { return is_barrel_up; }
	bool GetIsBarrelStraight() { return isBarrelStraight; }
	bool GetIsCabinOpened() { return isCabinOpened; }
};