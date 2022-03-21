#include <algorithm>
#include <assert.h>
#include "Utils.h"

#include "CabinObject.h"
#include "Game.h"

#include "Goomba.h"
#include "Portal.h"

CCabinObject::CCabinObject(float x, float y) : CGameObject()
{
	
	SetState(MAIN_CHARACTER_STATE_IDLE);

	start_x = x;
	start_y = y;
	this->x = x;
	this->y = y;
}

void CCabinObject::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (vx != 0) //Add effect if running
	{
		StartUpEffect();
		if (GetTickCount() - up_effect_start > WHEEL_EFFECT_TIME)
		{
			y += 1;
		}


	}
	if (nx < 0) //Nhân vật quay sang trái
	{
		x += 7;
		if(state == MAIN_CHARACTER_STATE_UP_BARREL) // Nhân vật đưa nòng súng lên
			y -= 2;
	}
	else if (state == MAIN_CHARACTER_STATE_UP_BARREL) // Nhân vật đưa nòng súng lên
	{
		y -= 2;
	}
}

void CCabinObject::Render()
{
	int alpha = 255;
	if (untouchable)
		alpha = 128;
	bool flip = false;
	int ani = -1;
	if (nx > 0)
	{
		flip = true;
	}
	else
		flip = false;
	if (state == MAIN_CHARACTER_STATE_UP_BARREL)
	{
		ani = CABIN_ANI_UP;
	}
	else
		ani = CABIN_ANI_HORIZONTAL;
	animation_set->at(ani)->Render(x, y,flip,alpha);
}

void CCabinObject::SetState(int state)
{
	CGameObject::SetState(state);
}

void CCabinObject::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{

	
}

/*
	Reset CABIN status to the beginning state of a scene
*/
void CCabinObject::Reset()
{
	
}

