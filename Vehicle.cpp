#include <algorithm>
#include <assert.h>
#include "Utils.h"

#include "Vehicle.h"
#include "Game.h"

#include "Goomba.h"
#include "Portal.h"

CVehicle::CVehicle(float x, float y) : CGameObject() 
{

	vehicle_nx = nx;
	SetState(MAIN_CHARACTER_STATE_IDLE);

	start_x = x;
	start_y = y;

	this->x = x;
	this->y = y;
	y_delta = 0;
}

void CVehicle::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	// Calculate dx, dy 
	CGameObject::Update(dt);
	this->dt = dt;
	x = player_x;
	y = player_y;
	if (is_barrel_up&&!isBarrelStraight&&!is_firing)
		y_delta = 15 ;
	else if (isBarrelStraight)
		y_delta = y_delta;
	else
		y_delta = 0;
	//Hiệu ứng cabin lên xuống khi di chuyển
	if (vx != 0)
	{
		up_down_effect_time += dt;
		if (!is_being_up_effect_cabin)
		{
			if (up_down_effect_time <= VEHICLE_UP_DOWN_EFFECT_TIME)
			{
				y_delta += 0.02 * dt;
			}
			else
			{
				is_being_up_effect_cabin = true;
				up_down_effect_time = 0;
			}
		}
		else
		{
			if (up_down_effect_time <= VEHICLE_UP_DOWN_EFFECT_TIME)
			{
				y_delta -= 0.02 * dt;
			}
			else
			{
				is_being_up_effect_cabin = false;
				up_down_effect_time = 0;
			}
		}

	}
}

void CVehicle::Render()
{
	
	int alpha = 255;
	if (untouchable)
		alpha = 128;
	bool flip = false;
	int ani = -1;
	if (nx > 0)
	{
		flip = false;
	}
	else
		flip = true;
	if (isStateOpenCabin)
	{
		ani = VEHICLE_ANI_MO_CABIN;
		animation_set->at(ani)->Render(x, y+10, flip, alpha);
		if (animation_set->at(ani)->isFinish)
		{
			animation_set->at(ani)->isFinish = false;
			isCabinOpened = true;
		}
	}
	else
	{
		isCabinOpened = false;
		ani = VEHICLE_ANI_MOVE;
		switch (state)
		{
			case MAIN_CHARACTER_STATE_STRAIGHT_BARREL:
			case MAIN_CHARACTER_STATE_UP_BARREL:
			{
				ani = VEHICLE_ANI_NONG_SUNG;
				animation_set->at(ani)->isRepeat = false;
				is_barrel_up = true;//flag to determine suitable animation
				if (animation_set->at(ani)->isFinish)
					isBarrelStraight = true;
				else
				{
					isBarrelStraight = false;
				}
				if (nx > 0)
					animation_set->at(ani)->Render(x - 4, y + y_delta, flip, alpha);
				else
					animation_set->at(ani)->Render(x + 8, y + y_delta, flip, alpha);
			}
			break;
			default:
			{
				animation_set->at(VEHICLE_ANI_NONG_SUNG)->isFinish = false;
				ani = VEHICLE_ANI_MOVE;
				animation_set->at(VEHICLE_ANI_NONG_SUNG)->SetCurrentFrame(-1);
				animation_set->at(VEHICLE_ANI_NONG_SUNG)->isRepeat = true;
				is_barrel_up = false;
				isBarrelStraight = false;
				if (vehicle_nx != nx)
				{
					int _ani = VEHICLE_ANI_XOAY;
					animation_set->at(_ani)->Render(x, y+2, flip, alpha);
					if (animation_set->at(_ani)->isFinish)
					{
						animation_set->at(ani)->Render(x, y+2, flip, alpha);
						vehicle_nx = nx;
						animation_set->at(_ani)->isFinish = false;
					}
				}
				else
				{
					animation_set->at(ani)->Render(x, y+2+y_delta, flip, alpha);

				}
			}
			break;
		}
	}
}

void CVehicle::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case MAIN_CHARACTER_STATE_IDLE:
	case MAIN_CHARACTER_STATE_RUN_LEFT:
	case MAIN_CHARACTER_STATE_RUN_RIGHT:
		is_barrel_up = false;
		isBarrelStraight = false;
		is_firing = false;
		break;
	case MAIN_CHARACTER_STATE_BARREL_FIRE:
		is_firing = true;
		break;
	case MAIN_CHARACTER_STATE_STRAIGHT_BARREL:
	case MAIN_CHARACTER_STATE_UP_BARREL:
		is_barrel_up = true;
		vy = 0.04;
		is_firing = false;
		break;
	case MAIN_CHARACTER_STATE_OPEN_CABIN:
		isStateOpenCabin = true;
		break;
	case MAIN_CHARACTER_STATE_CLOSE_CABIN:
		isStateOpenCabin = false;
		isCabinOpened = false;
		break;
	default:
		is_firing = false;
		break;
	}
}

void CVehicle::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{


}

/*
	Reset CABIN status to the beginning state of a scene
*/
void CVehicle::Reset()
{

}

