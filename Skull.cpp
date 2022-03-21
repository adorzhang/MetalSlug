#include "Skull.h"
#include "Brick.h"
#include "Worm.h"
#include "Dome.h"
#include "Weapon.h"
CSkull::CSkull(int _item) :CEnemyObject()
{
	SetState(SKULL_STATE_IDLE);
	this->blood = 1;
	item = _item;
	time_moving = 0;
	isEnable = true;
	isDisplay = true;


}

void CSkull::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (isEnable)
	{
		left = x;
		if (state != STATE_ITEM)
		{
			top = y - SKULL_BBOX_HEIGHT;
			right = x + SKULL_BBOX_WIDTH;
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

void CSkull::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	

	// Calculate dx, dy 
	CGameObject::Update(dt);
	// Simple fall down
	//if (isDisplay)
		//vy -= 0.001f * dt;
	if (this->blood < 0)
	{
		if (item > 0)
			SetState(STATE_ITEM);
		else
			SetState(SKULL_STATE_DIE);
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

		if (nx != 0) vx = 0;
		if (ny != 0) vy = 0;

		/*for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (dynamic_cast<CBrick*>(e->obj))
			{
				


		}*/

	}
	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i]; for (int i = 0; i < list_weapon.size(); i++)
	{
		list_weapon[i]->Update(dt, coObjects);
	}


}

void CSkull::Render()
{
	if (isEnable)
	{

		int ani = -1;
		switch (state)
		{


		case SKULL_STATE_IDLE:
			ani = SKULL_ANI_MOVE_LEFT;
			break;
		case SKULL_STATE_MOVE_UP:
			
				ani = SKULL_ANI_MOVE_RIGHT;
		
			break;

		case SKULL_STATE_MOVE_RIGHT:

			ani = SKULL_ANI_MOVE_RIGHT;
			break;
		case SKULL_STATE_MOVE_LEFT:
			ani = SKULL_ANI_MOVE_LEFT;
			break;

		case SKULL_STATE_MOVE_RIGHT_ATTACK:

			ani = SKULL_ANI_MOVE_RIGHT_ATTACK;
			break;
		case SKULL_STATE_MOVE_LEFT_ATTACK:
			ani = SKULL_ANI_MOVE_LEFT_ATTACK;
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
	for (int i = 0; i < list_weapon.size(); i++)
	{
		list_weapon[i]->Render();
	}
}

void CSkull::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case SKULL_STATE_IDLE:
		vx = 0;
		vy = 0;
		break;

	case SKULL_STATE_MOVE_RIGHT:
		vx = SKULL_MOVE_SPEED;
		break;
	case SKULL_STATE_MOVE_LEFT:
		vx = -SKULL_MOVE_SPEED;;
		break;
	
	case SKULL_STATE_MOVE_UP:
		vx = 0;
		vy = SKULL_MOVE_SPEED;
		break;
	case SKULL_STATE_MOVE_RIGHT_ATTACK:
		vx = 0;
		{
			if (isDisplay)
			{
				CWeapon* weapon = new CWeapon(WEAPON_TYPE_ENEMY_SKULL);// Khởi tạo weapon
				weapon->SetDirection(nx);
				weapon->SetPosition(x + SKULL_BBOX_WIDTH / 2, y);
				weapon->SetState(WEAPON_SKULL_STATE_FLY_RIGHT_UP);
				if (player)
					weapon->SetPlayerObject(player);
				list_weapon.push_back(weapon);
			}

		}
		break;
	case SKULL_STATE_DIE:
		isDisplay = false;
		isEnable = false;
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