#include "Cannon.h"
#include "Weapon.h"
CCannon::CCannon(int _item):CEnemyObject()
{
	SetState(CANNON_STATE_IDLE);
	this->blood = 1;
	isEnable = true;
	isDisplay = true;
	item = _item;
}

void CCannon::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (isEnable)
	{
		left = x;
		if (state != STATE_ITEM)
		{
			top = y - CANNON_BBOX_HEIGHT;
			right = x + CANNON_BBOX_WIDTH;
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

void CCannon::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (this->blood < 0)
	{
		if (item > 0)
			SetState(STATE_ITEM);
		else
			SetState(CANNON_STATE_DIE);
	}
	if (isEnable)
	{
		for (int i = 0; i < list_weapon.size(); i++)
		{
			list_weapon[i]->Update(dt, coObjects);
		}
	}
	
}

void CCannon::Render()
{
	if (isEnable)
	{
		
		int ani = -1;
		if (state == STATE_ITEM)
		{
			ani = item;
			animation_item_set->at(ani - 1)->Render(x, y);
		}
		else
		{
			if (!isFireVertical)
			{
				ani = CANNON_ANI_FIRE_VERTICAL;

				if (isDisplay)
				{
					animation_set->at(ani)->Render(x, y);
				}

				if (animation_set->at(ani)->isFinish)
				{
					isFireVertical = !isFireVertical;
					animation_set->at(ani)->isFinish = false;
					SetState(CANNON_STATE_FIRE_VERTICAL);
				}

			}
			else
			{
				ani = CANNON_ANI_FIRE_HORIZONTAL;
				if (isDisplay)
				{
					animation_set->at(ani)->Render(x, y);
				}

				if (animation_set->at(ani)->isFinish)
				{
					isFireVertical = !isFireVertical;
					animation_set->at(ani)->isFinish = false;
					SetState(CANNON_STATE_FIRE_HORIZONTAL);
				}
			}
			RenderBoundingBox();
		}
		
		for (int i = 0; i < list_weapon.size(); i++)
		{
			list_weapon[i]->Render();
		}
	}
	
}

void CCannon::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case CANNON_STATE_IDLE:
		vx = 0;
		break;
	case CANNON_STATE_DIE:
		isDisplay = false;
		isEnable = false;
		break;
	case CANNON_STATE_FIRE_VERTICAL:
	{
		CWeapon* weapon_up = new CWeapon(WEAPON_TYPE_ENEMY_CANNONS);// Khởi tạo weapon
		weapon_up->SetPosition((x + CANNON_BBOX_WIDTH/2), y);
		weapon_up->SetState(WEAPON_CANNONS_STATE_FIRE_VERTICAL_UP);
		if (player)
			weapon_up->SetPlayerObject(player);
		list_weapon.push_back(weapon_up);

		CWeapon* weapon_down = new CWeapon(WEAPON_TYPE_ENEMY_CANNONS);// Khởi tạo weapon
		weapon_down->SetPosition((x + CANNON_BBOX_WIDTH/2) , y);
		weapon_down->SetState(WEAPON_CANNONS_STATE_FIRE_VERTICAL_DOWN);
		if (player)
			weapon_down->SetPlayerObject(player);
		list_weapon.push_back(weapon_down);

		break;
	}
	case CANNON_STATE_FIRE_HORIZONTAL:
	{
		CWeapon* weapon_left = new CWeapon(WEAPON_TYPE_ENEMY_CANNONS);// Khởi tạo weapon
		weapon_left->SetPosition(x,y);
		weapon_left->SetState(WEAPON_CANNONS_STATE_FIRE_HORIZONTAL_LEFT);
		if (player)
			weapon_left->SetPlayerObject(player);
		list_weapon.push_back(weapon_left);

		CWeapon* weapon_right = new CWeapon(WEAPON_TYPE_ENEMY_CANNONS);// Khởi tạo weapon
		weapon_right->SetPosition(x,y);
		weapon_right->SetState(WEAPON_CANNONS_STATE_FIRE_HORIZONTAL_RIGHT);
		if (player)
			weapon_right->SetPlayerObject(player);
		list_weapon.push_back(weapon_right);

		break;
	}
	case STATE_ITEM:
		vx = 0;
		vy = 0;
		isDisplay = false;
		break;
	default:
		break;
	}
}

