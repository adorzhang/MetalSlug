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
	x_delta = 0;

	this->isEnable = true;
	this->isDisplay = true;
}

void CLegs::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);
	x = player_x;
	y = player_y;

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
		animation_set->at(ani)->Render(x, y - MAIN_CHARACTER_BBOX_HEIGHT, 255, flip, 1, 0);
	}
	else // Nhân vật di chuyển
	{
		//animation_set->at(ani)->isPause = false; // Tiếp tục animation đã dừng trước đó
		animation_set->at(ani)->isRepeat = true;
		animation_set->at(ani)->Render(x , y - MAIN_CHARACTER_BBOX_HEIGHT  + y_delta, 255, flip, 1, 0);
	}
	RenderBoundingBox();
}

void CLegs::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case MAIN_CHARACTER_STATE_UP_BARREL:
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
		break;
	}
}

void CLegs::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (isEnable)
	{
		left = x;
		top = y - LEG_BBOX_HEIGHT;
		right = x + LEG_BBOX_WIDTH;
		bottom = y;
	}

}


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