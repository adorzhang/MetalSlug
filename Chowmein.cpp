#include "Chowmein.h"
#include "Brick.h"
#include "Jumper.h"
#include "MainCharacter.h"
#include "Utils.h"
CChowmein::CChowmein(int _item) :CEnemyObject()
{
	SetState(CHOWMEIN_STATE_WALKING);
	currentAni = CHOWMEIN_ANI_MOVE_LEFT;
	this->blood = 1;
	item = _item;
	this->isEnable = true;
	this->isDisplay = true;
	isPlaying = true;
}

void CChowmein::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (isEnable)
	{
		left = x;
		top = y - CHOWMEIN_BBOX_HEIGHT;
		right = x + CHOWMEIN_BBOX_WIDTH;
		bottom = y;
	}

}

void CChowmein::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	// Calculate dx, dy 
	CGameObject::Update(dt);
	if (x <= 0 && vx < 0)
		dx = 0;
	// Simple fall down
	if (isDisplay)
		vy -= 0.0001f * dt;
	if (this->blood < 0)
	{
		if (item > 0)
			SetState(STATE_ITEM);
		else
			SetState(CHOWMEIN_STATE_DIE);
	}
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();
	if (isDisplay)
		CalcPotentialCollisions(coObjects, coEvents);
	// No collision occured, proceed normally
	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;

		// TODO: This is a very ugly designed function!!!!
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		// block every object first!
		x += min_tx * dx + nx * 0.4f;
		y += min_ty * dy + ny * 0.4f;

		if (nx != 0) vx = 0;
		if (ny != 0) vy = 0;
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (dynamic_cast<CBrick*>(e->obj))
			{
				if (e->ny > 0)
				{
					if (!isPlaying)SetState(CHOWMEIN_STATE_WALKING);
					startMoving = true;
				}
				if (startMoving)
				{
					if (e->nx != 0)
					{
						SetState(CHOWMEIN_STATE_JUMP);
					}
				}
			}
			else if (dynamic_cast<CChowmein*>(e->obj))
			{
				x += dx;
			}
			else if (dynamic_cast<CJumper*>(e->obj))
			{
				CJumper* jumper = dynamic_cast<CJumper*>(e->obj);
				if (jumper->GetState() != STATE_ITEM)
				{
					float vxJumper, vyJumper;
					jumper->GetSpeed(vxJumper, vyJumper);
					if (e->ny != 0)
					{
						y -= vyJumper * dt;
					}
					else
						x += dx;
				}
			}


		}
	}
	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CChowmein::Render()
{
	if (animation_set->at(currentAni)->isFinish) {
		isPlaying = false;
		if (currentAni == CHOWMEIN_ANI_ATTACK) {
			SetState(CHOWMEIN_STATE_WALKING);
		}
		DebugOut(L"[INFO] stop playing : \n");
	}
	else {
		DebugOut(L"[INFO] start playing : \n");
		isPlaying = true;
	}
	bool flip = false;
	if (nx > 0)
	{
		flip = true;
	}
	else
		flip = false;

	if (isEnable)
	{
		int ani = -1;

		switch (state)
		{
		case CHOWMEIN_STATE_IDLE: {
			ani = CHOWMEIN_ANI_IDLE;
			animation_set->at(ani)->isRepeat = true;
			animation_set->at(ani)->isFinish = false;
			animation_set->at(ani)->Render(x, y, flip);
			currentAni = ani;

		}break;
		case CHOWMEIN_STATE_JUMP: {
			ani = CHOWMEIN_ANI_JUMP;
			animation_set->at(ani)->isRepeat = true;
			animation_set->at(ani)->isFinish = false;
			animation_set->at(ani)->Render(x, y, flip);
			currentAni = ani;
		}break;
		case CHOWMEIN_STATE_WALKING: {
			if (nx > 0)
				ani = CHOWMEIN_ANI_MOVE_RIGHT;
			else
				ani = CHOWMEIN_ANI_MOVE_LEFT;

			animation_set->at(ani)->isRepeat = true;
			animation_set->at(ani)->isFinish = false;
			animation_set->at(ani)->Render(x, y, flip);
			currentAni = ani;
		}break;
		case STATE_ITEM:
			if (hasItem)
			{
				ani = item;
				animation_item_set->at(ani - 1)->Render(x, y);
				currentAni = ani;
			}

			break;
		case CHOWMEIN_STATE_ATTACK: {
			ani = CHOWMEIN_ANI_ATTACK;
			animation_set->at(ani)->isRepeat = true;
			animation_set->at(ani)->isFinish = false;

			animation_set->at(ani)->Render(x, y, flip);

			currentAni = ani;

		}
								  break;
		}

		if (isDisplay)
		{

			//if (animation_set->at(ani)->isFinish) {
			//animation_set->at(CHOWMEIN_ANI_MOVE_LEFT)->Render(x, y, flip);

			RenderBoundingBox();
		}

	}

}

void CChowmein::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case CHOWMEIN_STATE_IDLE:
		vx = 0;
		break;
	case CHOWMEIN_STATE_WALKING:
		if (nx > 0)
		{
			vx = CHOWMEIN_MOVE_SPEED;
		}

		else
		{
			vx = -CHOWMEIN_MOVE_SPEED;
		}
		break;
	case CHOWMEIN_STATE_JUMP:
		vy = 0.02f;
		if (nx > 0)
		{
			vx = CHOWMEIN_MOVE_SPEED;
		}
		else
		{
			vx = -CHOWMEIN_MOVE_SPEED;
		}
		break;

	case CHOWMEIN_STATE_DIE:
		isDisplay = false;
		isEnable = false;
		break;
	case CHOWMEIN_STATE_ATTACK:
		break;
	case STATE_ITEM:
		hasItem = true;
		vy = 0;
		vx = 0;
		isDisplay = false;
		break;
	default:
		break;
	}
}