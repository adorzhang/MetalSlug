#include "Dome.h"
#include "Brick.h"
CDome::CDome(int _item) :CEnemyObject()
{
	SetState(DOME_STATE_START);
	this->blood = 1;
	item = _item;
	isEnable = true;
	isDisplay = true;
}

void CDome::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (isEnable)
	{
		left = x;
		if (state != STATE_ITEM)
		{
			top = y - DOME_BBOX_HEIGHT;
			right = x + DOME_BBOX_WIDTH;
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

void CDome::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (!isEnable)
		return;
	// Calculate dx, dy 
	CGameObject::Update(dt);

	// Simple fall down
	if (isDisplay)
		vy -= 0.0001f * dt;
	if (this->blood < 0)
	{
		if (item > 0)
			SetState(STATE_ITEM);
		else
			SetState(DOME_STATE_DIE);
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
		if(!isMoving)
			SetState(DOME_STATE_LAY_UP);
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;

		// TODO: This is a very ugly designed function!!!!
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		// how to push back Mario if collides with a moving objects, what if Mario is pushed this way into another object?
		//if (rdx != 0 && rdx!=dx)
		//	x += nx*abs(rdx); 

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
					SetState(DOME_STATE_MOVE);
				}
				else if (e->nx < 0)
				{
					SetState(DOME_STATE_CLIMB);
				}
				
			}
			
		}
	}
	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

	
}

void CDome::Render()
{
	if (isEnable)
	{
		int ani = -1;
		switch (state)
		{
		case DOME_STATE_MOVE:
			if (nx > 0)
				ani = DOME_ANI_MOVE_RIGHT;
			else
				ani = DOME_ANI_MOVE_LEFT;
			break;
		case DOME_STATE_LAY_UP:
			ani = DOME_ANI_LAY_UP;
			break;
		case DOME_STATE_CLIMB:
			ani = DOME_ANI_CLIMB;
			break;
		case DOME_STATE_START:
			ani = DOME_ANI_CLIMB;
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

void CDome::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case DOME_STATE_START:
		vx = DOME_MOVE_SPEED;
		break;
	case DOME_STATE_MOVE:
		if (nx > 0)
		{
			vx = DOME_MOVE_SPEED;
		}
		else
		{
			vx = -DOME_MOVE_SPEED;
		}
		isMoving = true;
		break;
	case DOME_STATE_DIE:
		isDisplay = false;
		isEnable = false;
		break;
	case DOME_STATE_CLIMB:
		vx = DOME_MOVE_SPEED;
		isClimbing = true;
		break;
	case DOME_STATE_LAY_UP:
		isLayingUp = true;
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