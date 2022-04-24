#include <algorithm>
#include <assert.h>
#include "Utils.h"

#include "Legs.h"
#include "Game.h"

#include "Goomba.h"
#include "Portal.h"

CLegs::CLegs(float x, float y) : CGameObject()
{

	SetState(MAIN_CHARACTER_STATE_IDLE);

	start_x = x;
	start_y = y;
	this->x = x;
	this->y = y;

	//is_Right_Wheel = false;
	//is_Middle_Wheel = false;
	x_delta = 0;
	//push_effect_time = 0;
}

void CLegs::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);
	x = player_x;
	y = player_y;
	//Hiệu ứng bánh xe lên xuống khi di chuyển
	/*if (vx != 0)
	{
		up_down_effect_time += dt;
		if (!is_being_up_effect_wheel)
		{
			if (up_down_effect_time <= WHEEL_UP_DOWN_EFFECT_TIME)
			{
				y_delta += 0.01 * dt;
			}
			else
			{
				is_being_up_effect_wheel = true;
				up_down_effect_time = 0;
			}
		}
		else
		{
			if (up_down_effect_time <= WHEEL_UP_DOWN_EFFECT_TIME)
			{
				y_delta -= 0.01 * dt;
			}
			else
			{
				is_being_up_effect_wheel = false;
				up_down_effect_time = 0;
			}
		}

	}
	if (is_start_push_effect && is_being_up && is_Right_Wheel)
	{
		push_effect_time += dt;
		if (push_effect_time <= WHEEL_PUSH_EFFECT_TIME && !is_end_push_effect)
			x_delta -= 0.01 * dt;
		else
		{
			is_end_push_effect = true;
			push_effect_time = 0;
			x_delta = x_delta;
		}
	}

	else if (is_start_push_effect && is_being_up && !is_Right_Wheel)
	{
		push_effect_time += dt;
		if (push_effect_time <= WHEEL_PUSH_EFFECT_TIME && !is_end_push_effect)
			x_delta += 0.01 * dt;
		else
		{
			is_end_push_effect = true;
			push_effect_time = 0;
			x_delta = x_delta;
		}
	}*/

	//else
	{
		//push_effect_time = 0;
		//x_delta = 0;
	}

}

void CLegs::Render()
{
	int alpha = 255;
	if (untouchable)
		alpha = 128;
	int ani = -1;
	int flip = false;
	if (nx > 0)
	{
		flip = false;
	}
	else
		flip = true;

	switch (state)
	{
	case MAIN_CHARACTER_STATE_JUMP:
		ani = LEG_ANI_JUMP;
		break;
	case MAIN_CHARACTER_STATE_UP_BARREL:
		ani = LEG_ANI_IDLE;
		break;
	case MAIN_CHARACTER_STATE_JUMP_STRAIGHT:
		ani = LEG_ANI_JUMP_STRAIGHT;
		break;
	case MAIN_CHARACTER_STATE_RUN_RIGHT:
		ani = LEG_ANI_RUN;
		break;
	case MAIN_CHARACTER_STATE_RUN_LEFT:
		ani = LEG_ANI_RUN;
		break;
	case MAIN_CHARACTER_STATE_IDLE:
		ani = LEG_ANI_IDLE;
		break;
	case MAIN_CHARACTER_STATE_SWIM:
		ani = LEG_ANI_SWIM;
		break;
	default:
		ani = LEG_ANI_IDLE;
		break;
	}


	if (vx == 0) {
		//animation_set->at(ani)->isPause = true; //Dừng animation 
		animation_set->at(ani)->isRepeat = true;
		animation_set->at(ani)->Render(x, y - MAIN_CHARACTER_BBOX_HEIGHT, flip, alpha);
	}
	else // Nhân vật di chuyển
	{
		//animation_set->at(ani)->isPause = false; // Tiếp tục animation đã dừng trước đó
		animation_set->at(ani)->isRepeat = true;
		animation_set->at(ani)->Render(x , y - MAIN_CHARACTER_BBOX_HEIGHT  + y_delta, flip, alpha);
	}
	
}

void CLegs::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case MAIN_CHARACTER_STATE_UP_BARREL:
		is_start_push_effect = true;
		is_being_up = true;
		break;
	case MAIN_CHARACTER_STATE_JUMP:
		break;
	case MAIN_CHARACTER_STATE_RUN_RIGHT:
		break;
	case MAIN_CHARACTER_STATE_RUN_LEFT:
		break;
	case MAIN_CHARACTER_STATE_IDLE:
		break;
	case MAIN_CHARACTER_STATE_SWIM:
		break;
	default:
		is_start_push_effect = false;
		is_being_up = false;
		is_end_push_effect = false;
		break;
	}
}

void CLegs::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{


}

/*
	Reset WHEEL status to the beginning state of a scene
*/
void CLegs::Reset()
{

}
// Thiết lập đây là bánh xe nằm bên phải
/*void CLegs::SetIsRightWheel()
{
	is_Right_Wheel = true;
}*/
// Thiết lập đây là bánh xe nằm ở giữa
/*void CLegs::SetIsMiddleWheel()
{
	is_Middle_Wheel = true;
}*/