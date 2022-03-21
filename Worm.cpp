#include "Worm.h"
#include "Brick.h"
#include "Jumper.h"
CWorm::CWorm(int _item) :CEnemyObject()
{
	SetState(WORM_STATE_MOVE);
	this->blood = 1;
	item = _item;
	this->isEnable = true;
	this->isDisplay = true;
}

void CWorm::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (isEnable)
	{
		left = x;
		if (state != STATE_ITEM)
		{
			top = y - WORM_BBOX_HEIGHT;
			right = x + WORM_BBOX_WIDTH;
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

void CWorm::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
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
		if(item>0)
			SetState(STATE_ITEM);
		else
			SetState(WORM_STATE_DIE);
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
					SetState(WORM_STATE_MOVE);
					startMoving = true;
				}
				if (startMoving)
				{
					if (e->nx != 0)
					{
						SetState(WORM_STATE_JUMP);
					}
				}
				
				

			}
			else if (dynamic_cast<CWorm*>(e->obj))
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
						y -= vyJumper*dt;
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

void CWorm::Render()
{
	if (isEnable)
	{
		int ani = -1;
		switch (state)
		{
		case WORM_STATE_IDLE:
		case WORM_STATE_JUMP:
		case WORM_STATE_MOVE:
			if (nx > 0)
				ani = WORM_ANI_MOVE_RIGHT;
			else
				ani = WORM_ANI_MOVE_LEFT;
			break;
		case STATE_ITEM:
			if (hasItem)
			{
				ani = item;
				animation_item_set->at(ani-1)->Render(x, y);
			}
			
			break;
		}
		if (isDisplay)
		{
			animation_set->at(ani)->Render(x, y);
			RenderBoundingBox();
		}
			
	}

}

void CWorm::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case WORM_STATE_IDLE:
		vx = 0;
		break;
	case WORM_STATE_MOVE:
		if (nx > 0)
		{
			vx = WORM_MOVE_SPEED;
		}

		else
		{
			vx = -WORM_MOVE_SPEED;
		}
		break;
	case WORM_STATE_JUMP:
		vy = 0.02f;
		if (nx > 0)
		{
			vx = WORM_MOVE_SPEED;
		}
		else
		{
			vx = -WORM_MOVE_SPEED;
		}
		break;
	case WORM_STATE_DIE:
		isDisplay = false;
		isEnable = false;
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