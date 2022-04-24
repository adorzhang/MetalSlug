#include "EnemyObject1.h"

CEnemyObject1::CEnemyObject1():CEnemyObject()
{
	SetState(ENEMY1_STATE_WALKING);
}

void CEnemyObject1::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + ENEMY1_BBOX_WIDTH;
	bottom = y + ENEMY1_BBOX_HEIGHT;
}

void CEnemyObject1::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	// Calculate dx, dy 
	CGameObject::Update(dt);

	// Simple fall down
	vy -= 0.002f * dt;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();
	if (state != ENEMY1_STATE_DIE)
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

	}
	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CEnemyObject1::Render()
{
	if (state != ENEMY1_STATE_DIE)
	{
		int ani = -1;
		if (nx > 0)
			ani = ENEMY1_ANI_WALKING_RIGHT;
		else
			ani = ENEMY1_ANI_WALKING_LEFT;

		animation_set->at(ani)->Render(x, y);
		RenderBoundingBox();
	}
	
}

void CEnemyObject1::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case ENEMY1_STATE_IDLE:
		vx = 0;
		break;
	case ENEMY1_STATE_WALKING:
		if (nx > 0)
		{
			vx = ENEMY1_WALKING_SPEED;
		}

		else
		{
			vx = -ENEMY1_WALKING_SPEED;
		}
			
		break;
	case ENEMY1_STATE_DIE:
		break;
	default:
		break;
	}
}