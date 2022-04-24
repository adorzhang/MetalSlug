#include "Brick.h"
#include "Game.h"
CBrick::CBrick(float l, float t, float r, float b,int type)
{
	
	x = l;

	y = t;

	width = r - l + 1;

	height = -(b - t + 1);

	this->type = type;
	isDisplay = true;
	isEnable = true;

	SetState(BRICK_STATE_NORMAL);

}

void CBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (isEnable)
	{

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
		animation_set->at(ani)->Render(x, y);
		//RenderBoundingBox();
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
	
}