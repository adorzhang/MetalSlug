#include "Spider.h"

CSpider::CSpider() :CEnemyObject()
{
	SetState(SPIDER_STATE_IDLE);
}

void CSpider::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y- SPIDER_BBOX_HEIGHT;
	right = x + SPIDER_BBOX_WIDTH;
	bottom = y;
}

void CSpider::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	// Calculate dx, dy 
	CGameObject::Update(dt);

	// Simple fall down
	vy -= 0.002f * dt;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();
	if (state != SPIDER_STATE_DIE)
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

	}
	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CSpider::Render()
{
	if (state != SPIDER_STATE_DIE)
	{
		int ani = -1;
		if (nx > 0)
			ani = SPIDER_ANI_MOVE_RIGHT;
		else
			ani = SPIDER_ANI_MOVE_LEFT;

		animation_set->at(ani)->Render(x, y);
		RenderBoundingBox();
	}

}

void CSpider::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case SPIDER_STATE_IDLE:
		vx = 0;
		break;
	case SPIDER_STATE_MOVE:
		if (nx > 0)
		{
			vx = SPIDER_MOVE_SPEED;
		}
		else
		{
			vx = -SPIDER_MOVE_SPEED;
		}
		break;
	case SPIDER_STATE_DIE:
		break;
	default:
		break;
	}
}