#include <algorithm>
#include <assert.h>
#include "Utils.h"

#include "BarrelObject.h"
#include "Game.h"

#include "Goomba.h"
#include "Portal.h"

CBarrelObject::CBarrelObject(float x, float y) : CGameObject()
{
	
	SetState(MAIN_CHARACTER_STATE_IDLE);
	this->is_finish_animation = true;
	start_x = x;
	start_y = y;
	this->x = x;
	this->y = y;
}

void CBarrelObject::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	
	
	if (state == MAIN_CHARACTER_STATE_UP_BARREL)//nhân vật ở trạng thái đưa nòng súng lên
	{
		
		if (nx > 0)// Xét quay sang phải
		{
			x += (MAIN_CHARACTER_BBOX_WIDTH / 2) - 2;//nằm giữa width của main character
			y += 6;
		}
		else //Xét quay sang trái
		{
			x += (MAIN_CHARACTER_BBOX_WIDTH / 2) - 6;//nằm giữa width của main character
			y += 6;
		}
	}
	else if (nx > 0)// nhân vật quay sang phải
	{
		x += 16; // canh tọa độ x đặt nòng súng phù hợp
		y += 1; // canh tọa độ y đặt nòng súng phù hợp
		if (vx != 0) //Add effect if running
		{
			StartUpEffect();
			if (GetTickCount() - up_effect_start > WHEEL_EFFECT_TIME)
			{
				y += 1;
			}


		}
	}
	else // nhân vật quay sang trái
	{
		y += 1;// canh tọa độ y đặt nòng súng phù hợp
		if (vx != 0) //Add effect if running
		{
			StartUpEffect();
			if (GetTickCount() - up_effect_start > WHEEL_EFFECT_TIME)
			{
				y -= 1;
			}


		}
	}
		
}

void CBarrelObject::Render()
{
	int alpha = 255;
	if (untouchable)
		alpha = 128;
	bool flip = false;
	int ani = 0;//default
	if (nx > 0)
	{
		flip = true;
	}
	else
		flip = false;

	switch (state)
	{

	case MAIN_CHARACTER_STATE_UP_BARREL:
		ani = BARREL_ANI_UP;
		animation_set->at(ani)->isRepeat = false;
		is_barrel_up = true;//flag to determine suitable animation
		break;
	default:
		ani = 0;
		animation_set->at(BARREL_ANI_UP)->SetCurrentFrame(-1);
		animation_set->at(BARREL_ANI_UP)->isRepeat = true;
		is_barrel_up = false;
	}
	

	
	
	animation_set->at(ani)->Render(x, y, flip,alpha);
	
}

void CBarrelObject::SetState(int state)
{
	CGameObject::SetState(state);
	
	
}

void CBarrelObject::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{

	
}

/*
	Reset BARREL status to the beginning state of a scene
*/
void CBarrelObject::Reset()
{
	
}

