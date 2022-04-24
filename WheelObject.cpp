#include <algorithm>
#include <assert.h>
#include "Utils.h"

#include "WheelObject.h"
#include "Game.h"

#include "Goomba.h"
#include "Portal.h"

CWheelObject::CWheelObject(float x, float y) : CGameObject()
{
	
	SetState(MAIN_CHARACTER_STATE_IDLE);

	start_x = x;
	start_y = y;
	this->x = x;
	this->y = y;

	is_Right_Wheel = false;
	is_Middle_Wheel = false;
	x_delta = 0;
	push_effect_time = 0;
}

void CWheelObject::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);
	x = player_x;
	y = player_y;
	//Hiệu ứng bánh xe lên xuống khi di chuyển
	if (vx != 0)
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
	if (is_start_push_effect&&is_being_up && is_Right_Wheel)
	{
		push_effect_time += dt;
		if(push_effect_time<=WHEEL_PUSH_EFFECT_TIME&&!is_end_push_effect)
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
	}
	
	else
	{
		push_effect_time = 0;
		x_delta = 0;
	}
		
}

void CWheelObject::Render()
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
	
	if (is_Right_Wheel) // Bánh xe bên phải
	{
		
		ani = WHEEL_ANI_RIGHT_WHEEL; //Animation chỉ có 1 frame duy nhất
		if (vx == 0) // Nhân vật đứng yên
		{
			animation_set->at(ani)->isPause = true; //Dừng animation 
			animation_set->at(ani)->Render(x+MAIN_CHARACTER_BBOX_WIDTH- WHEEL_BBOX_WIDTH+x_delta, y- MAIN_CHARACTER_BBOX_HEIGHT+ WHEEL_BBOX_HEIGHT, flip,alpha); // Vẽ frame đang bị tạm dừng
		}
		else // Nhân vật di chuyển
		{
			animation_set->at(ani)->isPause = false; // Tiếp tục animation đã dừng trước đó
			animation_set->at(ani)->Render(x + MAIN_CHARACTER_BBOX_WIDTH - WHEEL_BBOX_WIDTH + x_delta, y - MAIN_CHARACTER_BBOX_HEIGHT + WHEEL_BBOX_HEIGHT+y_delta, flip,alpha);
		}
	}
	else //Bánh xe bên trái
	{
			ani = WHEEL_ANI_IDLE_RUN;
			if (vx==0) // Nhân vật đứng yên
			{
				animation_set->at(ani)->isPause = true; //Dừng animation 
				animation_set->at(ani)->Render(x + x_delta, y - MAIN_CHARACTER_BBOX_HEIGHT + WHEEL_BBOX_HEIGHT, flip,alpha); // Vẽ frame đang bị tạm dừng
			}
			else // Nhân vật di chuyển
			{
				animation_set->at(ani)->isPause = false; // Tiếp tục animation đã dừng trước đó
				animation_set->at(ani)->Render(x + x_delta, y - MAIN_CHARACTER_BBOX_HEIGHT + WHEEL_BBOX_HEIGHT+y_delta, flip,alpha);
			}	
	}
		
}

void CWheelObject::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case MAIN_CHARACTER_STATE_UP_BARREL:
		is_start_push_effect = true;
		is_being_up = true;
		break;
	default:
		is_start_push_effect = false;
		is_being_up = false;
		is_end_push_effect = false;
		break;
	}
}

void CWheelObject::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{


}

/*
	Reset WHEEL status to the beginning state of a scene
*/
void CWheelObject::Reset()
{
	
}
// Thiết lập đây là bánh xe nằm bên phải
void CWheelObject::SetIsRightWheel()
{
	is_Right_Wheel = true; 
}
// Thiết lập đây là bánh xe nằm ở giữa
void CWheelObject::SetIsMiddleWheel()
{
	is_Middle_Wheel = true;
}