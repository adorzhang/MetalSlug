#pragma once
#include "./GameObject.h"
#include "./MainCharacter.h"

#define LEG_ANI_IDLE			0
#define LEG_ANI_LIE				1
#define LEG_ANI_WALK			2
#define LEG_ANI_RUN				3
#define LEG_ANI_JUMP			4
#define LEG_ANI_SWIM			5
#define LEG_ANI_JUMP_STRAIGHT	6


#define LEG_BBOX_WIDTH 30
#define LEG_BBOX_HEIGHT 32

class CLegs : public CGameObject
{
	int level;
	float start_x;			// initial position of legs at scene
	float start_y;

	bool is_being_up;

	float x_delta;
	float y_delta;


	bool isEnable;
	bool isDisplay;
public:
	CLegs(float x = 0.0f, float y = 0.0f);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();

	void SetState(int state);
	void SetLevel(int l) { level = l; }

	void Reset();

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};