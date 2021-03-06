#include "Locust.h"
#include "Brick.h"
#include "Chowmein.h"
CLocust::CLocust(int _item, int _type) :CEnemyObject()
{
	SetState(LOCUST_STATE_FLYING);
	this->blood = 1;
	item = _item;
	time_moving = 0;
	isEnable = true;
	isDisplay = true;
	type = _type;

}

void CLocust::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (isEnable)
	{
		left = x;
		if (state != STATE_ITEM)
		{
			top = y - LOCUST_BBOX_HEIGHT;
			right = x + LOCUST_BBOX_WIDTH;
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

void CLocust::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{

	// Calculate dx, dy 
	CGameObject::Update(dt);
	// Simple fall down
	if (this->blood < 0)
	{
		if (item > 0)
			SetState(STATE_ITEM);
		else
			SetState(LOCUST_STATE_DIE);
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

		//if (nx != 0) vx = 0;
		if (ny != 0) vy = 0;

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (type == LOCUST_TYPE_NORMAL)
			{
				if (dynamic_cast<CBrick*>(e->obj))
				{
					if (e->nx != 0)
					{
						SetState(LOCUST_STATE_CHANGE_DIRECTION);
					}
				}
				else if (dynamic_cast<CLocust*>(e->obj))
				{
					float vxOrb, vyOrb;
					dynamic_cast<CLocust*>(e->obj)->GetSpeed(vxOrb, vyOrb);
					if (e->ny == 1)
					{
						y += dy;
					}
					else if (e->ny == -1)
					{
						y -= 4 * vyOrb * dt;
					}
					else
						x += dx;
				}
			}
			else if (type == LOCUST_TYPE_SPECIAL)
			{
				SetState(LOCUST_STATE_ATTACK);
				if (dynamic_cast<CLocust*>(e->obj))
				{
					float vxOrb, vyOrb;
					dynamic_cast<CLocust*>(e->obj)->GetSpeed(vxOrb, vyOrb);
					if (e->ny == 1)
					{
						y += dy;
					}
					else if (e->ny == -1)
					{
						y -= 4 * vyOrb * dt;
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

void CLocust::Render()
{
	if (isEnable)
	{

		int ani = -1;
		switch (state)
		{
		case LOCUST_STATE_IDLE:
			ani = LOCUST_ANI_FLYING_LEFT;
			break;
		case LOCUST_STATE_FLYING:
			if (nx > 0)
				ani = LOCUST_ANI_FLYING_RIGHT;
			else
				ani = LOCUST_ANI_FLYING_LEFT;
			break;
		case LOCUST_STATE_CHANGE_DIRECTION:
			if (nx > 0)
				ani = LOCUST_ANI_FLYING_RIGHT;
			else
				ani = LOCUST_ANI_FLYING_LEFT;
			break;
		case LOCUST_STATE_ATTACK:
			if (nx > 0)
				ani = LOCUST_ANI_FLYING_RIGHT;
			else
				ani = LOCUST_ANI_FLYING_LEFT;
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

void CLocust::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case LOCUST_STATE_IDLE:
		vx = 0;
		break;
	case LOCUST_STATE_FLYING:
		vx = LOCUST_FLYING_SPEED;
		break;
	case LOCUST_STATE_CHANGE_DIRECTION:
		vx = -vx;
		if (vx > 0)
		{
			nx = 1;
		}
		else
		{
			nx = 0;
		}
		break;
	case LOCUST_STATE_ATTACK:
		if (nx > 0)
		{
			vx = 0.07f;
		}
		else
		{
			vx = -0.07f;
		}
		vy -= 0.002f;
		break;
	case LOCUST_STATE_DIE:
		isDisplay = false;
		isEnable = false;
		break;
	case STATE_ITEM:
		vx = 0;
		isDisplay = false;
		break;
	default:
		break;
	}
}