#include "Item.h"
#include "Game.h"
CItem::CItem(int type)
{
	isDisplay = true;
	isEnable = true;
	isEnable = true;
	this->type = type;

}

void CItem::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (isEnable)
	{
		switch (type)
		{
		case 1:
			break;
		case 2:
			break;
		case 3:
			break;
		case 4:
			break;
		default:
			break;
		}
	}
}
void CItem::Render()
{
	int ani = -1;
	if (isEnable)
	{
		switch (type)
		{
		case 1:
			ani = ITEM_ANI_H_ROCKET;
			break;
		case 2:
			ani = ITEM_ANI_H_GREEN;
			break;
		case 3:
			ani = ITEM_ANI_FLASH;
			break;
		case 4:
			ani = ITEM_ANI_P_GREEN;
			break;
		case 5:
			ani = ITEM_ANI_KEY;
			break;
		default:
			break;
		}
		animation_set->at(ani)->Render(x, y);
	}

}
void CItem::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case ITEM_STATE_IDLE:
		vx = 0;
		break;
	case ITEM_STATE_DIE:
		isDisplay = false;
		isEnable = false;
		break;
	default:
		break;
	}
}
void CItem::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	if (isEnable)
	{
		l = x;
		t = y - ITEM_BBOX_HEIGHT;
		r = x + ITEM_BBOX_WIDTH;
		b = y;
	}

}