#include "INSECT.h"
#include "Brick.h"
#include "Worm.h"
#include "Dome.h"
CInsect::CInsect(int _item) :CEnemyObject()
{
	SetState(INSECT_STATE_MOVE);
	this->blood = 1;
	item = _item;
	time_moving = 0;
	isEnable = true;
	isDisplay = true;


}

void CInsect::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (isEnable)
	{
		left = x;
		if (state != STATE_ITEM)
		{
			top = y - INSECT_BBOX_HEIGHT;
			right = x + INSECT_BBOX_WIDTH;
		}
		else
		{
			if (item == 1)
			{
				top = y - ITEM_P_BBOX_HEIGHT;

				right = x + ITEM_P_BBOX_WIDTH;
			}

		}
		bottom = y;
	}

}

void CInsect::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	time_moving += dt;
	time_change += dt;
	if (!isMovingRight)
	{
		if (!isBeingUp)
		{
			if (time_moving > TIME_CHANGE_DIRECTION_Y)
			{
				if (time_change < TIME_CHANGE_DIRECTION)
				{
					SetState(INSECT_STATE_MOVE_UP_RIGHT);
					isBeingUp = true;
					time_moving = 0;
				}
				else
				{
					SetState(INSECT_STATE_MOVE_UP_RIGHT);
					isBeingUp = true;
					time_moving = 0;
					isMovingRight = true;
					time_change = 0;
				}
			}


		}
		else
		{
			if (time_moving > TIME_CHANGE_DIRECTION_Y)
			{
				if (time_change < TIME_CHANGE_DIRECTION)
				{
					SetState(INSECT_STATE_MOVE_DIRECTION_Y_RIGHT);
					isBeingUp = false;
					time_moving = 0;
				}
				else
				{
					SetState(INSECT_STATE_MOVE_DIRECTION_Y_LEFT);
					isBeingUp = false;
					time_moving = 0;
					time_change = 0;
					isMovingRight = true;
				}
			}
		}
	}
	else
	{
		if (!isBeingUp)
		{
			if (time_moving > TIME_CHANGE_DIRECTION_Y)
			{
				if (time_change < TIME_CHANGE_DIRECTION)
				{
					SetState(INSECT_STATE_MOVE_UP_LEFT);
					isBeingUp = true;
					time_moving = 0;
				}
				else
				{
					SetState(INSECT_STATE_MOVE_UP_LEFT);
					isBeingUp = true;
					time_moving = 0;
					isMovingRight = false;
					time_change = 0;
				}
			}


		}
		else
		{
			if (time_moving > TIME_CHANGE_DIRECTION_Y)
			{
				if (time_change < TIME_CHANGE_DIRECTION)
				{
					SetState(INSECT_STATE_MOVE_DIRECTION_Y_LEFT);
					isBeingUp = false;
					time_moving = 0;
				}
				else
				{
					SetState(INSECT_STATE_MOVE_DIRECTION_Y_RIGHT);
					isBeingUp = false;
					time_moving = 0;
					time_change = 0;
					isMovingRight = false;
				}
			}
		}
	}

	// Calculate dx, dy 
	CGameObject::Update(dt);
	// Simple fall down
	//if (isDisplay)
		//vy -= 0.001f * dt;
	if (this->blood < 0)
	{
		if (item > 0)
			SetState(STATE_ITEM);
		else
			SetState(INSECT_STATE_DIE);
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

		/*for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (dynamic_cast<CBrick*>(e->obj))
			{
				if (e->nx > 0) {
					SetState(INSECT_STATE_MOVE_RIGHT);
				}
				else if(e->nx <0)
				{
					SetState(INSECT_STATE_MOVE_LEFT);
				}

			}
			else if (dynamic_cast<CWorm*>(e->obj))
			{
				if (e->nx > 0) {
					SetState(INSECT_STATE_MOVE_RIGHT);
				}
				else if (e->nx <0)
				{
					SetState(INSECT_STATE_MOVE_LEFT);
				}
			}
			else if (dynamic_cast<CDome*>(e->obj))
			{
				if (e->nx > 0) {
					SetState(INSECT_STATE_MOVE_RIGHT);
				}
				else if (e->nx < 0)
				{
					SetState(INSECT_STATE_MOVE_LEFT);
				}
			}


		}*/

	}
	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];


}

void CInsect::Render()
{
	if (isEnable)
	{

		int ani = -1;
		switch (state)
		{


		case INSECT_STATE_IDLE:
		case INSECT_STATE_MOVE:
			if (vx > 0 || nx > 0)
				ani = INSECT_ANI_MOVE_RIGHT;
			else if (vx < 0 || nx < 0)
				ani = INSECT_ANI_MOVE_LEFT;
			break;

		case INSECT_STATE_MOVE_DIRECTION_Y_RIGHT:

			ani = INSECT_ANI_MOVE_RIGHT;
			break;
		case INSECT_STATE_MOVE_DIRECTION_Y_LEFT:
			ani = INSECT_ANI_MOVE_LEFT;
			break;

		case INSECT_STATE_MOVE_UP_RIGHT:

			ani = INSECT_ANI_MOVE_RIGHT;
			break;
		case INSECT_STATE_MOVE_UP_LEFT:
			ani = INSECT_ANI_MOVE_LEFT;
			break;

		case STATE_ITEM:
			ani = item;
			animation_item_set->at(ani - 1)->Render(x, y);
			break;
		}
		if (isDisplay)
		{
			animation_set->at(ani)->Render(x, y);
			RenderBoundingBox();
		}

	}

}

void CInsect::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case INSECT_STATE_IDLE:
		vx = 0;
		break;
	case INSECT_STATE_MOVE:
		if (ny > 0)
		{
			vy = INSECT_MOVE_SPEED;
		}
		else
		{
			vy = -INSECT_MOVE_SPEED;
		}
		vx = INSECT_MOVE_SPEED;

		break;

	case INSECT_STATE_MOVE_DIRECTION_Y_RIGHT:
		vy = -INSECT_MOVE_SPEED;
		vx = INSECT_MOVE_SPEED;
		break;
	case INSECT_STATE_MOVE_DIRECTION_Y_LEFT:
		vy = -INSECT_MOVE_SPEED;
		vx = -INSECT_MOVE_SPEED;
		break;
	case INSECT_STATE_MOVE_UP_RIGHT:
		vx = 0;
		vy = INSECT_MOVE_SPEED;
		break;
	case INSECT_STATE_MOVE_UP_LEFT:
		vx = 0;
		vy = INSECT_MOVE_SPEED;
		break;
	case INSECT_STATE_DIE:
		isDisplay = false;
		isEnable = false;
		break;

	case STATE_ITEM:
		vx = 0;
		vy = 0;
		isDisplay = false;
		break;
	default:
		break;
	}
}