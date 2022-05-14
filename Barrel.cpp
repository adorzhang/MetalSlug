#include <algorithm>
#include <assert.h>
#include "Utils.h"

#include "Barrel.h"
#include "Game.h"

#include "Goomba.h"
#include "Portal.h"

CBarrel::CBarrel(float x, float y) : CGameObject()
{
	SetState(MAIN_CHARACTER_STATE_IDLE);

	start_x = x;
	start_y = y;

	this->x = x;
	this->y = y;
	y_delta = 0;
	ani = BODY_ANI_IDLE;
}

void CBarrel::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	// Calculate dx, dy 
	CGameObject::Update(dt);
	this->dt = dt;
	x = player_x;
	y = player_y;
	if (isBodyUp)
		y_delta = 15;
	else
		y_delta = 0;
	

}

void CBarrel::Render()
{

	int alpha = 255;
	if (untouchable)
		alpha = 128;
	bool flip = false;
	//int ani = -1;
	if (nx > 0)
	{
		flip = false;
	}
	else
		flip = true;

	//
	switch (state)
	{
	case MAIN_CHARACTER_STATE_RUN_RIGHT:
	case MAIN_CHARACTER_STATE_RUN_LEFT:
		ani = BODY_ANI_IDLE;
		if (nx > 0)
			animation_set->at(ani)->Render(x, y + 2 + y_delta, 255, flip, 1, 0);
		else
			animation_set->at(ani)->Render(x, y + 2 + y_delta, 255, flip, 1, 0);
		break;
	case MAIN_CHARACTER_STATE_UP_BARREL:
	{
		//ani == BODY_ANI_SHOOT_UP;
		//if (ani == BODY_ANI_SHOOT_UP) {
			//if(animation_set->at(ani)->isFinish)
				//ani = BODY_ANI_STARE;
		//}else
			ani = BODY_ANI_STARE;

		animation_set->at(ani)->isRepeat = false;
		//isBodyUp = true;//flag to determine suitable animation
		
		if (nx > 0)
			animation_set->at(ani)->Render(x + 5, y + y_delta - 10, 255, flip, 1, 0);
		else
			animation_set->at(ani)->Render(x - 4, y + y_delta - 10, 255, flip, 1, 0);
		
	}
	break;
	case MAIN_CHARACTER_STATE_BARREL_FIRE:
	{
		if (isBodyUp) {
			ani = BODY_ANI_SHOOT_UP;
		}
		else {
			ani = BODY_ANI_SHOOT_STRAIGHT;
		}
		animation_set->at(ani)->isFinish = false;
		animation_set->at(ani)->SetCurrentFrame(-1);
		animation_set->at(ani)->isRepeat = true;

		if (ani == BODY_ANI_SHOOT_UP) {
			if (nx > 0)
				animation_set->at(ani)->Render(x, y + 2 + y_delta, 255, flip, 1, 0);
			else
				animation_set->at(ani)->Render(x, y + 2 + y_delta, 255, flip, 1, 0);
		}
		else {
			if (nx > 0)
				animation_set->at(ani)->Render(x, y + 2 + y_delta, 255, flip, 1, 0);
			else
				animation_set->at(ani)->Render(x, y + 2 + y_delta, 255, flip, 1, 0);
		}
	}
	break;
	default:
	{
		if (ani == BODY_ANI_STARE) {
			isBodyUp = false;
			isBodyStraight = false;
			animation_set->at(ani)->Render(x, y + 2 + y_delta, 255, flip, 1, 0);
			return;
		}
		else {
			if (animation_set->at(ani)->isFinish) {

				ani = BODY_ANI_IDLE;
				animation_set->at(BODY_ANI_STARE)->isFinish = false;
				animation_set->at(BODY_ANI_STARE)->SetCurrentFrame(-1);
				animation_set->at(BODY_ANI_STARE)->isRepeat = false;

				animation_set->at(BODY_ANI_SHOOT_UP)->isFinish = false;
				animation_set->at(BODY_ANI_SHOOT_UP)->SetCurrentFrame(-1);
				animation_set->at(BODY_ANI_SHOOT_UP)->isRepeat = false;

				animation_set->at(BODY_ANI_SHOOT_DOWN)->isFinish = false;
				animation_set->at(BODY_ANI_SHOOT_DOWN)->SetCurrentFrame(-1);
				animation_set->at(BODY_ANI_SHOOT_DOWN)->isRepeat = false;

				animation_set->at(BODY_ANI_SHOOT_STRAIGHT)->isFinish = false;
				animation_set->at(BODY_ANI_SHOOT_STRAIGHT)->SetCurrentFrame(-1);
				animation_set->at(BODY_ANI_SHOOT_STRAIGHT)->isRepeat = false;

				animation_set->at(BODY_ANI_KNIFE_HIT)->isFinish = false;
				animation_set->at(BODY_ANI_KNIFE_HIT)->SetCurrentFrame(-1);
				animation_set->at(BODY_ANI_KNIFE_HIT)->isRepeat = false;

			}
			
			if (ani == BODY_ANI_SHOOT_UP) {
				if (nx > 0)
					animation_set->at(ani)->Render(x , y + 2 + y_delta, 255, flip, 1, 0);
				else
					animation_set->at(ani)->Render(x, y + 2 + y_delta, 255, flip, 1, 0);
			}
			else {
				if (nx > 0)
					animation_set->at(ani)->Render(x, y + 2 + y_delta, 255, flip, 1, 0);
				else
					animation_set->at(ani)->Render(x-7, y + 2 + y_delta, 255, flip, 1, 0);
			}

			isBodyUp = false;
			isBodyStraight = false;
			//animation_set->at(ani)->Render(x, y + 2 + y_delta, flip, alpha);
		}
		
		
		

	}
	break;
	}

}

void CBarrel::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case MAIN_CHARACTER_STATE_IDLE:
		isBodyStraight = true;
		isBodyUp = false;
		break;
	case MAIN_CHARACTER_STATE_RUN_LEFT:
	case MAIN_CHARACTER_STATE_RUN_RIGHT:
		isBodyUp = false;
		isBodyStraight = false;
		isFiring = false;
		break;
	case MAIN_CHARACTER_STATE_BARREL_FIRE:
		isFiring = true;
		break;
	case MAIN_CHARACTER_STATE_STRAIGHT_BARREL:
		isBodyStraight = true;
		isBodyUp = false;
		break;
	case MAIN_CHARACTER_STATE_UP_BARREL:
		isBodyUp = true;
		isBodyStraight = false;
		vy = 0.08;
		isFiring = false;
		break;
	default:
		isFiring = false;
		break;
	}
}

void CBarrel::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{


}


void CBarrel::Reset()
{

}

