#include "Eyeball.h"
#include "Weapon.h"
#include "Spike.h"
CEyeball::CEyeball(int _item) :CEnemyObject()
{
	SetState(EYEBALL_STATE_IDLE);
	timeWaitingToMove = 0;
	timeMoving = 0;
	item = _item;
	this->blood = 1;
	isEnable = true;
	isDisplay = true;
}

void CEyeball::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (isEnable)
	{
		left = x;
		if (state != STATE_ITEM)
		{
			top = y - EYEBALL_BBOX_HEIGHT;
			right = x + EYEBALL_BBOX_WIDTH;
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

void CEyeball::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (isEnable)
	{
		
		// Calculate dx, dy 
		CGameObject::Update(dt);
		
		if(!isMoving)
			timeWaitingToMove += dt;

		if (timeWaitingToMove >= TIME_START_MOVING)
		{
			SetState(EYEBALL_STATE_MOVE);
			isStartMoving = true;
			
		}

		if (isStartMoving)
		{
			timeMoving += dt;
			if (timeMoving >= TIME_MOVING)
			{
				SetState(EYEBALL_STATE_CLOSE_EYE);
				isStartMoving = false;
				timeWaitingToMove = 0;
				isMoving = false;
				timeMoving = 0;
			}
			x += dx;
			y += dy;
		}
		if (this->blood < 0)
		{
			if (item > 0)
				SetState(STATE_ITEM);
			else
				SetState(EYEBALL_STATE_DIE);
		}
		for (int i = 0; i < list_weapon.size(); i++)
		{
			list_weapon[i]->Update(dt, coObjects);
		}


		
	}
	
}

void CEyeball::Render()
{
	if (isEnable)
	{
		int ani = -1;
		bool flip = false;
		if (nx > 0)
		{
			flip = true;
		}
		else
			flip = false;
		ani = EYEBALL_ANI_LEFT;
		switch (state)
		{
		case EYEBALL_STATE_MOVE:
			break;
		case EYEBALL_STATE_CLOSE_EYE:
		{
			ani = EYEBALL_ANI_CLOSE_EYES;
			if (animation_set->at(ani)->isFinish)
			{
				animation_set->at(ani)->isFinish = false;
				SetState(EYEBALL_STATE_ATTACK);
			}
		}
			break;
		case EYEBALL_STATE_IDLE:
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
		for (int i = 0; i < list_weapon.size(); i++)
		{
			list_weapon[i]->Render();
		}
	}

}

void CEyeball::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case EYEBALL_STATE_IDLE:
		vx = 0;
		vy = 0;
		break;
	case EYEBALL_STATE_MOVE:
		if (nx > 0)
		{
			vx = EYEBALL_MOVE_SPEED;
		}
		else
		{
			vx = -EYEBALL_MOVE_SPEED;
		}

		if (ny > 0)
		{
			vy = EYEBALL_MOVE_SPEED;
		}
		else
		{
			vy = -EYEBALL_MOVE_SPEED;
		}
		break;
	case EYEBALL_STATE_DIE:
		isEnable = false;
		isDisplay = false;
		break;
	case EYEBALL_STATE_CLOSE_EYE:
		vx = 0;
		vy = 0;
		break;
	case EYEBALL_STATE_ATTACK:
		{
			CWeapon* weapon = new CWeapon(WEAPON_TYPE_ENEMY_EYEBALL);// Khởi tạo weapon
			weapon->SetPosition(x, y);
			weapon->SetDirection(nx);
			weapon->SetState(WEAPON_EYEBALL_STATE_FLY);
			list_weapon.push_back(weapon);
		}
		
		break;
	case STATE_ITEM:
		if (item > 0)
		{
			hasItem = true;
		}
		vy = 0;
		vx = 0;
		isDisplay = false;
		break;
	default:
		break;
	}
}