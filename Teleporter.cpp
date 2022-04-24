#include "Teleporter.h"
#include "Weapon.h"
#include "Spike.h"

CTeleporter::CTeleporter(int _item) :CEnemyObject()
{
	SetState(TELEPORTER_STATE_IDLE);
	timeWaitingToMove = 0;
	timeMoving = 0;
	item = _item;
	this->blood = 1;
	isEnable = true;
	isDisplay = true;
}

void CTeleporter::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (isEnable)
	{
		left = x;
		if (state != STATE_ITEM)
		{
			top = y - TELEPORTER_BBOX_HEIGHT;
			right = x + TELEPORTER_BBOX_WIDTH;
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

void CTeleporter::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (isEnable)
	{
		// Calculate dx, dy 
		CGameObject::Update(dt);

		if (!isMoving && !isStartCloak)
			timeWaitingToMove += dt;
		if (timeWaitingToMove >= TIME_START_MOVING)
		{
			SetState(TELEPORTER_STATE_MOVE);
			isStartMoving = true;

		}
		if (isStartMoving)
		{
			timeMoving += dt;
			if (timeMoving >= 200)
			{
				SetState(TELEPORTER_STATE_ATTACK);
				isStartMoving = false;
				timeWaitingToMove = 0;
				isMoving = false;
				timeMoving = 0;
			}
			int dir = rand() % 4 + 1;
			switch (dir)
			{
			case 1:
				if (x + 10 <= 320 - 64 - 25 && x >= 0)
					x += 10;
				break;
			case 2:
				if (x - 10 >= 88)
					x -= 10;
				break;
			case 3:
				if (y + 10 <= 320 - 64 -35 && y >= 0)
					y += 10;
				break;
			case 4:
				if (y - 10 >= 96)
					y -= 10;
				break;
			default:
				break;
			}
		}

		if (!isCloak && !isStartMoving)
			timeWaitingCloak += dt;
		if (timeWaitingCloak >= TIME_START_CLOAK)
		{
			SetState(TELEPORTER_STATE_CLOAK);
			isStartCloak = true;
		}
		if (isStartCloak)
		{
			timeCloak += dt;
			if (timeCloak >= TIME_CLOAK)
			{
				SetState(TELEPORTER_STATE_UNCLOAK);
				isStartCloak = false;
				timeWaitingCloak = 0;
				isCloak = false;
				timeCloak = 0;
			}
		}

		if (this->blood < 0)
		{
			if (item > 0)
				SetState(STATE_ITEM);
			else
				SetState(TELEPORTER_STATE_DIE);
		}
		for (int i = 0; i < list_weapon.size(); i++)
		{
			list_weapon[i]->Update(dt, coObjects);
		}



	}

}

void CTeleporter::Render()
{
	if (isEnable)
	{
		int a = 0;
		int ani = -1;
		bool flip = false;
		if (nx > 0)
		{
			flip = true;
		}
		else
			flip = false;
		ani = TELEPORTER_ANI_START;
		switch (state)
		{
		case TELEPORTER_STATE_MOVE:
		{
			ani = TELEPORTER_ANI_MOVE;		
			break;
		}
		case TELEPORTER_STATE_ATTACK:
		{
			ani = TELEPORTER_ANI_MOVE;
			if (animation_set->at(ani)->isFinish)
			{
				animation_set->at(ani)->isFinish = false;
				SetState(TELEPORTER_STATE_ATTACK);
			}
			break;
		}
		case TELEPORTER_STATE_CLOAK:
		{
			ani = TELEPORTER_ANI_CLOAK;
			break;
		}
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

void CTeleporter::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case TELEPORTER_STATE_MOVE:
		/*if (nx > 0)
		{
			vx = TELEPORTER_MOVE_SPEED;
		}
		else
		{
			vx = -TELEPORTER_MOVE_SPEED;
		}

		if (ny > 0)
		{
			vy = TELEPORTER_MOVE_SPEED;
		}
		else
		{
			vy = -TELEPORTER_MOVE_SPEED;
		}*/
		break;
	case TELEPORTER_STATE_DIE:
		isEnable = false;
		isDisplay = false;
		break;
	case TELEPORTER_STATE_CLOAK:
		vx = 0;
		vy = 0;
		break;
	case TELEPORTER_STATE_UNCLOAK:
		vx = 0;
		vy = 0;
		break;
	case TELEPORTER_STATE_ATTACK:
	{
		CWeapon* weapon = new CWeapon(WEAPON_TYPE_ENEMY_TELEPORTER);// Khởi tạo weapon
		weapon->SetPosition(x, y);
		weapon->SetDirection(nx);
		weapon->SetState(WEAPON_TELEPORTER_STATE_FLY);
		if (player)
			weapon->SetPlayerObject(player);
		list_weapon.push_back(weapon);
		SetState(TELEPORTER_STATE_MOVE);
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