#include <algorithm>
#include <assert.h>
#include "Utils.h"

#include "Body.h"
#include "Game.h"

#include "Goomba.h"
#include "Portal.h"

CBody::CBody(float x, float y) : CGameObject()
{
	SetState(MAIN_CHARACTER_STATE_IDLE);

	start_x = x;
	start_y = y;

	this->x = x;
	this->y = y;
	y_delta = 0;
	ani = BODY_ANI_IDLE;
}

void CBody::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	// Calculate dx, dy 
	CGameObject::Update(dt);
	this->dt = dt;
	x = player_x;
	y = player_y;
	if (isBodyUp )
		y_delta = 15;
	else
		y_delta = 0;
	

}

void CBody::Render()
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
	case MAIN_CHARACTER_STATE_STRAIGHT_BARREL:
		break;
	case MAIN_CHARACTER_STATE_UP_BARREL:
	{
		ani = BODY_ANI_STARE;
		animation_set->at(ani)->isRepeat = false;
		isBodyUp = true;//flag to determine suitable animation
		if (animation_set->at(ani)->isFinish) {
			//ani = BODY_ANI_IDLE;
		}
		
		if (nx > 0)
			animation_set->at(ani)->Render(x + 5, y + y_delta - 10, flip, alpha);
		else
			animation_set->at(ani)->Render(x - 4, y + y_delta - 10, flip, alpha);
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
		//animation_set->at(ani)->isFinish = false;
		animation_set->at(ani)->SetCurrentFrame(-1);
		animation_set->at(ani)->isRepeat = false;

		if (nx > 0)
			animation_set->at(ani)->Render(x, y + y_delta , flip, alpha);
		else
			animation_set->at(ani)->Render(x, y + y_delta, flip, alpha);
	}
	break;
	default:
	{
		if (animation_set->at(ani)->isFinish ) {
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
		
		isBodyUp = false;
		isBodyStraight = false;
		animation_set->at(ani)->Render(x, y + 2 + y_delta, flip, alpha);

	}
	break;
	}

}

void CBody::SetState(int state)
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

void CBody::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{


}


void CBody::Reset()
{

}

