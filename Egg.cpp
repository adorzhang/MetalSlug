#include "Egg.h"
#include "Game.h"
CEgg::CEgg(int item)
{
	width = height = 16;
	isDisplay = true;
	isEnable = true;
	this->item = item;
	SetState(EGG_STATE_NORMAL);
	blood = 0;
}

void CEgg::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (isEnable)
	{
		if (this->blood < 0)
		{
			if (item > 0)
				SetState(STATE_ITEM);
			else
				SetState(EGG_STATE_DESTROYED);
		}
	}
}
void CEgg::Render()
{
	int ani = -1;
	if (isEnable)
	{
		switch (state)
		{
		case EGG_STATE_NORMAL:
			ani = 0;
			break;
		case EGG_STATE_DESTROYED:
			break;
		case EGG_STATE_NONE:
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
void CEgg::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case EGG_STATE_NORMAL:
		break;
	case EGG_STATE_DESTROYED:
		isEnable = false;
		isDisplay = false;
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
void CEgg::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	if (isEnable)
	{
		l = x;
		if (state != STATE_ITEM)
		{
			t = y - EGG_BBOX_HEIGHT;
			r = x + EGG_BBOX_WIDTH;
		}
		else
		{
			if (item == 1)
			{
				t = y - ITEM_P_BBOX_HEIGHT;

				r = x + ITEM_P_BBOX_WIDTH;
			}

		}
		b = y;
	}
}