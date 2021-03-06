#include "Brick.h"
#include "Game.h"
#include "Utils.h"
CBrick::CBrick(float l, float t, float r, float b, float angle,int type)
{
	
	x = l;

	y = t;

	width = r - l + 1;
	this->angle = angle;
	height = -(b - t + 1);

	this->type = type;
	isDisplay = true;
	isEnable = true;

	SetState(BRICK_STATE_NORMAL);

}

void CBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);
	if (isEnable)
	{
		x += dx;
		y += dy;
	}
}
void CBrick::Render()
{
	int ani = -1;
	if (isEnable)
	{
		switch (state)
		{
		case BRICK_STATE_NORMAL:
			ani = 0;
			break;
		case BRICK_STATE_DESTROYED:
			break;
		case BRICK_STATE_NONE:
			break;
		}
		animation_set->at(ani)->Render(x, y, 255, false, 1, 0.8);
		//DebugOut(L"[INFO] Brick angle!: %f\n", this->angle);
		RenderBoundingBox();
	}
	
}
void CBrick::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case BRICK_STATE_NORMAL:
		break;
	case BRICK_STATE_DESTROYED:
		break;
	case BRICK_STATE_NONE:
		isEnable = false;
		break;
	default:
		break;
	}
}
void CBrick::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	if (isEnable)
	{
		l = x;
		t = y - height;
		r = x + width;
		b = y;
	}
	else
	{
		DebugOut(L"[INFO] Brick disable false!\n");
	}
	
}