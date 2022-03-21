#pragma once
#include "./GameObject.h"
#include "./MainCharacter.h"

#define BARREL_ANI_IDLE		0
#define BARREL_ANI_UP			2
#define BARREL_ANI_DOWN			1
class CBarrelObject : public CGameObject
{
	int level;
	DWORD up_effect_start;

	float start_x;			// initial position of BARREL at scene
	float start_y;
	bool is_finish_animation;
	bool is_barrel_up;
public:
	CBarrelObject(float x = 0.0f, float y = 0.0f);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();

	void SetState(int state);
	void SetLevel(int l) { level = l; }
	bool GetIsBarrelUp() { return is_barrel_up; }
	void StartUpEffect() { y -= 1; up_effect_start = GetTickCount(); }
	void Reset();

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);

	void SetIsFinishAnimaion(bool is_finish) { this->is_finish_animation = is_finish; }
	// void SetDelta(D3DXVECTOR2 delta);
};
