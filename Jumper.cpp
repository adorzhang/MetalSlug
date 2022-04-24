#include "Jumper.h"
#include "Brick.h"
#include "Worm.h"
CJumper::CJumper(int _item) :CEnemyObject()
{
	SetState(JUMPER_STATE_IDLE);
	this->blood = 1;
	item = _item;
	time_moving = 0;
	isEnable = true;
	isDisplay = true;

}

void CJumper::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (isEnable)
	{
		left = x;
		if (state != STATE_ITEM)
		{
			top = y - JUMPER_BBOX_HEIGHT;
			right = x + JUMPER_BBOX_WIDTH;
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

void CJumper::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{

	// Calculate dx, dy 
	CGameObject::Update(dt);
	// Simple fall down
	if (isDisplay)
		vy -= 0.001f * dt;
	if (this->blood < 0)
	{
		if (item > 0)
			SetState(STATE_ITEM);
		else
			SetState(JUMPER_STATE_DIE);
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
					isOnGround = true;
					if (isOnGround)
					{
						if (!isJumping)
						{
							time_moving += dt;
							if (!isMovingRight)
							{
								if (time_moving > TIME_CHANGE_DIRECTION)
								{
									SetState(JUMPER_STATE_MOVE_LEFT);
									isMovingRight = true;
									time_moving = 0;
								}
							}
							else
							{
								if (time_moving > TIME_CHANGE_DIRECTION)
								{
									SetState(JUMPER_STATE_MOVE_RIGHT);
									isMovingRight = false;
									time_moving = 0;
								}
							}
						}
						
					}	
				}
			}
			
		}

	}
	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];


}

void CJumper::Render()
{
	if (isEnable)
	{

		int ani = -1;
		bool flip = false;
		switch (state)
		{
		case JUMPER_STATE_MOVE_RIGHT:
			ani = JUMPER_ANI_MOVE_RIGHT;
			animation_set->at(ani)->isPause = false;
			break;
		case JUMPER_STATE_MOVE_LEFT:
			ani = JUMPER_ANI_MOVE_LEFT;
			animation_set->at(ani)->isPause = false;
			break;
		case JUMPER_STATE_IDLE:
			ani = JUMPER_ANI_MOVE_LEFT;
			animation_set->at(ani)->isPause=true;
			break;
		case JUMPER_STATE_MOVE:
			if (nx > 0)
				ani = JUMPER_ANI_MOVE_RIGHT;
			else
				ani = JUMPER_ANI_MOVE_LEFT;
			animation_set->at(ani)->isPause = true;
			break;
		case JUMPER_STATE_JUMP:
			if (nx > 0)
				ani = JUMPER_ANI_MOVE_RIGHT;
			else
				ani = JUMPER_ANI_MOVE_LEFT;
			break;
		case STATE_ITEM:
			ani = item;
			animation_item_set->at(ani - 1)->Render(x, y);
			break;
		}
		if (isDisplay)
		{
			animation_set->at(ani)->Render(x, y);
			//RenderBoundingBox();
		}

	}

}

void CJumper::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case JUMPER_STATE_IDLE:
		vx = 0;
		break;
	case JUMPER_STATE_MOVE:
		if (nx > 0)
		{
			vx = JUMPER_MOVE_SPEED;
		}
		else
		{
			vx = -JUMPER_MOVE_SPEED;
		}
		break;
	case JUMPER_STATE_MOVE_RIGHT:
		vx = JUMPER_MOVE_SPEED;
		break;
	case JUMPER_STATE_MOVE_LEFT:
		vx = -JUMPER_MOVE_SPEED;
		break;
	case JUMPER_STATE_JUMP:
		if (nx > 0)
		{
			vx = JUMPER_MOVE_SPEED;
		}
		else
		{
			vx = -JUMPER_MOVE_SPEED;
		}
		if (isOnGround)
		{
			vy = 0.2;
			isOnGround = false;
		}
		break;
	case JUMPER_STATE_DIE:
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