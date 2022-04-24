#pragma once
#include "./GameObject.h"
#include "./MainCharacter.h"
#define CABIN_ANI_UP	1;
#define CABIN_ANI_HORIZONTAL	0;
class CCabinObject : public CGameObject
{
	int level;
	DWORD up_effect_start;

	float start_x;			// initial position of CABIN at scene
	float start_y;

	
public:
	CCabinObject(float x = 0.0f, float y = 0.0f);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();

	void SetState(int state);
	void SetLevel(int l) { level = l; }
	void StartUpEffect() { y -= 1; up_effect_start = GetTickCount(); }

	void Reset();

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	//void SetDelta(D3DXVECTOR2 delta);
};