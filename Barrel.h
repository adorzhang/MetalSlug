#pragma once
#include "./GameObject.h"
#include "./MainCharacter.h"

#define BODY_ANI_IDLE	0
#define BODY_ANI_LIE	1
#define BODY_ANI_SWIM	2
#define BODY_ANI_STARE	3
#define BODY_ANI_SHOOT_UP	4
#define BODY_ANI_SHOOT_DOWN 5
#define BODY_ANI_SHOOT_STRAIGHT	6
#define BODY_ANI_KNIFE_HIT	7
#define BODY_UP_DOWN_EFFECT_TIME 100

class CBarrel : public CGameObject
{
	int level;
	int ani;
	DWORD up_effect_start;
	DWORD up_down_effect_time;


	float start_x;	// initial position of CABIN at scene
	float start_y;

	bool isBodyUp;// sung huong doc
	bool isBodyStraight; // sung huong ngang

	float y_delta;
	bool isFiring; // dang bắn

public:

	CBarrel(float x = 0.0f, float y = 0.0f);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();

	void SetState(int state);
	void SetLevel(int l) { level = l; }
	void StartUpEffect() { y -= 1; up_effect_start = GetTickCount(); }

	void Reset();

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);

	bool GetIsBodyUp() { return isBodyUp; }
	bool GetIsBodyStraight() { return isBodyStraight; }

};